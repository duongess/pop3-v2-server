// Entry point for server
#include <iostream>
#include <string>
#include <thread>
#include <atomic>
#include "services/tcp.h"
#include "../config/config.h"

int main(int /*argc*/, char* /*argv*/[]) {
  Config::AppConfig cfg = Config::defaultConfig();

  std::thread tcp_thread;
  std::atomic<bool> tcp_running{false};

  auto start_tcp = [&]() {
    if (tcp_running.load()) {
      std::cout << "⚠️  TCP service already running\n";
      return;
    }
    tcp_running = true;
    tcp_thread = std::thread([&]() {
      std::string port_str = std::to_string(cfg.tcp.port);
      (void)run_tcp(cfg.tcp.host.c_str(), port_str.c_str());
      tcp_running = false;
    });
    std::cout << "✅ Started TCP service on " << cfg.tcp.host << ":" << cfg.tcp.port << "\n";
  };

  auto stop_tcp_srv = [&]() {
    if (!tcp_running.load()) {
      std::cout << "⚠️  TCP service is not running\n";
      return;
    }
    stop_tcp();
    if (tcp_thread.joinable()) tcp_thread.join();
    std::cout << "🛑 Stopped TCP service\n";
  };

  // Main loop
  while (true) {
    std::cout << "\n=== Network System Menu ===\n";
    std::cout << "1) Start TCP service\n";
    std::cout << "2) Stop TCP service\n";
    std::cout << "q) Quit\n> ";

    std::string choice;
    if (!std::getline(std::cin, choice)) break;
    if (choice.empty()) continue;

    char option = choice[0]; // lấy ký tự đầu tiên

    switch (option) {
      case '1':
        start_tcp();
        break;

      case '2':
        stop_tcp_srv();
        break;

      case 'q':
      case 'Q':
        std::cout << "Exiting...\n";
        if (tcp_running.load()) stop_tcp_srv();
        return 0;

      default:
        std::cout << "❓ Unknown option: " << choice << "\n";
        break;
    }
  }

  // Graceful shutdown nếu thoát vòng while
  if (tcp_running.load()) {
    stop_tcp_srv();
  }

  return 0;
}
