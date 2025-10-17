#pragma once
#include "host.h"
#include "port.h"
#include "buffer.h"
#include <string>

namespace Config {
    struct TCPConfig {
        string hostServer = HOST_SERVER_DEFAULT;
        string hostClient = HOST_CLIENT_DEFAULT;
        string port = PORT_TCP_DEFAULT;
        int bufferSize = BUFFER_SIZE;
    };
}
