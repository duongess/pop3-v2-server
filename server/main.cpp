// Entry point for server
#include <iostream>
#include <string>
#include "server.h"

int main(int argc, char* argv[]) {
  const char* host = nullptr; // nullptr with AI_PASSIVE binds to all
  const char* port = "6000";
  if (argc >= 2) {
    port = argv[1];
  }

  std::cout << "Starting server on port " << port << std::endl;
  return run_server(host, port);
}
