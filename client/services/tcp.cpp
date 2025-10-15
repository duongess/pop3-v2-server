#include "tcp.h"

int sendMessage(const std::string& host, const std::string& port, const std::string& message) {
  TCP<std::string> client;

  // 1️⃣ Kết nối đến server
  if (!client.connectTo(host, port)) {
    std::cerr << "❌ Failed to connect to " << host << ":" << port << "\n";
    return 1;
  }
  std::cout << "✅ Connected to " << host << ":" << port << "\n";

  // 2️⃣ Gửi dữ liệu
  if (!client.sendData(message)) {
    std::cerr << "❌ Send failed.\n";
    client.close();
    return 1;
  }
  std::cout << "📤 Sent: " << message << "\n";

  // 3️⃣ Nhận phản hồi (nếu có)
  Response<std::string> response = client.receiveData();
  if (response.status == Status::OK) {
    std::cout << "📥 Received: " << response.data << "\n";
  }

  // 4️⃣ Đóng kết nối
  client.close();
  return 0;
}
