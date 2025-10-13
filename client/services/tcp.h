#pragma once

#include <string>

// Run a single TCP echo interaction:
// connects to host:port, sends message, prints echo, then exits.
int run_tcp_client(const std::string&  host, const std::string&  port, const std::string& message);

