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
    if (!net_init()) {
        std::cerr << "[TCP] Failed to init networking (WSAStartup)\n";
        return 1;
    }

    addrinfo* result = resolveAddress(host, port, false);

    for (addrinfo* ptr = result; ptr != nullptr; ptr = ptr->ai_next) {
        sock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (sock == invalid_socket_handle) continue;

        std::cout << "[DEBUG] Trying connect... " << host << ":" << port << "\n";

        int ret = connect(sock, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (ret == 0) {
            std::cout << "[DEBUG] Connected OK\n";
            break;
        } else {
            int err = WSAGetLastError();   // 🔥 Lấy error NGAY SAU connect()
            std::cerr << "[DEBUG] Connect failed with code: " << err << "\n";
            close_socket(sock);
            sock = invalid_socket_handle;
        }
    }

    freeaddrinfo(result);

    if (sock == invalid_socket_handle) {
        std::cerr << "[TCP] Unable to connect to server\n";
        net_cleanup();
        return false;
    }

    std::cout << "[TCP] Connected to " << host << ":" << port << "\n";
    return true;
}

bool TCPBase::bindAndListen(const std::string& host, const std::string& port) {
    if (!net_init()) {
        std::cerr << "[TCP] Failed to init networking (WSAStartup)\n";
        return 1;
    }

    addrinfo* result = resolveAddress(host, port, true);

    for (addrinfo* ptr = result; ptr != nullptr; ptr = ptr->ai_next) {
        sock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (sock == invalid_socket_handle) continue;

        set_reuseaddr(sock);
        if (bind(sock, ptr->ai_addr, (int)ptr->ai_addrlen) == 0) break;

        closesocket(sock);
        sock = invalid_socket_handle;
    }
    freeaddrinfo(result);

    if (sock == invalid_socket_handle) {
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
    std::cout << "[DEBUG] accept() on sock=" << this->sock << "\n";
    socket_handle_t client_sock = accept(this->sock, nullptr, nullptr);
    std::cout << "[DEBUG] accept() afef sock=" << client_sock << "\n";
    if (client_sock == invalid_socket_handle) {
        std::cerr << "[TCP] accept failed: " << WSAGetLastError() << "\n";
        return false;
    }
    std::cout << "[DEBUG] accept OK, client sock=" << client_sock << "\n";
    client.sock = client_sock;
    return true;
}



void TCPBase::close() {
    close_socket(this->sock);
}

void TCPBase::clean() {
    if (this->sock != invalid_socket_handle) {
        shutdown(this->sock, SD_BOTH);
        this->close();
        this->sock = invalid_socket_handle;
        std::cout << "[TCP] Socket closed\n";
    }
    net_cleanup();
}

void TCPBase::requestStop() {
    stop_flag = true;
}

bool TCPBase::shouldStop() {
    return stop_flag.load();
}
