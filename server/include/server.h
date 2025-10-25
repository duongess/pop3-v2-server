#pragma once
#include <cstdio>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <memory>
#include <unordered_map>
#include <string>
#include <mutex>
#include "utils.h"
#include "session-manager.h"
#include "types/auth.h"
#include "db.h"
#include "user.h"

class Server {
public:
  Server(std::string host);

  bool start(std::string port);
  bool hasUser(const std::string& username);
  bool hasAnyUser() const;
  void checkAccout() const;
  std::vector<MailInfo>  getMails(int userId);
  void shutdown();
  // auth
  AuthResult signUp(const std::string& username, const std::string& password);
  AuthResult signIn(const std::string& username, const std::string& password);
  AuthResult login(const std::string& username, const std::string& password, const socket_handle_t& socket_fd);
  // void verify(const socket_handle_t& socket_fd);
  Session getSessionBySocket(socket_handle_t sock);
  // broadcast tiện dùng
  void broadcastFrom(Session* who, const std::string& text);
  bool initDB();

private:
  socket_handle_t listen_fd_ = invalid_socket_handle;
  std::string host_;
  std::string currentName_;

  // Kết nối
  SessionManager sessionManager_;
  DB db;

  // Users (đơn giản cho đồ án): username -> passwordHash
  mutable std::mutex users_mu_;
  std::unordered_map<std::string, std::string> users_;
  void loadUserDB();

  // Tiện ích
  std::string hashPassword(const std::string& pw) const;

  void onAccept(socket_handle_t fd);
};
