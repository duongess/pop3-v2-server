#pragma once
#include "../../common/protocol.h"
#include "../../common/utils.h"

// API khởi chạy / dừng dịch vụ TCP
int start_tcp(const std::string& host, const std::string& port, int kBufferSize = 4096);
int stop_tcp();
