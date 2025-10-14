// protocol.cpp
#include "protocol.h"

addrinfo* resolveAddress(const std::string& host, const std::string& port, bool passive) {
  addrinfo hints{};
  hints.ai_family = AF_INET; // IPv4
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  if (passive) hints.ai_flags = AI_PASSIVE; // for bind()

  addrinfo* result = nullptr;
  int gai = getaddrinfo(
      passive ? nullptr : host.c_str(),
      port.c_str(),
      &hints,
      &result
  );
  if (gai != 0 || !result) {
    std::cerr << "[TCP] getaddrinfo failed: " << gai << std::endl;
    return nullptr;
  }
  return result;
}