#pragma once

#include <cstdint>

#ifndef NOMINMAX
#define NOMINMAX
#endif
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <iostream>
#include <atomic>
#include <iostream>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <memory>
#include <unordered_map>
#include <mutex>
#include <thread>
#include "console.h"

using socket_handle_t = SOCKET;
constexpr socket_handle_t invalid_socket_handle = INVALID_SOCKET;
const Console console;

bool net_init();
void net_cleanup();
void close_socket(socket_handle_t s);
addrinfo* resolveAddress(const std::string& host, const std::string& port, bool passive);
std::vector<std::string> splitWs(std::string_view s);
std::string tolowerCopy(std::string s);
bool lookLikeHost(const std::string& s);

