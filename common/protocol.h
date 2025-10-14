// protocol.h
#pragma once
#include <string>
#include <iostream>
#include "utils.h"

addrinfo* resolveAddress(const std::string& host, const std::string& port, bool passive);