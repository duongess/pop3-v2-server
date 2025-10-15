#pragma once
#include "tcp-base.h"
#include <vector>

template<typename T>
class TCP : public TCPBase {
public:
    bool sendData(const std::string& data);
    Response<T> receiveData(const size_t& size = 1024);
};
