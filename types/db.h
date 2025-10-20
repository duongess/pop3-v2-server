#pragma once
#include <string>

struct User {
    std::string username;
    std::string passwordHash;
};


struct Mail {
    int mailId;
    int userId;
    std::string subject;
    std::string body;
    long long receivedAt;
};

// Join...