#pragma once
#include "../../common/protocol/tcp.h"
#include "../../common/utils.h"

// API khởi chạy / dừng dịch vụ TCP
int start_tcp(const string& host, const string& port, int kBufferSize = 4096);
int stop_tcp();
