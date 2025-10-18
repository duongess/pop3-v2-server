#include "utils.h"

void menuClient() {
    console.log("\n=== Client Menu ===\n");
    console.log("1) Text send text\n");
    console.log("2> Email\n");
    console.log("e) End client\n> ");
}

void menuPop3v2() {
    console.log("\n=== Email Menu ===\n");
    console.log("1) Login: login <host> <username> <password>\n");
    // console.log("2> Email\n");
    console.log("q) Quit\n> ");
}

std::vector<std::string> splitWs(std::string_view s) {
  std::vector<std::string> out;
  std::string cur;
  for (char c : s) {
    if (std::isspace(static_cast<unsigned char>(c))) {
      if (!cur.empty()) { out.push_back(cur); cur.clear(); }
    } else cur.push_back(c);
  }
  if (!cur.empty()) out.push_back(cur);
  return out;
}

std::string tolowerCopy(std::string s) {
  std::transform(s.begin(), s.end(), s.begin(),
                 [](unsigned char c){ return (char)std::tolower(c); });
  return s;
}

// (Tuỳ chọn) kiểm tra rất sơ bộ host/ip
bool lookLikeHost(const std::string& s) {
  if (s.empty()) return false;
  // Cho phép chữ, số, '.', '-', ':' (IPv6/port nếu bạn hỗ trợ sau)
  return std::all_of(s.begin(), s.end(), [](unsigned char c){
    return std::isalnum(c) || c=='.' || c=='-' || c==':';
  });
}

// --- parser chính ---
ParsedCommand parseCliLine(std::string_view line) {
  ParsedCommand pc;
  auto t = splitWs(line);
  if (t.empty()) { pc.error = "Empty command"; return pc; }

  const std::string cmd = tolowerCopy(t[0]);
  if (cmd == "login") {
    if (t.size() != 4) { pc.error = "Usage: login <host> <username> <password>"; return pc; }
    if (!lookLikeHost(t[1])) { pc.error = "Invalid host/IP"; return pc; }
    pc.cmd = CliCmd::LOGIN;
    pc.payload = LoginArgs{ t[1], t[2], t[3] };
    return pc;
  }

  if (cmd == "sync") {
    if (t.size() != 1) { pc.error = "Usage: sync"; return pc; }
    pc.cmd = CliCmd::SYNC;
    pc.payload = SyncArgs{};
    return pc;
  }

  if (cmd == "quit") {
    if (t.size() != 1) { pc.error = "Usage: quit"; return pc; }
    pc.cmd = CliCmd::QUIT;
    pc.payload = QuitArgs{};
    return pc;
  }

  pc.error = "Unknown command: " + t[0];
  return pc;
}