#include "tcp.h"

static bool set_reuseaddr(socket_handle_t s) {
  int opt = 1;
  return setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt)) == 0;
}

static std::atomic<bool> g_tcp_stop{false};
static socket_handle_t g_listen_socket = invalid_socket_handle;

int start_tcp(const std::string& host, const std::string& port, const int& kBufferSize = 4096) {
  std::cout << "[TCP] Starting TCP server on " << host << ":" << port << "...\n";

  g_tcp_stop = false;
  if (!net_init()) {
    std::cerr << "[TCP] Failed to init networking\n";
    return 1;
  }

  addrinfo* result = resolveAddress(host, port, true);
  socket_handle_t listen_socket = invalid_socket_handle;

  for (addrinfo* ptr = result; ptr != nullptr; ptr = ptr->ai_next) {
    listen_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
    if (listen_socket == invalid_socket_handle) continue;
    set_reuseaddr(listen_socket);
    if (bind(listen_socket, ptr->ai_addr, (int)ptr->ai_addrlen) == 0) break;
    close_socket(listen_socket);
    listen_socket = invalid_socket_handle;
  }
  freeaddrinfo(result);

  if (listen_socket == invalid_socket_handle) {
    std::cerr << "[TCP] Failed to create/bind listening socket\n";
    net_cleanup();
    return 1;
  }

  if (listen(listen_socket, SOMAXCONN) != 0) {
    std::cerr << "[TCP] listen failed\n";
    close_socket(listen_socket);
    net_cleanup();
    return 1;
  }

  g_listen_socket = listen_socket;
  std::cout << "[TCP] Server started and listening on port " << port << "\n";

  char buffer[kBufferSize];
  while (!g_tcp_stop.load()) {
    socket_handle_t client = accept(listen_socket, nullptr, nullptr);
    if (client == invalid_socket_handle) {
      if (g_tcp_stop.load()) break;
      std::cerr << "[TCP] accept failed\n";
      continue;
    }
    std::cout << "[TCP] Client connected\n";

    char buffer[1024];
    int received = recv(client, buffer, sizeof(buffer) - 1, 0);
    if (received > 0) {
      buffer[received] = '\0';
      std::string message(buffer);
      std::cout << "[TCP] Received: " << message << std::endl;
      send(client, message.c_str(), (int)message.size(), 0);
    } else if (received == 0) {
      std::cout << "[TCP] Client disconnected (graceful)\n";
    } else {
      std::cerr << "[TCP] recv failed\n";
    }

    close_socket(client);
  }

  if (listen_socket != invalid_socket_handle) close_socket(listen_socket);
  g_listen_socket = invalid_socket_handle;
  net_cleanup();
  return 0;
}

int stop_tcp() {
  std::cout << "[TCP] Stopping TCP service...\n";
  g_tcp_stop = true;

  if (g_listen_socket != invalid_socket_handle) {
    shutdown(g_listen_socket, SD_BOTH);
    close_socket(g_listen_socket);
    std::cout << "[TCP] Closed listening socket\n";
  }

  std::cout << "[TCP] TCP service fully stopped\n";
  return 0;
}
