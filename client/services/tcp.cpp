// TCP client: simple send from user input
#include <iostream>
#include <string>
#include <cstring>
#include "../../common/utils.h"
#include "../../common/protocol.h"

int run_tcp_client(const std::string&  host, const std::string&  port, const std::string& message) {
  if (!net_init()) {
    std::cerr << "Failed to init networking" << std::endl;
    return 1;
  }

  // --- Resolve server address ---
  addrinfo hints{};
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;

  addrinfo* result = nullptr;
  int gai = getaddrinfo(host.c_str(), port.c_str(), &hints, &result);
  if (gai != 0 || !result) {
    std::cerr << "getaddrinfo failed\n";
    net_cleanup();
    return 1;
  }

  // --- Create socket and connect ---
  socket_handle_t s = invalid_socket_handle;
  for (addrinfo* ptr = result; ptr != nullptr; ptr = ptr->ai_next) {
    s = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
    if (s == invalid_socket_handle) continue;
    if (connect(s, ptr->ai_addr, (int)ptr->ai_addrlen) == 0) break;
    close_socket(s);
    s = invalid_socket_handle;
  }
  freeaddrinfo(result);

  if (s == invalid_socket_handle) {
    std::cerr << "Unable to connect to server\n";
    net_cleanup();
    return 1;
  }

  std::cout << "✅ Connected to " << host << ":" << port << "\n";

  // --- Gửi bản tin ---
  const char* data = message.c_str();
  int total_len = static_cast<int>(message.size());
  int sent_total = 0;

  while (sent_total < total_len) {
    int sent = send(s, data + sent_total, total_len - sent_total, 0);
    if (sent <= 0) {
      std::cerr << "❌ Send failed.\n";
      close_socket(s);
      net_cleanup();
      return 1;
    }
    sent_total += sent;
  }

  std::cout << "📤 Đã gửi thành công " << sent_total << " bytes tới server.\n";

  // --- Cleanup ---
  close_socket(s);
  net_cleanup();
  return 0;
}
