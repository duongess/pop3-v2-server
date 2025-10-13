// Interactive client with menu/cases
#include <iostream>
#include <string>
#include <sstream>
#include "services/tcp.h"
#include "../config/config.h"
#include "../common/protocol.h"

int main(int argc, char* argv[]) {
  Config::AppConfig cfg = Config::defaultConfig();
  std::string host = cfg.tcp.hostClient, port;

  if (argc >= 2) {
    host = argv[1];
  }

  while (true) {
    std::cout << "\n=== Client Menu ===\n";
    std::cout << "1) TCP send\n";
    std::cout << "q) Quit\n> ";

    std::string choice;
    if (!std::getline(std::cin, choice)) break;

    switch (choice.empty() ? '\0' : choice[0]) {
      case '1': {
        std::string messages;
        port = cfg.tcp.port;
        std::cout << "Send to server " << host << ":" << port << std::endl;
        std::cout << "messages: "; std::getline(std::cin, messages);
        int rc = run_tcp_client(host, port, messages);
        if (rc != 0) std::cerr << "TCP echo failed" << std::endl;
        break;
      }
      case 'q':
      case 'Q':
        return 0;
      default:
        if (!choice.empty()) std::cout << "Unknown option: " << choice << std::endl;
        break;
    }
  }

  return 0;
}
