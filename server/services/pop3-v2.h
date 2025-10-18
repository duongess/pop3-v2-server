#pragma once
#include "../../common/protocol.h"
#include "../../common/utils.h"
#include "../server.h"

// API khởi chạy / dừng dịch vụ TCP
int runPop3V2(Server& server, const std::string& host, const std::string& port, int kBufferSize = 4096);
int stopPop3V2();
