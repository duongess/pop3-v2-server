#pragma once
#include <thread>
#include <atomic>
#include "services/tcp.h"
#include "../types/services.h"


struct ServiceManager {
  Services current = Services::NOT;
  std::thread worker;
  std::atomic<bool> running = false;

  // Bắt đầu TCP service
  void startTCP(const std::string& host, const std::string& port, int bufferSize) {
    if (running) {
      std::cout << "[WARN] TCP service already running.\n";
      return;
    }
    running = true;
    current = Services::TCP;
    worker = std::thread([&, host, port, bufferSize]() {
      start_tcp(host, port, bufferSize);
      running = false;
      current = Services::NOT;
    });
    std::cout << "[INFO] TCP service started.\n";
  }

  // Dừng TCP service
  void stopTCP() {
    if (!running) {
      std::cout << "[WARN] TCP service is not running.\n";
      return;
    }
    std::cout << "[INFO] Stopping TCP service...\n";
    stop_tcp();
    if (worker.joinable()) worker.join();
    running = false;
    current = Services::NOT;
    std::cout << "[INFO] TCP service stopped.\n";
  }

  // Cleanup an toàn khi thoát
  void cleanup() {
    if (running && worker.joinable()) {
      stop_tcp();
      worker.join();
    }
  }
};
