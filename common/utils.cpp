// Networking utilities implementation
#include "utils.h"

bool net_init() {
#ifdef _WIN32
  WSADATA wsaData;
  int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
  return iResult == 0;
#else
  return true;
#endif
}

void net_cleanup() {
#ifdef _WIN32
  WSACleanup();
#else
  // nothing needed
#endif
}

void close_socket(socket_handle_t s) {
#ifdef _WIN32
  if (s != INVALID_SOCKET) closesocket(s);
#else
  if (s >= 0) close(s);
#endif
}

addrinfo* resolveAddress(const std::string& host, const std::string& port, bool passive) {
    addrinfo hints{};
    hints.ai_family = AF_INET; // IPv4 only; AF_UNSPEC nếu cần IPv6
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    if (passive)
        hints.ai_flags = AI_PASSIVE; // for bind()

    addrinfo* result = nullptr;
    int gai = getaddrinfo(host.c_str(), port.c_str(), &hints, &result);
    if (gai != 0 || !result) {
        std::cerr << "[TCP] getaddrinfo failed: " << gai << "\n";
        // ❌ Đừng cleanup ở đây — giữ Winsock đang mở
        return nullptr;
    }
    return result;
}
