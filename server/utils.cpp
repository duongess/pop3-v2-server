#include "utils.h"


void menuServer() {
    console.log(COLOR::GREEN, "\n=== Network System Menu ===\n");
    cout << "1) Start TCP service\n";
    cout << "c) Create account\n";
    cout << "j) Join account\n";
    cout << "q) Quit\n> ";
    return;
}

void registerServer(Server &server) {
    string username, password;
    cout << "Register a server account" << endl;
    cout << "Server Name: "; getline(cin, username);
    cout << "Server Password: "; getline(cin, password);
    server.signUp(username, password);
}

void joinServer(Server &server) {
    string username, password;
    cout << "Log in: " << endl;
    cout << "Server Name: "; getline(cin, username);
    cout << "Server Password: "; getline(cin, password);
    AuthResult status = server.signIn(username, password);
    switch (status) {
        case AuthResult::Ok :
            cout << "Log in OK " << endl;
            return;
        case AuthResult::UserNotFound:
        case AuthResult::WrongPassword:
            cout << "Log in failure " << endl;
            break;
    }
    server.checkAccout();
}