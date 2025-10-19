#pragma once
#include "table.h"

class MailTable : public Table
{
public:
    using Table::Table;

    bool createTableIfNeeded() override;

    bool setFlag(int mailId, const std::string &flag);
    // ví dụ API riêng:
    // bool addMail(const Mail& m);
    // std::vector<Mail> listByUser(int userId);
};
