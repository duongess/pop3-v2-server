#include "service-manager.h"

void ServiceManager::startTCP(const std::string& host, const std::string& port, int bufferSize) {
    if (running) {
        std::cout << "[WARN] TCP service already running.\n";
        return;
    }

    running = true;
    current = Services::TCP;

    worker = std::thread([&, host, port, bufferSize]() {
        start_tcp(host, port, bufferSize);  // <-- Gọi sang tcp.cpp
        running = false;
        current = Services::NOT;
    });

    std::cout << "[INFO] TCP service started.\n";
}

void ServiceManager::stopTCP() {
    if (!running) {
        std::cout << "[WARN] TCP service is not running.\n";
        return;
    }

    std::cout << "[INFO] Stopping TCP service...\n";
    stop_tcp();

    if (worker.joinable())
        worker.join();

    running = false;
    current = Services::NOT;
    std::cout << "[INFO] TCP service stopped.\n";
}

void ServiceManager::cleanup() {
    if (running && worker.joinable()) {
        stop_tcp();
        worker.join();
    }
}
