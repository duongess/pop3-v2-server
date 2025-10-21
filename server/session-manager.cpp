#include "session-manager.h"

std::string SessionManager::generateToken_(size_t len) {
    static const char* chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    thread_local std::mt19937_64 rng{std::random_device{}()};
    std::uniform_int_distribution<size_t> dist(0, strlen(chars)-1);
    std::string s; s.reserve(len);
    for (size_t i=0;i<len;++i) s.push_back(chars[dist(rng)]);
    return s;
}

std::string SessionManager::createSessionFor(const int& userId, const socket_handle_t& socket_fd) {
    Session s;
    s.userId = userId;
    s.token = generateToken_();
    s.isAuthenticated = true;

    this->sessionsByToken_[s.token] = s;
    this->sessionsBySocket_[socket_fd] = s;
    return s.token;
}

Session SessionManager::getSessionByToken(const std::string& token) {
    return this->sessionsByToken_[token];
}

Session SessionManager::getSessionBySocket(const socket_handle_t& socket_fd) {
    return this->sessionsBySocket_[socket_fd];
}

SessionManager::~SessionManager() {
    // cleanup nếu cần
    std::lock_guard<std::mutex> g(mu_);
    sessionsByToken_.clear();
    sessionsBySocket_.clear();
}
