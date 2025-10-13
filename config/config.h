#pragma once
#include "tcp.h"
#include "host.h"
#include "port.h"
#include "buffer.h"
// thêm udp.h, http.h ... sau

namespace Config {
    struct AppConfig {
        TCPConfig tcp;

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
