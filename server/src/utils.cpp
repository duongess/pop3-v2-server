#include "utils.h"



void menuServer(const std::string& host, const std::string& port) {
    console.log("=== POP3 v2 Server ===");
    console.log("Host: ", host, "  Port: ", port);
    console.log("[c] Configure/Register server");
    console.log("[j] Join (attach to running server info)");
    console.log("[e] Stop POP3 v2 service");
    console.log("[r] Restart POP3 v2 service");
    console.log("[q] Quit");
    console.log(">> ");
}

void registerServer(Server &server) {
    std::string username, password;
    std::cout << "Register a server account" << std::endl;
    std::cout << "Server Name: "; std::getline(std::cin, username);
    std::cout << "Server Password: "; std::getline(std::cin, password);
    AuthResult status = server.signUp(username, password);
    switch (status) {
        case AuthResult::Ok:
            std::cout << "Register successful" << std::endl;
            return;
        case AuthResult::AlreadyExists: 
            console.log("Account AlreadyExists");
    }
}

void joinServer(Server &server) {
    std::string username, password;
    std::cout << "Log in: " << std::endl;
    std::cout << "Server Name: "; std::getline(std::cin, username);
    std::cout << "Server Password: "; std::getline(std::cin, password);
    AuthResult status = server.signIn(username, password);
    switch (status) {
        case AuthResult::Ok :
            std::cout << "Log in OK " << std::endl;
            return;
        case AuthResult::UserNotFound:
        case AuthResult::WrongPassword:
            std::cout << "Log in failure " << std::endl;
            break;
    }
    server.checkAccout();
}

std:: string convertToJSONFile(std::vector<Mail> mails){
    std::ostringstream json;
    json << "{\n  \"emails\": [\n";

    for (size_t i = 0; i < mails.size(); ++i) {
        const Mail& mail = mails[i];
        json << "    {\n";
        json << "      \"mailId\": " << mail.mailId << ",\n";
        json << "      \"userId\": " << mail.userId << ",\n";
        json << "      \"subject\": \"" << escapeJSON(mail.subject) << "\",\n";
        json << "      \"body\": \"" << escapeJSON(mail.body) << "\",\n";
        json << "      \"receivedAt\": " << mail.receivedAt << "\n";
        json << "    }";
        if (i != mails.size() - 1) json << ",";
        json << "\n";
    }

    json << "  ]\n}\n";

    // std::string filename = "emails.json";
    // std::ofstream file(filename);
    // if (!file.is_open()) {
    //     std::cerr << "Error: cannot open file for writing: " << filename << std::endl;
    //     return "";
    // }

    // file << json.str();
    // file.close();

    console.debug(json.str());
    return json.str();
}

// Helper function to escape JSON special characters
std::string escapeJSON(const std::string& input) {
    std::ostringstream escaped;
    for (char c : input) {
        switch (c) {
            case '\"': escaped << "\\\""; break;
            case '\\': escaped << "\\\\"; break;
            case '\b': escaped << "\\b"; break;
            case '\f': escaped << "\\f"; break;
            case '\n': escaped << "\\n"; break;
            case '\r': escaped << "\\r"; break;
            case '\t': escaped << "\\t"; break;
            default:
                if (static_cast<unsigned char>(c) < 0x20)
                    escaped << "\\u" << std::hex << std::uppercase << (int)c;
                else
                    escaped << c;
        }
    }
    return escaped.str();
}