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
  return pw;
}

Server::Server(std::string host) {
  this->host_ = host;
  if (initDB()) {
    this->users_ = db.user.getAllUser();
    db.mail.addDemoMailsForUser(1);
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

AuthResult Server::login(const std::string& username, const std::string& password, const socket_handle_t& socket_fd) {
  AuthResult res = this->signIn(username, password);
  int userId = this->db.user.findUserId(username, this->users_[username]);
  console.info("User ", username, " (ID=", userId, ") logged in with socket ", socket_fd);
  this->sessionManager_.createSessionFor(userId, socket_fd);
  return res; 
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

Session Server:: getSessionBySocket(socket_handle_t sock){
  return sessionManager_.getSessionBySocket(sock );
}

std::vector<MailInfo>  Server::getMails(int userId){
  return db.mail.listMailsForUser(userId);
}
