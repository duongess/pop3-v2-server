#include "service-manager.h"
#include "../config/config.h"
#include "server.h"
#include "utils.h"
#include "../common/console.h"
#include "storage/db.h"

void dumpTables(sqlite3* db){
  const char* q = "SELECT name FROM sqlite_master WHERE type='table' ORDER BY name;";
  sqlite3_stmt* st=nullptr;
  if (sqlite3_prepare_v2(db, q, -1, &st, nullptr)==SQLITE_OK){
    while(sqlite3_step(st)==SQLITE_ROW){
      console.log("[TABLE] ", (const char*)sqlite3_column_text(st,0));
    }
  }
  sqlite3_finalize(st);
}

int main(int argc, char* argv[]) {
  console.reset();
  Config::AppConfig cfg = Config::defaultConfig();
  std::string host = cfg.tcp.hostServer;
  std::string port = cfg.tcp.port;

  if (argc >= 2) host = argv[1];

  DB db;
  if (!db.initSchema()) { 
    console.error("DB not connected");
    return 0;
  }
  dumpTables(db.conn.get());

  Server server(host);

  ServiceManager sm;
  sm.setServer(server);
  menuServer();

  while (true) {
    std::string choice;
    if (!std::getline(std::cin, choice)) break;
    if (choice.empty()) continue;

    switch (choice[0]) {
      case '1':
        sm.startTCP(host, port, cfg.tcp.bufferSize);
        console.log("Press e to stop TCP service");
        break;
      
      case '2':
        if (!server.hasAnyUser()) {
          registerServer(server);
        }
        port = cfg.pop3V2.port;
        sm.startPop3V2(host, port, cfg.pop3V2.bufferSize);
        menuServer();
        break;

      case 'c':
      case 'C':
        registerServer(server);
        menuServer();
        sm.resumeIfSelected();
        break;

      case 'j':
      case 'J':
        joinServer(server);
        menuServer();
        break;

      case 'e':
      case 'E':
        sm.endTCP();
        sm.endPop3V2();
        menuServer();
        break;

      case 'q':
      case 'Q':
        console.log("Exiting...");
        sm.cleanup();
        return 0;

      default:
        console.log("Unknown option: ", choice);
        console.log(">> ");
        break;
    }
  }

  sm.cleanup();
  console.reset();
  return 0;
}
