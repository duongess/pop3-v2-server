#pragma once
#include "../../common/protocol/pop3-v2.h"
#include "../../common/utils.h"

// API khởi chạy / dừng dịch vụ TCP
int startPop3v2(const string& host, const string& port, int kBufferSize = 4096);
int stopPop3v2();
