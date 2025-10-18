#include "service-manager.h"

void ServiceManager::startTCP(const std::string& host, const std::string& port, int bufferSize) {
    if (running) {
        console.warn("service already running.");
        return;
    }

    running = true;
    current = Services::TCP;

    worker = std::thread([&, host, port, bufferSize]() {
        runTCP(host, port, bufferSize);  // <-- Gọi sang tcp.cpp
        running = false;
        current = Services::NOT;
    });

    console.stopping("TCP service started.");
}

void ServiceManager::endTCP() {
    if (!running) {
        console.warn("TCP service is not running.");
        return;
    }

    console.stopping("Stopping TCP service...");
    stopTCP();

    if (worker.joinable())
        worker.join();

    running = false;
    current = Services::NOT;
    console.stopping("TCP service stopped.");
}

void ServiceManager::startPop3V2(const std::string& host, const std::string& port, int bufferSize) {
    if (!running) {
        console.warn("service already running.");
    }

    running = true;
    current = Services::POP3V2;
    worker = std::thread([&, host, port, bufferSize]() {
        runPop3V2(host, port, bufferSize);
        running = false;
        current = Services::NOT;
    })
    console.stopping("Pop3 V2 service started.");
}

void ServiceManager::endPop3V2() {
    if (!running) {
        console.warn("Pop3 V2 service is not running.");
        return;
    }

    console.stopping("Stopping Pop3 V2 service...");
    stopPop3V2();

    if (worker.joinable())
        worker.join();

    running = false;
    current = Services::NOT;
    console.stopping("Pop3 V2 service stopped.");
}

void ServiceManager::cleanup() {
    if (running && worker.joinable()) {
        endTCP();
        worker.join();
    }
}
