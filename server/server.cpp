#include "server.h"

static string trim(const string& s){
  auto a = s.find_first_not_of(" \t\r\n");
  auto b = s.find_last_not_of(" \t\r\n");
  if (a==string::npos) return "";
  return s.substr(a, b-a+1);
}

// “Hash” đơn giản cho demo
string Server::hashPassword(const string& pw) const {
  // salt cố định + hash
  static const string SALT = "sysnet-demo-salt";
  size_t h = hash<string>{}(SALT + pw);
  return to_string(h);
}

void Server::loadUsersFromFile(const string& path){
  lock_guard<mutex> lk(users_mu_);
  users_.clear();
  ifstream f(path);
  if (!f) return; // lần đầu chưa có file -> để trống (coi như “no users yet”)
  string line;
  while (getline(f, line)) {
    line = trim(line);
    if (line.empty() || line[0]=='#') continue;
    // format: username:hash
    auto p = line.find(':');
    if (p==string::npos) continue;
    string u = line.substr(0,p);
    string h = line.substr(p+1);
    users_[u] = h;
  }
}

void Server::saveUsersToFile(const string& path) const {
  lock_guard<mutex> lk(users_mu_);
  ofstream f(path, ios::trunc);
  for (auto& kv : users_) {
    f << kv.first << ":" << kv.second << "\n";
  }
}

AuthResult Server::signUp(const string& username, const string& password){
  lock_guard<mutex> lk(users_mu_);
  if (users_.count(username)) return AuthResult::AlreadyExists;
  users_[username] = hashPassword(password);
  // saveUsersToFile(); // lưu lại để lần sau còn dùng
  return AuthResult::Ok;
}

AuthResult Server::signIn(const string& username, const string& password) {
  lock_guard<mutex> lk(users_mu_);
  auto it = users_.find(username);
  if (it == users_.end()) return AuthResult::UserNotFound;
  if (it->second != hashPassword(password)) return AuthResult::WrongPassword;
  this->currentName_ = username;
  return AuthResult::Ok;
}

void Server::verify(const string& username, const string& password, const string& host, const socket_handle_t& socket_fd) {
  if (this->hasUser(username)) {
    cout << "username invaible\n";
    return;
  } else if (this->users_[username] == password) {
    cout << "password flase\n";
    return;
  }
  this->sessionManager_.createSessionFor(username, host, socket_fd);
}

// ... phần start(), onAccept(), broadcastFrom(...) của bạn giữ nguyên,
// chỉ cần gọi loadUsersFromFile() một lần khi khởi động.
bool Server::start(string port){
  // khởi tạo socket … (như bạn đã làm)
  loadUsersFromFile(); // <- thêm dòng này
  // vòng select/accept … (giữ nguyên)
  return true;
}

bool Server::hasUser(const string& username) const {
  lock_guard<mutex> lk(users_mu_);
  return users_.find(username) != users_.end();
}

bool Server::hasAnyUser() const {
  lock_guard<mutex> lk(users_mu_);
  return !users_.empty();
}

void Server::checkAccout() const {
  cout << "Join to server " << this->currentName_ << endl;
}
