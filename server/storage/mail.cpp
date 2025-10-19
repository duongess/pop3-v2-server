#include "mail.h"

bool MailTable::createTableIfNeeded() {
    static const char* sql =
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
