#pragma once
#include <iostream>
#include <string>
#include <cstring>
#include <atomic>
#include "../../common/utils.h"

int start_tcp(const std::string& host, const std::string& port, const int& kBufferSize);
int stop_tcp();

