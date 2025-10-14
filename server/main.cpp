// Entry point for server
#include <iostream>
#include <string>
#include <thread>
#include <atomic>
#include "services/tcp.h"
#include "../config/config.h"
#include "../common/utils.h"
#include "server.hpp"
#include "utils.h"
#include "../types/services.h"

int main(int argc, char* argv[]) {
  Config::AppConfig cfg = Config::defaultConfig();

  std::thread tcp_thread;
  Services currentServices;

  std::string host = cfg.tcp.hostServer, port;

  if (argc >= 2) {
    host = argv[1];
  }

  Server server = Server(host);
  registerServer(server);
  server.checkAccout();

  menuServer();
  // Main loop
  while (true) {
    std::string choice;
    if (!std::getline(std::cin, choice)) break;
    if (choice.empty()) continue;

    char option = choice[0]; // lấy ký tự đầu tiên

    switch (option) {
      case '1':
        port = cfg.tcp.port;
        tcp_thread = std::thread([&]() {
          start_tcp(host, port, cfg.tcp.bufferSize);
        });
        std::cout << "Press e for stop TCP service\n";
        currentServices = Services::TCP;
        break;

      case 'e':
      case 'E':
        stop_tcp();
        menuServer();
        currentServices = Services::NOT;
        break;

      case 'q':
      case 'Q':
        std::cout << "Exiting...\n";
        if (currentServices == Services::TCP) stop_tcp();
        return 0;

      default:
        std::cout << "Unknown option: " << choice << "\n";
        break;
    }
  }

  return 0;
}
