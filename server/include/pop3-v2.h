#pragma once
#include "common/protocol.h"
#include "common/utils.h"
#include "server.h"

// API khởi chạy / dừng dịch vụ TCP
int runPop3V2(Server& server, const std::string& host, const std::string& port, int kBufferSize = 4096);
int stopPop3V2();

std::string popOk(std::string_view msg = {});
std::string popError(std::string_view msg = {});
std::string handleUSER(AuthSession& sess, std::string_view username);
std::string handlePASS(AuthSession& sess, Server& server, std::string_view password, socket_handle_t sock);
void serveOneClient(Server& server, Protocol& client, int kBufferSize);
std::string handleCommandLine(Server& server, Protocol& client, std::string_view line);
std::string handleListPop3V2(Server& server, socket_handle_t sock);