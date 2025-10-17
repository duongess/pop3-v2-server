#include "tcp.h"

string sendMessage(const string& host, const string& port, const string& message) {
  TCP<string> client;

  // 1️⃣ Kết nối đến server
  if (!client.connectTo(host, port)) {
    console.error("[TCP] Failed to connect to ", host, ":", port);
    return "";
  }
  console.success("[TCP] Connected to ", host, ":", port);

  // 2️⃣ Gửi dữ liệu
  if (!client.sendData(message)) {
    console.error("[TCP] Send failed.");
    client.close();
    return "";
  }
  console.info("[TCP] Sent: ", message);

  // 3️⃣ Nhận phản hồi (nếu có)
  Response<string> response = client.receiveData();
  if (response.status == Status::OK) {
    if (response.data = "") {
      
    }
    console.info("[TCP] Received: ", response.data);
    client.close();
    return response.data;
  }
  console.warn("[TCP] Server not return");
  return "";
}
