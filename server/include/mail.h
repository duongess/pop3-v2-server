#pragma once
#include "table.h"
#include <optional>




class MailTable : public Table
{
public:
    using Table::Table;

    bool createTableIfNeeded() override;
    int addMail(const Mail &m);
    // ví dụ API riêng:
    // bool addMail(const  Mail& m);
    // std::vector<Mail> listByUser(int userId);
    bool addDemoMailsForUser(int userId);
        // 🔹 New for POP3 LIST
    std::vector<MailInfo> listMailsForUser(int userId);
    std::optional<MailInfo> getMailInfo(int userId, int mailId);
};
