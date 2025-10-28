#include "common/utils.h"
#include "server.h"
#include "types/auth.h"
#include "types/db.h"

class Server;

void menuServer(const std::string& host, const std::string& port);
void registerServer(Server &server);
void joinServer(Server &server);
std:: string convertToJSONFile(std::vector<Mail> mails);
std::string escapeJSON(const std::string& input);