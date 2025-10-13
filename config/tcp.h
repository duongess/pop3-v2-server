#pragma once
#include "host.h"
#include "port.h"
#include "buffer.h"
#include <string>

namespace Config {
    struct TCPConfig {
        std::string host = HOST_DEFAULT;
        int port = PORT_TCP_DEFAULT;
        int bufferSize = BUFFER_SIZE;
    };
}
