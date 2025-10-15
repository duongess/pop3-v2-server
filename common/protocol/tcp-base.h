#pragma once
#include <string>
#include <atomic>
#include <iostream>
#include "../utils.h"
#include "../../types/api.h"

class TCPBase {
protected:
    socket_handle_t sock = invalid_socket_handle;
    static std::atomic<bool> stop_flag;

public:
    TCPBase() = default;
    virtual ~TCPBase();

    bool connectTo(const std::string& host, const std::string& port);
    bool bindAndListen(const std::string& host, const std::string& port);
    bool acceptClient(TCPBase& client);
    void close();

    static void requestStop();
    static bool shouldStop();
};
