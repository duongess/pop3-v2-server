#pragma once
#include "table.h"
#include <sstream>
#include <iomanip>
#include <string>

class UserTable : public Table {
public:
    using Table::Table; // kế thừa ctor: UserTable(DbConnection&)

    bool createTableIfNeeded() override;

    bool createUser(const std::string& username, const std::string& passwordHash);
    int findUserId(const std::string& username, const std::string& passwordHash);
    // std::optional<User> findByUsername(...); v.v.
    std::vector<SetUser> getAllUser();

    // lightweight token generator (implemented in storage/user.cpp)
    std::string generateTokenSimple(const std::string& username);
    bool setSessionToken(int userId, const std::string& token);

};
