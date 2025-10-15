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

void Server::loadUsersFromFile(const std::string& path){
  std::lock_guard<std::mutex> lk(users_mu_);
  users_.clear();
  std::ifstream f(path);
  if (!f) return; // lần đầu chưa có file -> để trống (coi như “no users yet”)
  std::string line;
  while (std::getline(f, line)) {
    line = trim(line);
    if (line.empty() || line[0]=='#') continue;
    // format: username:hash
    auto p = line.find(':');
    if (p==std::string::npos) continue;
    std::string u = line.substr(0,p);
    std::string h = line.substr(p+1);
    users_[u] = h;
  }
}

void Server::saveUsersToFile(const std::string& path) const {
  std::lock_guard<std::mutex> lk(users_mu_);
  std::ofstream f(path, std::ios::trunc);
  for (auto& kv : users_) {
    f << kv.first << ":" << kv.second << "\n";
  }
}

AuthResult Server::signUp(const std::string& username, const std::string& password){
  std::lock_guard<std::mutex> lk(users_mu_);
  if (users_.count(username)) return AuthResult::AlreadyExists;
  users_[username] = hashPassword(password);
  // saveUsersToFile(); // lưu lại để lần sau còn dùng
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

void Server::verify(const std::string& username, const std::string& password, const std::string& host, const socket_handle_t& socket_fd) {
  if (this->hasUser(username)) {
    std::cout << "username invaible\n";
    return;
  } else if (this->users_[username] == password) {
    std::cout << "password flase\n";
    return;
  }
  this->sessionManager_.createSessionFor(username, host, socket_fd);
}

// ... phần start(), onAccept(), broadcastFrom(...) của bạn giữ nguyên,
// chỉ cần gọi loadUsersFromFile() một lần khi khởi động.
bool Server::start(std::string port){
  // khởi tạo socket … (như bạn đã làm)
  loadUsersFromFile(); // <- thêm dòng này
  // vòng select/accept … (giữ nguyên)
  return true;
}

bool Server::hasUser(const std::string& username) const {
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
