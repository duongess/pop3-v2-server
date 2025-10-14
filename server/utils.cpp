#include "utils.h"


void menuServer() {
    std::cout << "\n=== Network System Menu ===\n";
    std::cout << "1) Start TCP service\n";
    std::cout << "c) Create account\n";
    std::cout << "j) Join account\n";
    std::cout << "q) Quit\n> ";
    return;
}

void registerServer(Server &server) {
    std::string username, password;
    std::cout << "Register a server account" << std::endl;
    std::cout << "Server Name: "; std::getline(std::cin, username);
    std::cout << "Server Password: "; std::getline(std::cin, password);
    server.signUp(username, password);
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