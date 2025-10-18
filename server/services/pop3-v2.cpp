#include "pop3-v2.h"

static std::atomic<bool> g_tcp_stop{false};
static Protocol g_server;
std::string user, pass;

int runPop3V2(Server& server, const std::string& host, const std::string& port, int kBufferSize) {
  console.log("[Pop3 V2] Starting Pop3 V2 server on ", host, ":", port, "...\n");

  g_tcp_stop = false;
  if (!g_server.bindAndListen(host, port)) {
    console.error("[Pop3 V2] Failed to start Pop3 V2 server");
    return 1;
  }

  console.success("[Pop3 V2] Server started and listening on port ", port );

  // Vòng lặp chấp nhận client
  char buffer[4096];
  while (!g_tcp_stop.load()) {
    Protocol client;
    if (!g_server.acceptClient(client)) {
      if (Protocol::shouldStop()) break;
      continue;
    }

    console.success("[Pop3 V2] Client connected");

    Response received = client.receiveData(kBufferSize);

    if (received.status == Status::OK) {
        console.running("[Pop3 V2] Received: ", received.data );
        auto t = splitWs(received.data);
        std::string function = tolowerCopy(t[0]);
        console.warn(function);
        if (function == "user") {
          user = tolowerCopy(t[1]);
        }
        if (function == "pass") {
          pass = tolowerCopy(t[1]);
          console.warn(user, pass);
          client.sendData(server.login(user, pass, client.getSocket()));
        }        
    } else {
        console.error("[Pop3 V2] Error: ", received.error );
    }

    client.close();
  }

  g_server.clean();
  console.stopping("[Pop3 V2] Server stopped listening");
  return 0;
}

int stopPop3V2() {
  console.warn("[Pop3 V2] Stopping Pop3 V2 service...");
  g_tcp_stop = true;
  Protocol::requestStop();
  g_server.clean();
  console.stopping("[Pop3 V2] Pop3 V2 service fully stopped\n");
  return 0;
}
