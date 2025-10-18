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
    switch (response.status) {
    case Status::OK:
    case Status::Continue: {
        console.info("[TCP] Received: ", response.data);

        if (response.status == Status::OK) {
            // Hoàn tất phiên/đợt trao đổi hiện tại
            this->client.close();
        } else { // Status::Continue
            // Server vẫn OK nhưng cần lệnh kế tiếp (ví dụ sau USER thì gửi PASS)
            console.info("[TCP] Continue: awaiting next command...");
            // KHÔNG close(), cứ return để caller gửi lệnh tiếp theo
        }
        return response.data;
    }

    case Status::BadRequest:
    case Status::ServerError:
    case Status::Redirect:
    default:
        // tuỳ bạn muốn log/throw gì ở đây
        console.error("[TCP] Error status: ", static_cast<int>(response.status),
                    " - ", response.error);
        break;
    }


    // 4️⃣ Đóng kết nối
    this->client.close();
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