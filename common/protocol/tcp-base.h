#pragma once
#include <string>
#include <atomic>
#include <iostream>
#include "../utils.h"
#include "../../types/api.h"

class TCPBase {
protected:
    socket_handle_t sock = invalid_socket_handle;
    static atomic<bool> stop_flag;

public:
    TCPBase() = default;
    virtual ~TCPBase();

    bool connectTo(const string& host, const string& port);
    bool bindAndListen(const string& host, const string& port);
    bool acceptClient(TCPBase& client);
    void close();
    void clean();

    static void requestStop();
    static bool shouldStop();
};
