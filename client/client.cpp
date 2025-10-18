#include "client.h"

std::string Client::request(const std::string& message) {
    // 1️⃣ Kết nối đến server
    if (!this->client.connectTo(this->host, this->port)) {
        console.error("[TCP] Failed to connect to ", this->host, ":", this->port);
        return "";
    }
    console.success("[TCP] Connected to ", this->host, ":", this->port);

    // 2️⃣ Gửi dữ liệu
    if (!this->client.sendData(message)) {
        console.error("[TCP] Send failed.");
        this->client.close();
        return "";
    }
    console.info("[TCP] Sent: ", message);

    // 3️⃣ Nhận phản hồi (nếu có)
    Response response = this->client.receiveData();
    if (response.status == Status::OK) {
        console.info("[TCP] Received: ", response.data);
        this->client.close();
        return response.data;
    }

    // 4️⃣ Đóng kết nối
    this->client.close();
    return "";
}