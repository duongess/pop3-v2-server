#include "tcp-base.h"

atomic<bool> TCP::stop_flag{false};

// helper
static bool set_reuseaddr(socket_handle_t s) {
    int opt = 1;
    return setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt)) == 0;
}

TCP::~TCP() {
    close();
}

bool TCP::connectTo(const string& host, const string& port) {
    if (!net_init()) {
        cerr << "[TCP] Failed to init networking (WSAStartup)\n";
        return 1;
    }

    addrinfo* result = resolveAddress(host, port, false);

    for (addrinfo* ptr = result; ptr != nullptr; ptr = ptr->ai_next) {
        sock = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if (sock == invalid_socket_handle) continue;

        cout << "[DEBUG] Trying connect... " << host << ":" << port << "\n";

        int ret = connect(sock, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (ret == 0) {
            cout << "[DEBUG] Connected OK\n";
            break;
        } else {
            int err = WSAGetLastError();
            cerr << "[DEBUG] Connect failed with code: " << err << "\n";
            close_socket(sock);
            sock = invalid_socket_handle;
        }
    }

    freeaddrinfo(result);

    if (sock == invalid_socket_handle) {
        cerr << "[TCP] Unable to connect to server\n";
        net_cleanup();
        return false;
    }

    cout << "[TCP] Connected to " << host << ":" << port << "\n";
    return true;
}

bool TCP::bindAndListen(const string& host, const string& port) {
    if (!net_init()) {
        console.error("[TCP] Failed to init networking (WSAStartup)");
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
        console.error("[TCP] Failed to bind socket");
        WSACleanup();
        return false;
    }

    if (listen(sock, SOMAXCONN) != 0) {
        console.error("[TCP] listen failed");
        closesocket(sock);
        WSACleanup();
        return false;
    }

    console.success("[TCP] Listening on port ", port);
    stop_flag = false;
    return true;
}

bool TCP::acceptClient(TCP& client) {
    if (client.sock != invalid_socket_handle) return true;
    console.debug("accept() on sock=", this->sock);
    socket_handle_t client_sock = accept(this->sock, nullptr, nullptr);
    console.debug("accept() afef sock=", client_sock);
    if (client_sock == invalid_socket_handle) {
        console.debug("[TCP] accept failed: ", WSAGetLastError());
        return false;
    }
    console.success("accept OK, client sock=", client_sock);
    client.sock = client_sock;
    return true;
}



void TCP::close() {
    close_socket(this->sock);
}

void TCP::clean() {
    if (this->sock != invalid_socket_handle) {
        shutdown(this->sock, SD_BOTH);
        this->close();
        this->sock = invalid_socket_handle;
        console.stopping("[TCP] Socket closed\n");
    }
    net_cleanup();
}

void TCP::requestStop() {
    stop_flag = true;
}

bool TCP::shouldStop() {
    return stop_flag.load();
}

bool TCP::sendData(const string& data) {
    ssize_t sent = send(sock, data.c_str(), data.size(), 0);
    if (sent < 0) {
        cerr << "[TCP] Send failed\n";
        return false;
    }

    return true;
}


Response TCP::receiveData(const size_t& size) {
    vector<char> buffer(size);

    int bytesReceived = recv(sock, buffer.data(), (int)buffer.size() - 1, 0);

    if (bytesReceived < 0) {
        int err = WSAGetLastError();
        cerr << "[TCP] Receive error, code=" << err << "\n";
        return { Status::ServerError, {} };
    }

    if (bytesReceived == 0) {
        cerr << "[TCP] Connection closed by peer\n";
        return { Status::BadRequest, {} };
    }

    buffer[bytesReceived] = '\0';
    string data(buffer.data(), bytesReceived);
    return { Status::OK, data };
}