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
#include "../common/utils.h"
#include "session-manager.h"
#include "../types/auth.h"

class Server {
public:
  Server(std::string host): host_(host) {};

  bool start(std::string port);
  bool hasUser(const std::string& username);
  bool hasAnyUser() const;
  void checkAccout() const;

  void shutdown();
  // auth
  AuthResult signUp(const std::string& username, const std::string& password);
  AuthResult signIn(const std::string& username, const std::string& password);
  std::string login(const std::string& username, const std::string& password, const socket_handle_t& socket_fd);
  // void verify(const socket_handle_t& socket_fd);

  // broadcast tiện dùng
  void broadcastFrom(Session* who, const std::string& text);

private:
  socket_handle_t listen_fd_ = INVALID_SOCKET;
  std::string host_;
  std::string currentName_;

  // Kết nối
  SessionManager sessionManager_;

  // Users (đơn giản cho đồ án): username -> passwordHash
  mutable std::mutex users_mu_;
  std::unordered_map<std::string, std::string> users_;

  // Tiện ích
  std::string hashPassword(const std::string& pw) const;

  // Lưu/đọc file (tuỳ chọn, có thể bỏ nếu chỉ demo in-memory)
  void loadUsersFromFile(const std::string& path = "users.db");
  void saveUsersToFile(const std::string& path = "users.db") const;

  void onAccept(socket_handle_t fd);
};
