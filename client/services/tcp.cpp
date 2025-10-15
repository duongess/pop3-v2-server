#include "tcp.h"

int sendMessage(const std::string& host, const std::string& port, const std::string& message) {
  TCP<std::string> client;

  // 1️⃣ Kết nối đến server
  if (!client.connectTo(host, port)) {
    console.error("[TCP] Failed to connect to ", host, ":", port);
    return 1;
  }
  console.success("[TCP] Connected to ", host, ":", port);

  // 2️⃣ Gửi dữ liệu
  if (!client.sendData(message)) {
    console.error("[TCP] Send failed.");
    client.close();
    return 1;
  }
  console.info("[TCP] Sent: ", message);

  // 3️⃣ Nhận phản hồi (nếu có)
  Response<std::string> response = client.receiveData();
  if (response.status == Status::OK) {
    console.info("[TCP] Received: ", response.data);
  }

  // 4️⃣ Đóng kết nối
  client.close();
  return 0;
}
