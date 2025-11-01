#include "pop3-v2-session.h"

Pop3V2Session::Pop3V2Session(TcpSocket& slave, Pop3V2ServerConfig* conf) : Session(slave, conf), pop3V2Conf(conf) {
    // slave.send("+OK POP3 server ready\r\n");
}

void Pop3V2Session::doUnknown(std::string cmd_argv[], int cmd_argc) {
    slave.send("-ERR Unknown command\r\n");
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
    if (this->pop3V2Conf->verify(this->username, pass)) {
        console.success("[AUTH] OK - User '" + this->username + "' authenticated successfully.");
        
        // (Chuyển state sang TRANSACTION)
        // (Gọi logic lock maildrop)

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
    
}