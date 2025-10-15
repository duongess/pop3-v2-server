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
    std::vector<char> buffer(size);

    int bytesReceived = recv(sock, buffer.data(), (int)buffer.size() - 1, 0);

    if (bytesReceived < 0) {
        int err = WSAGetLastError();
        std::cerr << "[TCP] Receive error, code=" << err << "\n";
        return { Status::ServerError, {} };
    }

    if (bytesReceived == 0) {
        std::cerr << "[TCP] Connection closed by peer\n";
        return { Status::BadRequest, {} };
    }

    buffer[bytesReceived] = '\0';
    T data(buffer.data(), bytesReceived);
    return { Status::OK, data };
}


template class TCP<std::string>;