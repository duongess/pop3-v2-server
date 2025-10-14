#pragma once

#include <cstdint>

#ifndef NOMINMAX
#define NOMINMAX
#endif
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>

using socket_handle_t = SOCKET;
constexpr socket_handle_t invalid_socket_handle = INVALID_SOCKET;

bool net_init();
void net_cleanup();
void close_socket(socket_handle_t s);
void menuServer();

