#include "session-manager.h"

std::string SessionManager::generateToken_(size_t len) {
    static const char* chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    thread_local std::mt19937_64 rng{std::random_device{}()};
    std::uniform_int_distribution<size_t> dist(0, strlen(chars)-1);
    std::string s; s.reserve(len);
    for (size_t i=0;i<len;++i) s.push_back(chars[dist(rng)]);
    return s;
}

void SessionManager::createSessionFor(const std::string& username, const std::string& host, const socket_handle_t& socket_fd) {
    Session s;
    s.username = username;
    s.host = host;
    s.socket_fd = socket_fd;
    s.token = generateToken_();
    s.isAuthenticated = true;

    this->sessionsByToken_[s.token] = s;
}

Session SessionManager::getSessionByToken(const std::string& token) {
    return this->sessionsByToken_[token];
}

SessionManager::~SessionManager() {
    // cleanup nếu cần
    std::lock_guard<std::mutex> g(mu_);
    sessionsByToken_.clear();
}
