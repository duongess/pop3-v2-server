#pragma once
#include "utils.h"
#include "../types/response.h"
#include "../types/auth.h"

class Protocol {
protected:
    socket_handle_t sock = invalid_socket_handle;
    static std::atomic<bool> stop_flag;

public:
    AuthSession sess;
    Protocol() = default;
    virtual ~Protocol();
    socket_handle_t getSocket(){return this->sock;}

    bool connectTo(const std::string& host, const std::string& port);
    bool isConnected();
    bool bindAndListen(const std::string& host, const std::string& port);
    bool acceptClient(Protocol& client);
    void close();
    void clean();

    static void requestStop();
    static bool shouldStop();
    
    bool sendData(const std::string& data);
    Response receiveData(const size_t& size = 1024);
};
