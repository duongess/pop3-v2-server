#pragma once

#include <cstdint>

#ifdef _WIN32
#  define NOMINMAX
#  define WIN32_LEAN_AND_MEAN
#  include <winsock2.h>
#  include <ws2tcpip.h>
using socket_handle_t = SOCKET;
constexpr socket_handle_t invalid_socket_handle = INVALID_SOCKET;
#else
#  include <sys/types.h>
#  include <sys/socket.h>
#  include <netdb.h>
#  include <unistd.h>
using socket_handle_t = int;
constexpr socket_handle_t invalid_socket_handle = -1;
#endif

bool net_init();
void net_cleanup();
void close_socket(socket_handle_t s);

