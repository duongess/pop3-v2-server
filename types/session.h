// session.h
#pragma once
#include "../common/utils.h"

struct Session {
    std::string username;
    std::string token;
    std::string host;
    socket_handle_t socket_fd;
    bool isAuthenticated = false;
};
