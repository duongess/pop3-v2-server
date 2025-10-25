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

bool MailTable::addMail(const Mail &m)
{
    static const char *sql =
        "INSERT INTO emails (userId, uidl, subject, body, flags, receivedAt) "
        "VALUES (?, ?, ?, ?, ?, ?);";

    sqlite3_stmt *stmt = nullptr;
    int rc = sqlite3_prepare_v2(conn_.get(), sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK)
    {
        std::cerr << "[MailTable] Failed to prepare insert statement: " << sqlite3_errmsg(conn_.get()) << "\n";
        return false;
    }

    // Generate a unique UIDL (e.g., using timestamp + random part)
    std::string uidl = std::to_string(m.userId) + "_" + std::to_string(m.receivedAt);

    // Default flag (e.g., "unseen" or empty)
    std::string flags = "unseen";

    // Bind parameters
    sqlite3_bind_int(stmt, 1, m.userId);
    sqlite3_bind_text(stmt, 2, uidl.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, m.subject.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 4, m.body.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, flags.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int64(stmt, 6, m.receivedAt);

    // Execute
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        std::cerr << "[MailTable] Failed to insert mail: " << sqlite3_errmsg(conn_.get()) << "\n";
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

bool MailTable::deleteFlaggedMails(int userId)
{
    const char *sql = "DELETE FROM emails WHERE userId = ? AND flags = 'read'";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(conn_.get(), sql, -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_int(stmt, 1, userId);
    bool ok = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);
    return ok;
}

std::vector<MailInfo> MailTable::listMailsForUser(int userId)
{
    std::vector<MailInfo> list;
    const char *sql =
        "SELECT mailId, uidl, LENGTH(body) "
        "FROM emails WHERE userId = ? AND (flags IS NULL OR flags != 'read')";

    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(conn_.get(), sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "[MailTable] listMailsForUser prepare failed: "
                  << sqlite3_errmsg(conn_.get()) << "\n";
        return list;
    }

    sqlite3_bind_int(stmt, 1, userId);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        MailInfo info;
        info.mailId = sqlite3_column_int(stmt, 0);
        info.uidl = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        info.size = sqlite3_column_int(stmt, 2);
        list.push_back(info);
    }

    sqlite3_finalize(stmt);
    return list;
}

std::optional<MailInfo> MailTable::getMailInfo(int userId, int mailId)
{
    const char *sql =
        "SELECT mailId, uidl, LENGTH(body) "
        "FROM emails WHERE userId = ? AND mailId = ? "
        "AND (flags IS NULL OR flags != 'read')";

    sqlite3_stmt *stmt = nullptr;
    if (sqlite3_prepare_v2(conn_.get(), sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        std::cerr << "[MailTable] getMailInfo prepare failed: "
                  << sqlite3_errmsg(conn_.get()) << "\n";
        return std::nullopt;
    }

    sqlite3_bind_int(stmt, 1, userId);
    sqlite3_bind_int(stmt, 2, mailId);

    MailInfo info;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        info.mailId = sqlite3_column_int(stmt, 0);
        info.uidl = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        info.size = sqlite3_column_int(stmt, 2);
        sqlite3_finalize(stmt);
        return info;
    }

    sqlite3_finalize(stmt);
    return std::nullopt;
}

bool MailTable::addDemoMailsForUser(int userId)
{
    // Demo subjects & bodies
    std::vector<std::pair<std::string, std::string>> demoMails = {
        {"Welcome to POP3 Demo", "This is a test message for your inbox."},
        {"System Notice", "Remember to check your new messages daily."},
        {"Newsletter", "Here is your weekly update! Enjoy."},
        {"Test Mail", "If you can read this, the mail system works!"}
    };

    for (const auto& [subject, body] : demoMails)
    {
        Mail m;
        m.userId = userId;
        m.subject = subject;
        m.body = body;
        m.receivedAt = std::time(nullptr);

        if (!addMail(m))
        {
            std::cerr << "[MailTable] Failed to insert demo mail for userId=" << userId << "\n";
            return false;
        }
    }

    std::cout << "[MailTable] Demo mails inserted for userId=" << userId << "\n";
    return true;
}