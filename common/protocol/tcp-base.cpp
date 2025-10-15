#include "tcp-base.h"

std::atomic<bool> TCPBase::stop_flag{false};

// helper
static bool set_reuseaddr(socket_handle_t s) {
    int opt = 1;
    return setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt)) == 0;
}

TCPBase::~TCPBase() {
    close();
}

bool TCPBase::connectTo(const std::string& host, const std::string& port) {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "[TCP] Failed to init Winsock\n";
        return false;
    }

    addrinfo* result = resolveAddress(host, port, true);

    for (addrinfo* ptr = result; ptr != nullptr; ptr = ptr->ai_next) {
        sock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (sock == INVALID_SOCKET) continue;

        if (connect(sock, ptr->ai_addr, (int)ptr->ai_addrlen) == 0) break;

        closesocket(sock);
        sock = INVALID_SOCKET;
    }
    freeaddrinfo(result);

    if (sock == INVALID_SOCKET) {
        std::cerr << "[TCP] Unable to connect to server\n";
        WSACleanup();
        return false;
    }

    std::cout << "[TCP] Connected to " << host << ":" << port << "\n";
    return true;
}

bool TCPBase::bindAndListen(const std::string& host, const std::string& port) {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "[TCP] Failed to init Winsock\n";
        return false;
    }

    addrinfo* result = resolveAddress(host, port, true);

    for (addrinfo* ptr = result; ptr != nullptr; ptr = ptr->ai_next) {
        sock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (sock == INVALID_SOCKET) continue;

        set_reuseaddr(sock);
        if (bind(sock, ptr->ai_addr, (int)ptr->ai_addrlen) == 0) break;

        closesocket(sock);
        sock = INVALID_SOCKET;
    }
    freeaddrinfo(result);

    if (sock == INVALID_SOCKET) {
        std::cerr << "[TCP] Failed to bind socket\n";
        WSACleanup();
        return false;
    }

    if (listen(sock, SOMAXCONN) != 0) {
        std::cerr << "[TCP] listen failed\n";
        closesocket(sock);
        WSACleanup();
        return false;
    }

    std::cout << "[TCP] Listening on port " << port << "\n";
    stop_flag = false;
    return true;
}

bool TCPBase::acceptClient(TCPBase& client) {
    sockaddr_in client_addr{};
    int addr_size = sizeof(client_addr);
    SOCKET client_sock = accept(sock, (sockaddr*)&client_addr, &addr_size);
    if (client_sock == INVALID_SOCKET) {
        if (shouldStop()) return false;
        std::cerr << "[TCP] accept failed\n";
        return false;
    }
    client.sock = client_sock;
    std::cout << "[TCP] Client connected\n";
    return true;
}

void TCPBase::close() {
    if (sock != INVALID_SOCKET) {
        shutdown(sock, SD_BOTH);
        closesocket(sock);
        sock = INVALID_SOCKET;
        std::cout << "[TCP] Socket closed\n";
    }
    WSACleanup();
}

void TCPBase::requestStop() {
    stop_flag = true;
}

bool TCPBase::shouldStop() {
    return stop_flag.load();
}
