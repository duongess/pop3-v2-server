// session.h
#pragma once
#include "../common/utils.h"

struct Session {
    std::string userId;
    std::string token;
    std::string host;
    bool isAuthenticated = false;
};
