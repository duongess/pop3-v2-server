#include "user.h"

bool UserTable::createTableIfNeeded() {
    static const char* sql =
        "CREATE TABLE IF NOT EXISTS users ("
        "  userId INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  username TEXT UNIQUE NOT NULL,"
        "  passwordHash TEXT NOT NULL,"
        "  session_token TEXT,"
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

std::vector<SetUser> UserTable::getAllUser() {
    const char* sql = "SELECT userId, username, passwordHash, session_token FROM users ORDER BY userId;";

    std::vector<SetUser> out;
    sqlite3_stmt* st = nullptr;
    int prep = sqlite3_prepare_v2(conn_.get(), sql, -1, &st, nullptr);
    if (prep != SQLITE_OK) {
        console.error("[DB] prepare failed: ", sqlite3_errmsg(conn_.get()));
        return out;
    }

    while (true) {
        int rc = sqlite3_step(st);
        if (rc == SQLITE_ROW) {
            const unsigned int i = sqlite3_column_int(st, 0);
            const unsigned char* u = sqlite3_column_text(st, 1);
            const unsigned char* p = sqlite3_column_text(st, 2);
            console.debug("database...null");

            const unsigned char* t  =  sqlite3_column_text(st, 3) ? sqlite3_column_text(st, 3) : (const unsigned char*)"";
            out.push_back(SetUser(i, reinterpret_cast<const char*>(u)
            , reinterpret_cast<const char*>(p), reinterpret_cast<const char*>(t)));
        } else if (rc == SQLITE_DONE) {
            break;
        } else {
            console.error("[DB] step failed: ", sqlite3_errmsg(conn_.get()));
            break;
        }
    }
    console.debug("database");
    sqlite3_finalize(st);
    return out;
}

int UserTable::findUserId(const std::string& username, const std::string& passwordHash) {
    const char* sql = "SELECT userId FROM users WHERE username = ? AND passwordHash = ? LIMIT 1;";

    sqlite3_stmt* st = nullptr;
    int prep = sqlite3_prepare_v2(conn_.get(), sql, -1, &st, nullptr);
    if (prep != SQLITE_OK) {
        console.error("[DB] prepare failed: ", sqlite3_errmsg(conn_.get()));
        return -1;
    }

    sqlite3_bind_text(st, 1, username.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(st, 2, passwordHash.c_str(), -1, SQLITE_TRANSIENT);
    if (sqlite3_step(st) == SQLITE_ROW) {
        return sqlite3_column_int(st, 0);
    } else {
        return -1; // not found
    }
    sqlite3_finalize(st);
    return -1;
}

std::string generateTokenSimple(const std::string& username) {
    std::string seed = username + std::to_string(std::time(nullptr)) + std::to_string(rand());
    std::hash<std::string> hasher;
    size_t value = hasher(seed);

    std::ostringstream oss;
    oss << std::hex << std::setw(16) << std::setfill('0') << value;
    return oss.str(); // hex-style token, no external libs
}

bool UserTable::setSessionToken(int userId, const std::string& token) {
    const char* sql = "UPDATE users SET session_token = ? WHERE userId = ?;";
    sqlite3_stmt* st = nullptr;
    if (sqlite3_prepare_v2(conn_.get(), sql, -1, &st, nullptr) != SQLITE_OK) {
        console.error("[DB] prepare failed: ", sqlite3_errmsg(conn_.get()));
        return false;
    }

    sqlite3_bind_text(st, 1, token.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(st, 2, userId);

    int rc = sqlite3_step(st);
    sqlite3_finalize(st);
    return rc == SQLITE_DONE;
}
