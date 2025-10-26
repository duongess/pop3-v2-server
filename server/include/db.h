#pragma once
#include "user.h"
#include "mail.h"
#include "string"
#include "dbConnection.h"

class DB {
public:
    DB();

    // tiện ích init schema 1 chỗ
    bool initSchema() {
        return user.createTableIfNeeded() && mail.createTableIfNeeded();
    }
    
    void dumpTables();


    DbConnection conn; // nếu cần truy cập thô
    UserTable user;
    MailTable mail;
};