// Interactive client with menu/cases
#include "client.h"
#include "../config/config.h"

int main(int argc, char* argv[]) {
  Config::AppConfig cfg = Config::defaultConfig();
  std::string host = cfg.tcp.hostClient, port = cfg.tcp.port;

  if (argc >= 2) {
    host = argv[1];
  }

  Client client(host, port);
  client.request("auhdwddwd");

  return 1;
  // while (true) {
  //   std::cout << "\n=== Client Menu ===\n";
  //   std::cout << "1) TCP send\n";
  //   std::cout << "q) Quit\n> ";

  //   std::string choice;
  //   if (!std::getline(std::cin, choice)) break;

  //   switch (choice.empty() ? '\0' : choice[0]) {
  //     case '1': {
  //       std::string messages;
  //       port = cfg.tcp.port;
  //       std::cout << "Send to server " << host << ":" << port << std::endl;
  //       std::cout << "messages: "; std::getline(std::cin, messages);
  //       int rc = sendMessage(host, port, messages);
  //       if (rc != 0) std::cerr << "TCP echo failed" << std::endl;
  //       break;
  //     }
  //     case 'q':
  //     case 'Q':
  //       return 0;
  //     default:
  //       if (!choice.empty()) std::cout << "Unknown option: " << choice << std::endl;
  //       break;
  //   }
  // }

  return 0;
}
