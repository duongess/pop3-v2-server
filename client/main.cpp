// Interactive client with menu/cases
#include <iostream>
#include <string>
#include <sstream>
#include "services/tcp.h"
#include "../config/config.h"

int main(int argc, char* argv[]) {
  Config::AppConfig cfg = Config::defaultConfig();
  string host = cfg.tcp.hostClient, port;

  if (argc >= 2) {
    host = argv[1];
  }

  while (true) {
    cout << "\n=== Client Menu ===\n";
    cout << "1) TCP send\n";
    cout << "q) Quit\n> ";

    string choice;
    if (!getline(cin, choice)) break;

    switch (choice.empty() ? '\0' : choice[0]) {
      case '1': {
        string messages;
        port = cfg.tcp.port;
        cout << "Send to server " << host << ":" << port << endl;
        cout << "messages: "; getline(cin, messages);
        int rc = sendMessage(host, port, messages);
        if (rc != 0) cerr << "TCP echo failed" << endl;
        break;
      }
      case 'q':
      case 'Q':
        return 0;
      default:
        if (!choice.empty()) cout << "Unknown option: " << choice << endl;
        break;
    }
  }

  return 0;
}
