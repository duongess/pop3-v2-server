#include "pop3-v2.h"

static std::atomic<bool> g_tcp_stop{false};
static Protocol g_server;
static std::vector<std::thread> clients;

static void serveOneClientThread(Server& server, Protocol client, int kBufferSize) {
  if (!client.isValid()) { 
    console.error("[Pop3 V2] Invalid client socket");
    return;
  }
  serveOneClient(server, client, kBufferSize);
}

static void cancelClients() {
  for (auto& cli:clients) {
    if (cli.joinable()) cli.join();
  }
  clients.clear();
}

int runPop3V2(Server& server, const std::string& host, const std::string& port, int kBufferSize) {
  console.log("[Pop3 V2] Starting Pop3 V2 server on ", host, ":", port, "...\n");

  g_tcp_stop = false;
  if (!g_server.bindAndListen(host, port)) {
    console.error("[Pop3 V2] Failed to start Pop3 V2 server");
    return 1;
  }
  console.success("[Pop3 V2] Server started and listening on port ", port);
  while (!g_tcp_stop.load()) {
    Protocol client;
    if (!g_server.acceptClient(client)) {
      if (Protocol::shouldStop()) break;
      continue;
    }
    console.success("[Pop3 V2] Client connected");
    try {
      clients.emplace_back(
        serveOneClientThread,
        std::ref(server),
        std::move(client),
        kBufferSize
      );
    } catch (const std::exception& ex) {
      console.error("[Pop3 V2] Failed to spawn client thread: ", ex.what());
    }
  }

  g_server.clean();
  cancelClients();
  console.stopping("[Pop3 V2] Server stopped listening");
  return 0;
}

int stopPop3V2() {
  console.warn("[Pop3 V2] Stopping Pop3 V2 service...");
  g_tcp_stop = true;
  g_server.disconnect();
  Protocol::requestStop();
  console.stopping("[Pop3 V2] Pop3 V2 service fully stopped\n");
  return 0;
}

// Trả về chuỗi theo kiểu POP3: +OK / -ERR <msg>\r\n
inline std::string pop_ok(std::string_view msg = {}) {
  std::string s = "+OK";
  if (!msg.empty()) { s += " "; s += msg; }
  s += "\r\n"; return s;
}
inline std::string pop_err(std::string_view msg = {}) {
  std::string s = "-ERR";
  if (!msg.empty()) { s += " "; s += msg; }
  s += "\r\n"; return s;
}

// USER <username>
std::string handleUSER(AuthSession& sess, std::string_view username) {
  if (sess.state != AuthState::AUTHORIZATION) return pop_err("already authorized");
  if (username.empty()) return pop_err("missing username");
  sess.pendingUser = std::string(username);
  return pop_ok("user accepted, send PASS");
}

// PASS <password>
std::string handlePASS(AuthSession& sess, Server& server, std::string_view password, socket_handle_t sock) {
  if (sess.state != AuthState::AUTHORIZATION) return pop_err("already authorized");
  if (sess.pendingUser.empty()) return pop_err("send USER first");
  auto result = server.login(sess.pendingUser, std::string(password), sock); // dùng hàm của bạn
  switch (result) {
    case AuthResult::Ok:
      sess.authed = true;
      sess.state = AuthState::TRANSACTION;
      return pop_ok("authentication successful");
    case AuthResult::UserNotFound:
      return pop_err("no such user");
    case AuthResult::WrongPassword:
      return pop_err("invalid password");
    default:
      return pop_err("auth error");
  }
}


void serveOneClient(Server& server, Protocol& client, int kBufferSize) {
  // banner POP3
  // client.sendData(pop_ok("connecting"));
  while(true) {
    Response received = client.receiveData(kBufferSize);
    if (received.status != Status::OK) {
      console.error("[Pop3 V2] Error: " + received.error);
    }
    std::string reply = handleCommandLine(server, client, received.data);
    if (!client.sendData(reply)) {
      console.error("[POPv2] Send failed → closing client");
      break; // THOÁT vòng
    }

    if (client.sess.state == AuthState::UPDATE) { // sau QUIT
      break;
    }
  }
  client.disconnect();
}

std::string handleCommandLine(Server& server, Protocol& client, std::string_view line) {
  auto tokens = splitWs(line);                  // bạn đã có
  if (tokens.empty()) return pop_err("empty command");
  auto cmd = tolowerCopy(tokens[0]);

  if (cmd == "user") {
    if (tokens.size() < 2) return pop_err("usage: USER <username>");
    console.debug("username: ", tokens[1]);
    return handleUSER(client.sess, tokens[1]);
  }
  if (cmd == "pass") {
    if (tokens.size() < 2) return pop_err("usage: PASS <password>");
    console.debug("password: ", tokens[1]);

    auto temp = handlePASS(client.sess, server, tokens[1], client.getSocket());
    client.sendData(handleListPop3V2(server, client.getSocket()));
    return temp;
    // return handlePASS(client.sess, server, tokens[1], client.getSocket());
  }

  // Các lệnh sau yêu cầu đã đăng nhập
  if (!client.sess.authed) return pop_err("please authenticate first");

  if (cmd == "uidl") {
    // TODO: trả multi-line kết thúc bằng ".\r\n"
    // ví dụ:
    // client.sendData(pop_ok("uidl follows"));
    // for (auto& item: list) client.sendData(item + "\r\n");
    // client.sendData(".\r\n");
    return pop_ok("uidl follows (not implemented)");
  }
  if (cmd == "list") {
    handleListPop3V2(server, client.getSocket());
    
    // return pop_ok("list follows (not implemented)");
    // Get session info
    // auto sess = server.getSessionManager().getSessionBySocket(client.getSocket());
    // int userId = sess.userId;

    // // Case 1: LIST <msgId> → show single message size
    // if (tokens.size() == 2) {
    //     int msgId = std::stoi(tokens[1]);
    //     auto infoOpt = server.getDatabase().mail.getMailInfo(userId, msgId);
    //     if (!infoOpt)
    //         return pop_err("no such message");

    //     auto info = *infoOpt;
    //     return pop_ok(std::to_string(info.mailId) + " " + std::to_string(info.size));
    // }

    // // Case 2: LIST → list all messages for user
    // auto mails = server.getDatabase().mail.listMailsForUser(userId);

    // std::string reply = pop_ok(std::to_string(mails.size()) + " messages\r\n");
    // for (auto &m : mails) {
    //     reply += std::to_string(m.mailId) + " " + std::to_string(m.size) + "\r\n";
    // }
    // reply += ".\r\n"; // POP3 multi-line terminator

    // return reply;
  }
  if (cmd == "retr") {
    if (tokens.size() < 2) return pop_err("usage: RETR <id>");
    return pop_ok("retr follows (not implemented)");
  }
  if (cmd == "dele") {
    if (tokens.size() < 2) return pop_err("usage: DELE <id>");
    return pop_ok("marked deleted (not implemented)");
  }
  if (cmd == "quit") {
    client.sess.state = AuthState::UPDATE;
    return pop_ok("bye");  // caller sẽ đóng socket
  }

  return pop_err("unknown command");
}

std::string handleListPop3V2(Server& server,socket_handle_t sock){

  Session  session = server.getSessionBySocket(sock);

  int userID = session.userId;

  std::vector<Mail> mails = server.getMails(userID);

  return pop_ok(convertToJSONFile(mails));
    // client.getSocket()
}