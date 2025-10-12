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
