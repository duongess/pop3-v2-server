#pragma once
#include <string>
#include <string_view>

namespace pop {

inline std::string make_line(std::string_view cmd, std::string_view arg = {}) {
  std::string out;
  out.reserve(cmd.size() + (arg.empty() ? 0 : (1 + arg.size())) + 2);
  out.append(cmd);
  if (!arg.empty()) { out.push_back(' '); out.append(arg); }
  out.append("\r\n"); // CRLF bắt buộc
  return out;
}

struct RequestPopV2 {
  // USER <username>\r\n
  std::string USER(std::string_view username) const {
    return make_line("USER", username);
  }
  // PASS <password>\r\n
  std::string PASS(std::string_view password) const {
    return make_line("PASS", password);
  }
  // UIDL\r\n
  std::string UIDL() const { return make_line("UIDL"); }
  // LIST\r\n
  std::string LIST() const { return make_line("LIST"); }
  // RETR <id>\r\n
  std::string RETR(int id) const { return make_line("RETR", std::to_string(id)); }
  // DELE <id>\r\n
  std::string DELE(int id) const { return make_line("DELE", std::to_string(id)); }
  // QUIT\r\n
  std::string QUIT() const { return make_line("QUIT"); }
};

} // namespace pop
