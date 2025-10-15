#include "tcp.h"

static std::atomic<bool> g_tcp_stop{false};
static TCP<std::string> g_server;

int start_tcp(const std::string& host, const std::string& port, int kBufferSize) {
  console.log("[TCP] Starting TCP server on ", host, ":", port, "...\n");

  g_tcp_stop = false;
  if (!g_server.bindAndListen(host, port)) {
    console.error("[TCP] Failed to start TCP server");
    return 1;
  }

  console.success("[TCP] Server started and listening on port ", port );

  // Vòng lặp chấp nhận client
  char buffer[4096];
  while (!g_tcp_stop.load()) {
    TCP<std::string> client;
    if (!g_server.acceptClient(client)) {
      if (TCP<std::string>::shouldStop()) break;
      continue;
    }

    console.success("[TCP] Client connected");

    Response<std::string> received = client.receiveData(kBufferSize);

    if (received.status == Status::OK) {
        console.running("[TCP] Received: ", received.data );
        client.sendData("[TCP] Server OK");
    } else {
        console.error("[TCP] Error: ", received.error );
    }

    client.close();
  }

  g_server.clean();
  console.stopping("[TCP] Server stopped listening");
  return 0;
}

int stop_tcp() {
  console.warn("[TCP] Stopping TCP service...");
  g_tcp_stop = true;
  TCP<std::string>::requestStop();
  g_server.clean();
  console.stopping("[TCP] TCP service fully stopped\n");
  return 0;
}
