#ifndef POP3V2ACCOUNT_H_
#define POP3V2ACCOUNT_H_
#include "serverconfig.h"
#include "db.h"
#include "types/db.h"

class Pop3V2Account: public Account {
public:
    int userId;
    std::string host;

    void setUserId(const int userId);
    void setHost(const std::string& host);
    

    bool isLocked();
    void lock();
    void unLock();

private:
    bool locked = false;
};

class Pop3V2ServerConfig: public ServerConfig {
    private:
        DB db;

    public:
        Pop3V2ServerConfig();
        bool loadAccountsFromDB();
        bool loadAccountsFromFile(const std::string& filePath) override;
        std::vector<MailInfo> getMailsForUser(const int& userId);
};

#endif