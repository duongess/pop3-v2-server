#pragma once

class TCP {
protected:
    socket_handle_t sock = invalid_socket_handle;
    static atomic<bool> stop_flag;

public:
    TCP() = default;
    virtual ~TCP();

    bool connectTo(const string& host, const string& port);
    bool bindAndListen(const string& host, const string& port);
    bool acceptClient(TCP& client);
    void close();
    void clean();

    static void requestStop();
    static bool shouldStop();

    bool sendData(const string& data);
    Response receiveData(const size_t& size = 1024);
};
