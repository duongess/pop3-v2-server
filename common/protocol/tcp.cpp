#include "tcp.h"

template<typename T>
bool TCP<T>::sendData(const std::string& data) {
    ssize_t sent = send(sock, data.c_str(), data.size(), 0);
    if (sent < 0) {
        std::cerr << "[TCP] Send failed\n";
        return false;
    }

    return true;
}


template<typename T>
Response<T> TCP<T>::receiveData(const size_t& size) {
    char buffer[size] = {0};

    ssize_t bytesReceived = recv(sock, buffer, size - 1, 0);

    if (bytesReceived < 0) {
        std::cerr << "[TCP] Receive error\n";
        return { Status::ServerError, {} };
    }
    if (bytesReceived == 0) {
        std::cerr << "[TCP] Connection closed\n";
        return { Status::BadRequest, {} };
    }

    // Chuyển buffer -> T (giả sử T là std::string)
    T data(buffer, bytesReceived);
    return { Status::OK, data };
}

template class TCP<std::string>;