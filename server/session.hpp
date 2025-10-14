#pragma once
#include <string>
#include <memory>
#include "../common/utils.h"
#include "server.hpp"

class Server;
class Session : public std::enable_shared_from_this<Session> {
public:
  Session(uint64_t id, socket_handle_t fd, Server& server)
    : id_(id), fd_(fd), server_(server) {}

  uint64_t id() const { return id_; }
  socket_handle_t fd() const { return fd_; }

  bool handleReadable();
  bool sendText(const std::string& s);

  // tuỳ chọn: nickname/room/lastSeen...
  void setNick(std::string n){ nick_ = std::move(n); }
  const std::string& nick() const { return nick_; }

private:
  uint64_t id_;
  socket_handle_t fd_;
  Server&  server_;
  std::string nick_ = "anon";
};
