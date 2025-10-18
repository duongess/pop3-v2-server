#pragma once
#include "protocol.h"
#include "host.h"
#include "port.h"
#include "buffer.h"

namespace Config {
    struct AppConfig {
        TCPConfig tcp;
        Pop3v2Config pop3V2;
        // chừa chỗ để thêm service khác
        // UDPConfig udp;
        // HTTPConfig http;
    };

    inline AppConfig defaultConfig() {
        AppConfig cfg;
        // nếu cần override mặc định thì làm ở đây
        return cfg;
    }
}
