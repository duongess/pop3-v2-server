#include "account.h"

bool AccountTable::createTableIfNeeded() {
    static const char* sql =
        "CREATE TABLE IF NOT EXISTS Account ("
        "  username TEXT UNIQUE NOT NULL,"
        "  host TEXT NOT NULL,"
        "  port TEXT NOT NULL,"
        "  lastAccessed INTEGER NOT NULL,"
        "  syncLast INTEGER NOT NULL"
        ");";
    return exec_sql(conn_.get(), sql, "Account");
}

bool AccountTable::createAccount(const std::string& username, const std::string& host, const std::string& port) {
    const char* sql =
        "INSERT INTO Account(username, host, port, lastAccessed, syncLast)"
        " VALUES(?, ?, ?, strftime('%s','now'), 0);";

    sqlite3_stmt* st = nullptr;
    if (sqlite3_prepare_v2(conn_.get(), sql, -1, &st, nullptr) != SQLITE_OK) return false;

    sqlite3_bind_text(st, 1, username.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(st, 2, host.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(st, 3, port.c_str(), -1, SQLITE_TRANSIENT);

    int rc = sqlite3_step(st);
    sqlite3_finalize(st);
    return rc == SQLITE_DONE;
}