#pragma once
#include "../common/protocol/index.h"

class Client {
    private:
        TCP<std::string> client;
        std::string token;
        std::string host;
        std::string port;
    public:
        Client(std::string& host, std::string& port) {
            this->host = host;
            this->port = port;
        }

        std::string sendTCP(const std::string& message);

}