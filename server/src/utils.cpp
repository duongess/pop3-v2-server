#include "utils.h"


void menuServer() {
    console.log(COLOR::GREEN, "\n=== Network System Menu ===\n");
    std::cout << "1) Start TCP service\n";
    std::cout << "2) Start Pop3 V2 service\n";
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