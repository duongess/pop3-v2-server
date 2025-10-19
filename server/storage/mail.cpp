#include "mail.h"

bool MailTable::createTableIfNeeded()
{
    static const char *sql =
        "CREATE TABLE IF NOT EXISTS emails ("
        "  mailId INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  userId INTEGER NOT NULL REFERENCES users(id) ON DELETE CASCADE,"
        "  uidl TEXT UNIQUE NOT NULL,"
        "  subject TEXT,"
        "  body TEXT,"
        "  flags TEXT,"
        "  receivedAt INTEGER NOT NULL"
        ");";
    return exec_sql(conn_.get(), sql, "Emails");
}

bool MailTable::setFlag(int mailId, const std::string &flag)
{
    const char *sql = "UPDATE emails SET flags = ? WHERE mailId = ?";
    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(conn_.get(), sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_text(stmt, 1, flag.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, mailId);

    bool ok = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return ok;
}
