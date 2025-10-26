#include "common/utils.h"
#include "server.h"
#include "types/auth.h"

class Server;

void menuServer(const std::string& host, const std::string& port);
void registerServer(Server &server);
void joinServer(Server &server);
