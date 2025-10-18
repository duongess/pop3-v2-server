#pragma once
#include "../../common/protocol.h"
#include "../../common/utils.h"

// API khởi chạy / dừng dịch vụ TCP
int runTCP(const std::string& host, const std::string& port, int kBufferSize = 4096);
int stopTCP();
