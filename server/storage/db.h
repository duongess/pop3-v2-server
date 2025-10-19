#pragma once
#include "user.h"
#include "mail.h"
#include "string"
#include "dbConnection.h"

class DB {
public:
    explicit DB()
        : conn("database/POP3V2.db"), user(conn), mail(conn) {}

    // tiện ích init schema 1 chỗ
    bool initSchema() {
        return user.createTableIfNeeded() && mail.createTableIfNeeded();
    }

    DbConnection conn; // nếu cần truy cập thô
    UserTable user;
    MailTable mail;
};