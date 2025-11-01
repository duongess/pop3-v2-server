#include "server-cli.h"

int main(int argc, char* argv[]) {
  ServerCLI server;
  server.runRailway(argv, argc);
  return 0;
}
