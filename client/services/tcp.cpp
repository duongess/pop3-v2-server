// TCP client: simple send from user input
#include "tcp.h"

int sendMessage(const std::string&  host, const std::string&  port, const std::string& message) {
  if (!net_init()) {
    std::cerr << "Failed to init networking" << std::endl;
    return 1;
  }

  // --- Resolve server address ---
  addrinfo* result = resolveAddress(host, port, false);
  socket_handle_t s = invalid_socket_handle;

  for (addrinfo* ptr = result; ptr != nullptr; ptr = ptr->ai_next) {
    s = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
    if (s == invalid_socket_handle) continue;
    if (connect(s, ptr->ai_addr, (int)ptr->ai_addrlen) == 0) break;
    close_socket(s);
    s = invalid_socket_handle;
  }
  freeaddrinfo(result);

  if (s == invalid_socket_handle) return 1;

  std::cout << "Connected to " << host << ":" << port << "\n";

  // --- Gửi bản tin ---
  const char* data = message.c_str();
  int total_len = static_cast<int>(message.size());
  int sent_total = 0;

  while (sent_total < total_len) {
    int sent = send(s, message.c_str(), total_len - sent_total, 0);
    if (sent <= 0) {
      std::cerr << "❌ Send failed.\n";
      close_socket(s);
      net_cleanup();
      return 1;
    }
    sent_total += sent;
  }

  std::cout << "Sent successfully " << sent_total << " bytes to server.\n";

  // --- Cleanup ---
  close_socket(s);
  net_cleanup();
  return 0;
}
