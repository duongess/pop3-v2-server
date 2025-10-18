#include "../common/utils.h"
#include "../common/protocol.h"

class Client {
    private:
        std::string host;
        std::string port;
        std::string token;
        Protocol client;
    public:
        Client(const std::string& host, const std::string& port) {
            this->host = host;
            this->port = port;
        }
        std::string request(const std::string& message);
}