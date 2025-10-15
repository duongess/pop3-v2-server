#include "tcp.h"

static std::atomic<bool> g_tcp_stop{false};
static TCP<std::string> g_server;

int start_tcp(const std::string& host, const std::string& port, int kBufferSize) {
  std::cout << "[TCP] Starting TCP server on " << host << ":" << port << "...\n";

  g_tcp_stop = false;
  if (!g_server.bindAndListen(host, port)) {
    std::cerr << "[TCP] Failed to start TCP server\n";
    return 1;
  }

  std::cout << "[TCP] Server started and listening on port " << port << "\n";

  // Vòng lặp chấp nhận client
  char buffer[4096];
  while (!g_tcp_stop.load()) {
    TCP<std::string> client;
    if (!g_server.acceptClient(client)) {
      if (TCP<std::string>::shouldStop()) break;
      continue;
    }

    std::cout << "[TCP] Client connected\n";

    Response<std::string> received = g_server.receiveData(kBufferSize);

    if (received.status == Status::OK) {
        std::cout << "[TCP] Received: " << received.data << "\n";
        g_server.sendData("[TCP] Server OK");
    } else {
        std::cerr << "[TCP] Error: " << received.error << "\n";
    }

    client.close();
  }

  g_server.clean();
  std::cout << "[TCP] Server stopped listening\n";
  return 0;
}

int stop_tcp() {
  std::cout << "[TCP] Stopping TCP service...\n";
  g_tcp_stop = true;
  TCP<std::string>::requestStop();
  g_server.clean();
  std::cout << "[TCP] TCP service fully stopped\n";
  return 0;
}
