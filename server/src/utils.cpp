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

std::string convertToString(std::vector<MailInfo> mails){
    std::string cur = "";
    for (MailInfo mail:mails) {
        cur += std::to_string(mail.mailId) + " " + std::to_string(mail.size) + "\r\n";
    }
    cur += "\r\n"
    console.debug(cur);
    return cur;
}