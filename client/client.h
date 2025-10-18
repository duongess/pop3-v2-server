#include "../common/utils.h"
#include "../common/protocol.h"

class Client {
    private:
        std::string host;
        std::string port;
        std::string token;
        Protocol client;
        std::string request(const std::string& message);
    public:
        void setIp(const std::string& host, const std::string& port) {
            this->host = host;
            this->port = port;
        }

        void sendText(const std::string& message);
        void sendPopv2(const std::string& message);
};