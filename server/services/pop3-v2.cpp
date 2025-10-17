#include "pop3-v2.h"

static atomic<bool> g_pop3v2_stop{false};
static Pop3V2<string> g_server;

int startPop3v2(const string& host, const string& port, int kBufferSize) {
  console.log("[TCP] Starting TCP server on ", host, ":", port, "...\n");

  g_pop3v2_stop = false;
  if (!g_server.bindAndListen(host, port)) {
    console.error("[TCP] Failed to start TCP server");
    return 1;
  }

  console.success("[TCP] Server started and listening on port ", port );

  // Vòng lặp chấp nhận client
  char buffer[4096];
  while (!g_pop3v2_stop.load()) {
    TCP<string> client;
    if (!g_server.acceptClient(client)) {
      if (TCP<string>::shouldStop()) break;
      continue;
    }

    console.success("[TCP] Client connected");

    Response<string> received = client.receiveData(kBufferSize);

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

int stopPop3v2() {
  console.warn("[TCP] Stopping TCP service...");
  g_pop3v2_stop = true;
  TCP<string>::requestStop();
  g_server.clean();
  console.stopping("[TCP] TCP service fully stopped\n");
  return 0;
}
