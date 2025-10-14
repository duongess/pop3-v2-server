// session.h
#pragma once
#include <string>
#include <chrono>
#include "../common/utils.h"

struct Session {
    std::string username;
    std::string token;
    std::string host;
    socket_handle_t socket_fd;
    bool isAuthenticated = false;
};
