#include "server.h"

static std::string trim(const std::string& s){
  auto a = s.find_first_not_of(" \t\r\n");
  auto b = s.find_last_not_of(" \t\r\n");
  if (a==std::string::npos) return "";
  return s.substr(a, b-a+1);
}

// “Hash” đơn giản cho demo
std::string Server::hashPassword(const std::string& pw) const {
  // salt cố định + std::hash
  static const std::string SALT = "sysnet-demo-salt";
  size_t h = std::hash<std::string>{}(SALT + pw);
  return std::to_string(h);
}

Server::Server(std::string host) {
  this->host_ = host;
  if (initDB()) {
    this->users_ = db.user.getAllUser();
  }
}

bool Server::initDB() {
  if (!db.initSchema()) { 
    console.error("DB not connected");
    return false;
  }
  db.dumpTables();
  return true;
}

AuthResult Server::signUp(const std::string& username, const std::string& password){
  std::lock_guard<std::mutex> lk(users_mu_);
  if (users_.count(username)) return AuthResult::AlreadyExists;
  users_[username] = hashPassword(password);
  db.user.createUser(username, users_[username]);
  return AuthResult::Ok;
}

AuthResult Server::signIn(const std::string& username, const std::string& password) {
  std::lock_guard<std::mutex> lk(users_mu_);
  auto it = users_.find(username);
  if (it == users_.end()) return AuthResult::UserNotFound;
  if (it->second != hashPassword(password)) return AuthResult::WrongPassword;
  this->currentName_ = username;
  return AuthResult::Ok;
}

std::string Server::login(const std::string& username, const std::string& password, const socket_handle_t& socket_fd) {
  if (this->signIn(username, password) == AuthResult::UserNotFound) {
    std::cout << "username invaible\n";
    return "";
  } else if (this->users_[username] == password) {
    std::cout << "password flase\n";
    return "";
  }
  return this->sessionManager_.createSessionFor(username, socket_fd); 
}

bool Server::hasUser(const std::string& username) {
  std::lock_guard<std::mutex> lk(users_mu_);
  return users_.find(username) != users_.end();
}

bool Server::hasAnyUser() const {
  std::lock_guard<std::mutex> lk(users_mu_);
  return !users_.empty();
}

void Server::checkAccout() const {
  std::cout << "Join to server " << this->currentName_ << std::endl;
}
