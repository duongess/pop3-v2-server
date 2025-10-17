// session.h
#pragma once

struct Session {
    string username;
    string token;
    string host;
    socket_handle_t socket_fd;
    bool isAuthenticated = false;
};
