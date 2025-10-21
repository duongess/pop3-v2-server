// session-manager.h
#pragma once
#include "../types/session.h"
#include "../common/utils.h"
#include <unordered_map>
#include <mutex>
#include <random>
#include <algorithm>

class SessionManager {
public:
    SessionManager() = default;
    ~SessionManager();

    // Trả về token nếu tạo thành công
    std::string createSessionFor(const std::string& userId, const socket_handle_t& socket_fd);

    // Tìm session từ token
    Session getSessionByToken(const std::string& token);
    Session getSessionBySocket(const socket_handle_t& socket_fd);

    // Update lastActive (touch)
    void touchSession(const std::string& token);

    // Xoá session
    void removeSession(const std::string& token);
    void removeSessionBySocket(int socket_fd);

private:
    std::string generateToken_(size_t len = 32);

    mutable std::mutex mu_;
    std::unordered_map<std::string, Session> sessionsByToken_; // token -> Session
    std::unordered_map<socket_handle_t, Session> sessionsBySocket_; // socket_fd -> Session
};
