#pragma once
#include <thread>
#include <atomic>
#include <string>
#include <iostream>

#include "services/tcp.h"
#include "../types/services.h"

class ServiceManager {
  private:
    Services current = Services::NOT;
    std::thread worker;
    std::atomic<bool> running = false;
  
  public:
    // Khai báo các hàm public
    void startTCP(const std::string& host, const std::string& port, int bufferSize);
    void stopTCP();
    void cleanup();
};
