#include "client.h"

bool Client::connect() {
    if (client.isConnected()) return true;
    if (!client.connectTo(host, port)) {
        console.error("[TCP] Failed to connect to ", host, ":", port);
        return false;
    }
    console.success("[TCP] Connected to ", host, ":", port);
    return true;
}

void Client::close() { client.close(); }

std::string Client::request(const std::string& message) {
    // KHÔNG connectTo ở đây nữa
    if (!client.isConnected()) {
        this->connect();
    }

    if (!client.sendData(message)) {
        console.error("[TCP] Send failed.");
        return "";
    }
    console.info("[TCP] Sent: ", message);

    Response response = client.receiveData();
    if (response.status == Status::OK || response.status == Status::Continue) {
        console.info("[TCP] Received: ", response.data);
        return response.data;
    }
    console.error("[TCP] Error status: ", (int)response.status, " - ", response.error);
    return "";
}

void Client::sendText(const std::string& message) {
    this->request(message);
};
void Client::sendPopv2(const std::string& message) {
    RequestPopV2 p;
    ParsedCommand pc = parseCliLine(message);
    if (!pc.ok()) {
        console.error(pc.error);
        return;
    }
    switch (pc.cmd) {
    case CliCmd::LOGIN: {
      const LoginArgs& a = std::get<LoginArgs>(pc.payload);
      this->host = a.host;
      this->request(p.USER(a.user));
      std::string token = this->request(p.PASS(a.pass));
      if (token != "") {
        this->token = token;
      };
      // connect + gửi USER/PASS theo POP3 (dùng code sẵn có của bạn)
      // connectTo(a.host); send_line(USER a.user); recv_status(); send_line(PASS a.pass); ...
      break;
    }
    case CliCmd::SYNC: {
      // gửi UIDL/LIST, đối sánh local, RETR các mail mới, lưu file, in số lượng
      // sync_flow();
      break;
    }
    case CliCmd::QUIT: {
      // gửi QUIT và đóng kết nối
      // send_line("QUIT\r\n"); close();
      break;
    }
    default: break;
  }
}