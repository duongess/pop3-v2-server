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
    void createSessionFor(const string& username, const string& host, const socket_handle_t& socket_fd);

    // Tìm session từ token
    Session getSessionByToken(const string& token);

    // Update lastActive (touch)
    void touchSession(const string& token);

    // Xoá session
    void removeSession(const string& token);
    void removeSessionBySocket(int socket_fd);

private:
    string generateToken_(size_t len = 32);

    mutable mutex mu_;
    unordered_map<string, Session> sessionsByToken_; // token -> Session
};
