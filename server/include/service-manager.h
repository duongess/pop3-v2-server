#pragma once
#include <thread>
#include <atomic>
#include <string>
#include <iostream>

#include "index.h"
#include "types/services.h"
#include "server.h"

class ServiceManager {
  private:
    Services current = Services::NOT;
    std::thread worker;
    std::atomic<bool> running = false;
    Server* server = nullptr;
  
  public:
    void setServer(Server& server) {
      this->server = &server;
    }
    // Khai báo các hàm public
    void startTCP(const std::string& host, const std::string& port, int bufferSize);
    void endTCP();

    void startPop3V2(const std::string& host, const std::string& port, int bufferSize);
    void endPop3V2();
    void cleanup();

    void resumeIfSelected();
};
