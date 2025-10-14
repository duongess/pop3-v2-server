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

int main(int argc, char* argv[]) {
  Config::AppConfig cfg = Config::defaultConfig();

  std::thread tcp_thread;
  std::atomic<bool> tcp_running{false};
  std::string host = cfg.tcp.hostServer, port;

  if (argc >= 2) {
    host = argv[1];
  }

  Server server = Server(host);
  std::string username, password;
  while (true) {
    if (!server.hasAnyUser()) {
      std::cout << "Register a server account" << std::endl;
      std::cout << "Server Name: "; std::getline(std::cin, username);
      std::cout << "Server Password: "; std::getline(std::cin, password);
      server.signUp(username, password);
      std::cout << ___________________ << std::endl;
    } else {
      std::cout << "1" << std::endl;
      std::cout << "Server Name: "; std::getline(std::cin, username);
      std::cout << "Server Password: "; std::getline(std::cin, password);
      server.signIn(username, password);
      break;
    }
  }

  server.checkAccout();

  auto start_tcp = [&]() {
    if (tcp_running.load()) {
      std::cout << "TCP service already running\n";
      return;
    }
    tcp_running = true;
    port = cfg.tcp.port;
    tcp_thread = std::thread([&]() {
      run_tcp(host, port);
      tcp_running = false;
    });
    std::cout << "Started TCP service on " << host << ":" << port << "\n";
  };

  auto stop_tcp_srv = [&]() {
    if (!tcp_running.load()) {
      std::cout << "TCP service is not running\n";
      return;
    }
    stop_tcp();
    if (tcp_thread.joinable()) tcp_thread.join();
    std::cout << "Stopped TCP service\n";
  };

  menuServer();
  // Main loop
  while (true) {
    std::string choice;
    if (!std::getline(std::cin, choice)) break;
    if (choice.empty()) continue;

    char option = choice[0]; // lấy ký tự đầu tiên

    switch (option) {
      case '1':
        start_tcp();
        std::cout << "Press e for stop TCP service\n";
        break;

      case 'e':
      case 'E':
        stop_tcp_srv();
        menuServer();
        break;

      case 'q':
      case 'Q':
        std::cout << "Exiting...\n";
        if (tcp_running.load()) stop_tcp_srv();
        return 0;

      default:
        std::cout << "Unknown option: " << choice << "\n";
        break;
    }
  }

  // Graceful shutdown nếu thoát vòng while
  if (tcp_running.load()) {
    stop_tcp_srv();
  }

  return 0;
}
