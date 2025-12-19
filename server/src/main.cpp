#include "server-cli.h"

int main(int argc, char* argv[]) {
  std::filesystem::create_directories("database");
  ServerCLI server;
  server.runRailway(argv, argc);
  return 0;
}
