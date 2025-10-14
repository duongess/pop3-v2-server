#pragma once
#include <iostream>
#include <string>
#include <cstring>
#include "../../common/utils.h"
#include "../../common/protocol.h"

// Run a single TCP echo interaction:
// connects to host:port, sends message, prints echo, then exits.
int sendMessage(const std::string&  host, const std::string&  port, const std::string& message);

