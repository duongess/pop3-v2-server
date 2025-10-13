// Simple client that sends a message and prints echo
#include <iostream>
#include <string>
#include <cstring>
#include "../common/utils.h"
#include "../common/protocol.h"

int main(int argc, char* argv[]) {
  const char* host = "localhost";
  const char* port = "6000";
  std::string message = kDefaultMessage;
  if (argc >= 2) host = argv[1];
  if (argc >= 3) port = argv[2];
  if (argc >= 4) message = argv[3];

  if (!net_init()) {
    std::cerr << "Failed to init networking" << std::endl;
    return 1;
  }

  addrinfo hints{};
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;

  addrinfo* result = nullptr;
  int gai = getaddrinfo(host, port, &hints, &result);
  if (gai != 0 || !result) {
    std::cerr << "getaddrinfo failed" << std::endl;
    net_cleanup();
    return 1;
  }

  socket_handle_t s = invalid_socket_handle;
  for (addrinfo* ptr = result; ptr != nullptr; ptr = ptr->ai_next) {
    s = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
    if (s == invalid_socket_handle) continue;
    if (connect(s, ptr->ai_addr, (int)ptr->ai_addrlen) == 0) {
      break;
    }
    close_socket(s);
    s = invalid_socket_handle;
  }
  freeaddrinfo(result);

  if (s == invalid_socket_handle) {
    std::cerr << "Unable to connect to server" << std::endl;
    net_cleanup();
    return 1;
  }

  std::cout << "Connected to " << host << ":" << port << std::endl;

  // Send the message
  const char* data = message.c_str();
  int remaining = (int)message.size();
  int sent_total = 0;
  while (remaining > 0) {
    int sent = send(s, data + sent_total, remaining, 0);
    if (sent <= 0) {
      std::cerr << "send failed" << std::endl;
      close_socket(s);
      net_cleanup();
      return 1;
    }
    sent_total += sent;
    remaining -= sent;
  }

  // Receive echo
  char buffer[kBufferSize];
  int received = recv(s, buffer, (int)sizeof(buffer) - 1, 0);
  if (received > 0) {
    buffer[received] = '\0';
    std::cout << "Received: " << buffer << std::endl;
  } else {
    std::cerr << "recv failed or connection closed" << std::endl;
  }

  close_socket(s);
  net_cleanup();
  return 0;
}
