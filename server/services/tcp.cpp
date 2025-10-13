// Blocking echo server implementation
#include <iostream>
#include <string>
#include <cstring>
#include <atomic>
#include "../../common/utils.h"
#include "../../common/protocol.h"

static bool set_reuseaddr(socket_handle_t s) {
  int opt = 1;
#ifdef _WIN32
  return setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt)) == 0;
#else
  return setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == 0;
#endif
}

static std::atomic<bool> g_tcp_stop{false};
static socket_handle_t g_listen_socket = invalid_socket_handle;

int run_tcp(const std::string& host, const std::string& port) {
  g_tcp_stop = false;
  if (!net_init()) {
    std::cerr << "Failed to init networking" << std::endl;
    return 1;
  }

  addrinfo hints{};
  hints.ai_family = AF_UNSPEC; // IPv4 or IPv6
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  hints.ai_flags = AI_PASSIVE; // for bind

  addrinfo* result = nullptr;
  int gai = getaddrinfo(host.c_str(), port.c_str(), &hints, &result);
  if (gai != 0 || !result) {
#ifdef _WIN32
    std::cerr << "getaddrinfo failed: " << gai << std::endl;
#else
    std::cerr << "getaddrinfo failed" << std::endl;
#endif
    net_cleanup();
    return 1;
  }

  socket_handle_t listen_socket = invalid_socket_handle;
  for (addrinfo* ptr = result; ptr != nullptr; ptr = ptr->ai_next) {
    listen_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
    if (listen_socket == invalid_socket_handle) continue;
    set_reuseaddr(listen_socket);
    if (bind(listen_socket, ptr->ai_addr, (int)ptr->ai_addrlen) == 0) {
      break;
    }
    close_socket(listen_socket);
    listen_socket = invalid_socket_handle;
  }
  freeaddrinfo(result);

  if (listen_socket == invalid_socket_handle) {
    std::cerr << "Failed to create/bind listening socket" << std::endl;
    net_cleanup();
    return 1;
  }

  if (listen(listen_socket, SOMAXCONN) != 0) {
    std::cerr << "listen failed" << std::endl;
    close_socket(listen_socket);
    net_cleanup();
    return 1;
  }

  g_listen_socket = listen_socket;
  std::cout << "Server listening on port " << port << std::endl;

  char buffer[kBufferSize];
  while (!g_tcp_stop.load()) {
    socket_handle_t client = accept(listen_socket, nullptr, nullptr);
    if (client == invalid_socket_handle) {
      if (g_tcp_stop.load()) break;
      std::cerr << "accept failed" << std::endl;
      continue;
    }
    std::cout << "Client connected" << std::endl;

    char buffer[1024];
    int received = recv(client, buffer, sizeof(buffer) - 1, 0);
    if (received > 0) {
        buffer[received] = '\0'; // thêm ký tự kết thúc chuỗi
        std::string message(buffer); // chuyển char[] -> std::string
        std::cout << "Received: " << message << std::endl;
    }


    std::cout << "Client disconnected" << std::endl;
    close_socket(client);
  }

  // Unreachable in current loop, but included for completeness
  if (listen_socket != invalid_socket_handle) close_socket(listen_socket);
  g_listen_socket = invalid_socket_handle;
  net_cleanup();
  return 0;
}

int stop_tcp() {
  g_tcp_stop = true;
  // Unblock accept: shutdown + close listening socket
  if (g_listen_socket != invalid_socket_handle) {
#ifdef _WIN32
    shutdown(g_listen_socket, SD_BOTH);
#else
    shutdown(g_listen_socket, SHUT_RDWR);
#endif
    close_socket(g_listen_socket);
  }
  return 0;
}
