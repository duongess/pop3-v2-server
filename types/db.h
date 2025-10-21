#pragma once
#include <string>

struct User {
    int userId;
    std::string username;
    std::string passwordHash;
    ssize_t createdAt;
};


struct Mail {
    int mailId;
    int userId;
    std::string subject;
    std::string body;
    ssize_t receivedAt;
};

// Join...