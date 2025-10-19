#pragma once
#include <string>

struct User {
    int userId;
    std::string username;
    std::string passwordHash;
    std::string createdAt;
};


struct Mail {
    int mailId;
    int userId;
    std::string subject;
    std::string body;
    long long receivedAt;
};

// Join...