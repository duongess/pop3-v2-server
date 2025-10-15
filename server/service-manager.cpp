#include "service-manager.h"

void ServiceManager::startTCP(const std::string& host, const std::string& port, int bufferSize) {
    if (running) {
        console.warn("service already running.");
        return;
    }

    running = true;
    current = Services::TCP;

    worker = std::thread([&, host, port, bufferSize]() {
        start_tcp(host, port, bufferSize);  // <-- Gọi sang tcp.cpp
        running = false;
        current = Services::NOT;
    });

    console.stopping("TCP service started.");
}

void ServiceManager::stopTCP() {
    if (!running) {
        console.warn("TCP service is not running.");
        return;
    }

    console.stopping("Stopping TCP service...");
    stop_tcp();

    if (worker.joinable())
        worker.join();

    running = false;
    current = Services::NOT;
    console.stopping("TCP service stopped.");
}

void ServiceManager::cleanup() {
    if (running && worker.joinable()) {
        stop_tcp();
        worker.join();
    }
}
