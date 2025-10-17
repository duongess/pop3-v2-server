#pragma once
#include "../../common/protocol/tcp.h"
#include "../../common/utils.h"

// Run a single TCP echo interaction:
// connects to host:port, sends message, prints echo, then exits.
int sendMessage(const string&  host, const string&  port, const string& message);

