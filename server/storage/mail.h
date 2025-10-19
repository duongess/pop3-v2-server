#pragma once
#include "table.h"

class MailTable : public Table {
public:
    using Table::Table;

    bool createTableIfNeeded() override;

    // ví dụ API riêng:
    // bool addMail(const Mail& m);
    // std::vector<Mail> listByUser(int userId);
};
