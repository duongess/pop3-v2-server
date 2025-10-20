#include "user.h"

bool UserTable::createTableIfNeeded() {
    static const char* sql =
        "CREATE TABLE IF NOT EXISTS users ("
        "  userId INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  username TEXT UNIQUE NOT NULL,"
        "  passwordHash TEXT NOT NULL,"
        "  createdAt INTEGER NOT NULL"
        ");";
    return exec_sql(conn_.get(), sql, "Users");
}

bool UserTable::createUser(const std::string& username, const std::string& passwordHash) {
    const char* sql =
        "INSERT INTO users(username, passwordHash, createdAt)"
        " VALUES(?, ?, strftime('%s','now'));";

    sqlite3_stmt* st = nullptr;
    if (sqlite3_prepare_v2(conn_.get(), sql, -1, &st, nullptr) != SQLITE_OK) return false;

    sqlite3_bind_text(st, 1, username.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(st, 2, passwordHash.c_str(), -1, SQLITE_TRANSIENT);

    int rc = sqlite3_step(st);
    sqlite3_finalize(st);
    return rc == SQLITE_DONE;
}

std::unordered_map<std::string, std::string> UserTable::getAllUser() {
    const char* sql = "SELECT username, passwordHash FROM users ORDER BY userId;";

    std::unordered_map<std::string, std::string> out;
    sqlite3_stmt* st = nullptr;
    int prep = sqlite3_prepare_v2(conn_.get(), sql, -1, &st, nullptr);
    if (prep != SQLITE_OK) {
        console.error("[DB] prepare failed: ", sqlite3_errmsg(conn_.get()));
        return out;
    }

    while (true) {
        int rc = sqlite3_step(st);
        if (rc == SQLITE_ROW) {
            const unsigned char* u = sqlite3_column_text(st, 0);
            const unsigned char* p = sqlite3_column_text(st, 1);
            out[reinterpret_cast<const char*>(u)] = reinterpret_cast<const char*>(p);
        } else if (rc == SQLITE_DONE) {
            break;
        } else {
            console.error("[DB] step failed: ", sqlite3_errmsg(conn_.get()));
            break;
        }
    }
    sqlite3_finalize(st);
    return out;
}