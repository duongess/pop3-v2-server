// Blocking echo server implementation
#include <iostream>
#include <string>
#include <cstring>
#include "server.h"
#include "../common/utils.h"
#include "../common/protocol.h"

static bool set_reuseaddr(socket_handle_t s) {
  int opt = 1;
#ifdef _WIN32
  return setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt)) == 0;
#else
  return setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == 0;
#endif
}

int run_server(const char* host, const char* port) {
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
  int gai = getaddrinfo(host, port, &hints, &result);
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

  std::cout << "Server listening on port " << port << std::endl;

  char buffer[kBufferSize];
  while (true) {
    socket_handle_t client = accept(listen_socket, nullptr, nullptr);
    if (client == invalid_socket_handle) {
      std::cerr << "accept failed" << std::endl;
      continue;
    }
    std::cout << "Client connected" << std::endl;

    while (true) {
      int received = recv(client, buffer, (int)sizeof(buffer), 0);
      if (received <= 0) {
        break; // disconnect or error
      }
      int total_sent = 0;
      while (total_sent < received) {
        int sent = send(client, buffer + total_sent, received - total_sent, 0);
        if (sent <= 0) { break; }
        total_sent += sent;
      }
    }

    std::cout << "Client disconnected" << std::endl;
    close_socket(client);
  }

  // Unreachable in current loop, but included for completeness
  close_socket(listen_socket);
  net_cleanup();
  return 0;
}
