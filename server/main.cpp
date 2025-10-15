#include "service-manager.h"
#include "../config/config.h"
#include "server.hpp"
#include "utils.h"

int main(int argc, char* argv[]) {
  Config::AppConfig cfg = Config::defaultConfig();
  std::string host = cfg.tcp.hostServer;
  std::string port = cfg.tcp.port;

  if (argc >= 2) host = argv[1];

  Server server(host);

  if (!server.hasAnyUser()) {
    registerServer(server);
  }

  ServiceManager sm;
  menuServer();

  while (true) {
    std::string choice;
    if (!std::getline(std::cin, choice)) break;
    if (choice.empty()) continue;

    switch (choice[0]) {
      case '1':
        sm.startTCP(host, port, cfg.tcp.bufferSize);
        std::cout << "Press e to stop TCP service\n";
        break;

      case 'c':
      case 'C':
        registerServer(server);
        menuServer();
        break;

      case 'j':
      case 'J':
        joinServer(server);
        menuServer();
        break;

      case 'e':
      case 'E':
        sm.stopTCP();
        menuServer();
        break;

      case 'q':
      case 'Q':
        std::cout << "Exiting...\n";
        sm.cleanup();
        return 0;

      default:
        std::cout << "Unknown option: " << choice << "\n";
        std::cout << ">> ";
        break;
    }
  }

  sm.cleanup();
  return 0;
}
