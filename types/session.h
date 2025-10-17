// session.h
#pragma once
#include <string>
#include <chrono>
#include "../common/utils.h"


struct Session {
    string username;
    string token;
    string host;
    socket_handle_t socket_fd;
    bool isAuthenticated = false;
};
