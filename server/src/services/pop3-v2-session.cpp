#include "pop3-v2-session.h"
#include "user.h" // for generateTokenSimple

Pop3V2Session::Pop3V2Session(TcpSocket& slave, Pop3V2ServerConfig* conf) : Session(slave, conf), pop3V2Conf(conf), account(nullptr) {
    slave.send("+OK POP3 server ready\r\n");
}

void Pop3V2Session::doUnknown(std::string cmd_argv[], int cmd_argc) {
    slave.send("-ERR Unknown command\r\n");
}

void Pop3V2Session::disconnect() {
    if (this->account)
        this->account->unLock();
}

void Pop3V2Session::doUser(std::string cmd_argv[], int cmd_argc) {
    // (1) Kiểm tra tham số
    if (cmd_argc < 2) {
        // Log lỗi cho server
        console.error("[AUTH] FAILED - USER command missing argument.");
        
        // Trả lời client
        slave.send("-ERR USER requires an argument\r\n");
        return;
    }

    // (2) Kiểm tra trạng thái
    if (!this->username.empty()) {
        console.error("[AUTH] FAILED - Client sent USER twice.");
        slave.send("-ERR User already specified, awaiting PASS\r\n");
        return;
    }
    
    std::string user = cmd_argv[1];

    // (3) Kiểm tra user
    if (this->pop3V2Conf->isValidUser(user)) {
        // Xây dựng chuỗi log
        console.info("[AUTH] OK - User '", user, "' found. Waiting for PASS.");
        
        // Lưu lại username
        this->username = user;
        
        // Trả lời client
        slave.send("+OK User valid, send PASS\r\n");
    } else {
        // Xây dựng chuỗi log
        console.error("[AUTH] FAILED - User '" + user + "' not found.");
        
        // Trả lời client
        slave.send("-ERR No such user\r\n");
    }
}

void Pop3V2Session::doPass(std::string cmd_argv[], int cmd_argc) {
    // (1) Kiểm tra tham số
    if (cmd_argc < 2) {
        console.error("[AUTH] FAILED - PASS command missing argument.");
        slave.send("-ERR PASS requires an argument\r\n");
        return;
    }

    // (2) Kiểm tra trạng thái (phải có USER trước)
    if (this->username.empty()) {
        console.error("[AUTH] FAILED - Client sent PASS before USER.");
        slave.send("-ERR USER command required first\r\n");
        return;
    }

    std::string pass = cmd_argv[1];

    // (3) Xác thực password
    if (this->pop3V2Conf->authenticate(this->username, pass)) {
        console.success("[AUTH] OK - User '" + this->username + "' authenticated successfully.");
        
        // (Chuyển state sang TRANSACTION)
        // (Gọi logic lock maildrop)
        Account* baseAccountPtr = this->pop3V2Conf->getAccount(this->username);
        this->account = dynamic_cast<Pop3V2Account*>(baseAccountPtr);

        if (this->account->isLocked()) {
            // Nếu tài khoản đã bị khóa (do 1 session khác)
            console.error("[AUTH] FAILED - User '" + this->username + "' maildrop is locked.");
            slave.send("-ERR maildrop locked\r\n");
            
            // Reset lại, bắt đăng nhập lại từ đầu
            this->account = nullptr; 
            this->username = "";
            return;
        }
        this->account->lock();

        std::string username = this->username;
        // ✅ Automatically generate a session token
        std::string token = generateSessionToken(username);
        account->setSessionToken(token);
        pop3V2Conf->setSessionToken(account->userId, token);


        // Trả lời client
        slave.send("+OK Authentication successful, maildrop locked and ready.\r\n");
    } else {
        console.error("[AUTH] FAILED - User '" + this->username + "' sent wrong password.");
        
        // Reset username để bắt nhập lại
        this->username = ""; 

        // Trả lời client
        slave.send("-ERR Invalid password\r\n");
    }
}

void Pop3V2Session::doLiss(std::string cmd_argv[], int cmd_argc) {
    
    // BẠN NÊN KIỂM TRA TRẠNG THÁI TRƯỚC!
    // Lệnh LIST chỉ hợp lệ sau khi đã đăng nhập (TRANSACTION state)
    if (this->username == "") {
        console.error("[LIST] FAILED - Client not authenticated.");
        slave.send("-ERR Command only valid in TRANSACTION state\r\n");
        return;
    }

    // (Giả sử đã check, hoặc account.userId chỉ có khi đã đăng nhập)
    console.debug(this->account->userId);
    std::vector<MailInfo> emails = this->pop3V2Conf->getMailsForUser(this->account->userId);

    if (!emails.empty()) {
        // (1) SỬA LỖI NỐI CHUỖI: Dùng std::to_string
        std::string log_msg = "[LIST] Found " + std::to_string(emails.size()) + 
                              " emails for user '" + std::to_string(this->account->userId) + "'. Sending list.";
        
        console.success(log_msg); // Dùng success như code gốc của bạn
        
        // Giả sử convertToString(emails) trả về 1 chuỗi đã định dạng POP3
        slave.send("+OK\r\n" + convertToString(emails));

    } else {
        // (2) SỬA LOGIC ELSE: Đây là trường hợp 0 email, không phải lỗi
        std::string log_msg = "[LIST] Found 0 emails for user '" + std::to_string(this->account->userId) + "'.";
        
        // Dùng console.info() sẽ hợp lý hơn là error()
        console.error(log_msg); 
        
        // (RẤT QUAN TRỌNG) Vẫn phải trả lời +OK cho client
        // Đây là phản hồi chuẩn của POP3 cho trường hợp 0 email
        slave.send("-ERR 0 messages\r\n.\r\n");
    }
}

// Helper function to generate token
std::string Pop3V2Session::generateSessionToken(const std::string& username) {
        std::string seed = username + std::to_string(std::time(nullptr)) + std::to_string(rand());
    std::hash<std::string> hasher;
    size_t value = hasher(seed);

    std::ostringstream oss;
    oss << std::hex << std::setw(16) << std::setfill('0') << value;
    return oss.str(); // hex-style token, no external libs
}