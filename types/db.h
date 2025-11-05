#pragma once
#include <string>

struct User {
    int userId;
    std::string username;
    std::string passwordHash;
    ssize_t createdAt;
    std::string sessionToken;
};

class SetUser {
public:
    int userId;
    std::string username;
    std::string password;
    std::string sessionToken;
    SetUser(int i, const std::string& u, const std::string& p, const std::string& t)
    : userId(i), username(u), password(p),
      sessionToken(t) {}

};


struct Mail {
    int mailId;
    int userId;
    std::string subject;
    std::string body;
    ssize_t receivedAt;
};


// Simple metadata returned for LIST command
struct MailInfo {
    int mailId;
    size_t size;
};


// Join...