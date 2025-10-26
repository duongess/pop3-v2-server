#include "service-manager.h"
#include "config/config.h"
#include "server.h"
#include "utils.h"
#include "common/console.h"

static std::atomic<bool> g_quitting{false};
static void onSignal(int) { g_quitting.store(true); }
static inline bool stdinIsInteractive() {
  return isatty(fileno(stdin)) == 1;
}

int main(int argc, char* argv[]) {
  // console + signal
  console.reset();
  std::signal(SIGINT,  onSignal);
  std::signal(SIGTERM, onSignal);

  // Load config
  auto cfg  = Config::defaultConfig();
  std::string host = cfg.tcp.hostServer;      // hoặc cfg.pop3V2.host nếu có
  if (argc >= 2) host = argv[1];
  std::string port = cfg.pop3V2.port;

  // Core services
  Server server(host);
  ServiceManager sm;
  sm.setServer(server);

  // Đảm bảo đã có account server (idempotent)
  if (!server.hasAnyUser()) {
    registerServer(server);     // đừng throw nếu đã tồn tại
  }

  const bool interactive = stdinIsInteractive();

  // Khởi động POP3 v2 đúng 1 lần
  sm.startPop3V2(host, port, cfg.pop3V2.bufferSize);

  if (!interactive) {
    console.log("POP3 v2 running (non-interactive). Send SIGTERM/SIGINT to stop.");
    // Chờ tín hiệu dừng (Docker `stop` sẽ gửi SIGTERM)
    while (!g_quitting.load()) {
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    sm.endPop3V2();
    sm.cleanup();
    console.reset();
    return 0;
  }

  // === Interactive mode (local terminal) ===
  menuServer(host, port);

  std::string line;
  while (std::getline(std::cin, line)) {
    if (line.empty()) { console.log(">> "); continue; }

    const char cmd = line[0];
    switch (cmd) {
      case 'q': case 'Q': {
        console.log("Exiting...");
        sm.endPop3V2();
        sm.cleanup();
        console.reset();
        return 0;
      }

      case 'c': case 'C': {
        registerServer(server);           // idempotent
        sm.resumeIfSelected();            // nếu bạn có cơ chế resume
        menuServer(host, port);
        break;
      }

      case 'j': case 'J': {
        joinServer(server);
        menuServer(host, port);
        break;
      }

      case 'e': case 'E': {
        sm.endPop3V2();
        console.log("POP3 v2 stopped.");
        menuServer(host, port);
        break;
      }

      case 'r': case 'R': {
        sm.endPop3V2();
        sm.startPop3V2(host, port, cfg.pop3V2.bufferSize);
        console.log("POP3 v2 restarted.");
        menuServer(host, port);
        break;
      }

      default: {
        console.log("Unknown command: ", line);
        console.log(">> ");
        break;
      }
    }
  }

  // EOF trên stdin (đóng terminal) → dọn dẹp
  sm.endPop3V2();
  sm.cleanup();
  console.reset();
  return 0;
}
