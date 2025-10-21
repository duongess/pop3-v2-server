#pragma once
#include "table.h"

class UserTable : public Table {
public:
    using Table::Table; // kế thừa ctor: UserTable(DbConnection&)

    bool createTableIfNeeded() override;

    bool createUser(const std::string& username, const std::string& passwordHash);
    int findUserId(const std::string& username, const std::string& passwordHash);
    // std::optional<User> findByUsername(...); v.v.
    std::unordered_map<std::string, std::string> getAllUser();
};
