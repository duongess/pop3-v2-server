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
  Server(string host): host_(host) {};

  bool start(string port);
  bool hasUser(const string& username) const;
  bool hasAnyUser() const;
  void checkAccout() const;

  void shutdown();
  // auth
  AuthResult signUp(const string& username, const string& password);
  AuthResult signIn(const string& username, const string& password);
  void verify(const string& username, const string& password, const string& host, const socket_handle_t& socket_fd);

  // broadcast tiện dùng
  void broadcastFrom(Session* who, const string& text);

private:
  socket_handle_t listen_fd_ = INVALID_SOCKET;
  string host_;
  string currentName_;

  // Kết nối
  SessionManager sessionManager_;

  // Users (đơn giản cho đồ án): username -> passwordHash
  mutable mutex users_mu_;
  unordered_map<string, string> users_;

  // Tiện ích
  string hashPassword(const string& pw) const;

  // Lưu/đọc file (tuỳ chọn, có thể bỏ nếu chỉ demo in-memory)
  void loadUsersFromFile(const string& path = "users.db");
  void saveUsersToFile(const string& path = "users.db") const;

  void onAccept(socket_handle_t fd);
};
