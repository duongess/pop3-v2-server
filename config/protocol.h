#pragma once
#include "host.h"
#include "port.h"
#include "buffer.h"
#include <string>

namespace Config {
    struct TCPConfig {
        std::string hostServer = HOST_SERVER_DEFAULT;
        std::string hostClient = HOST_CLIENT_DEFAULT;
        std::string port = PORT_TCP_DEFAULT;
        int bufferSize = BUFFER_SIZE;
    };

    struct Pop3v2Config {
        std::string hostServer = HOST_SERVER_DEFAULT;
        std::string hostClient = HOST_CLIENT_DEFAULT;
        std::string port = PORT_POP3V2_DEFAULT;
        int bufferSize = BUFFER_SIZE;
    };
}
