#ifndef POP3V2ACCOUNT_H_
#define POP3V2ACCOUNT_H_
#include "serverconfig.h"
#include "db.h"
#include "types/db.h"
#include <queue>


class Pop3V2Account: public Account {
public:
    int userId;
    std::string host;
    std::string sessionToken;

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
        bool createAccount(const std::string& username, const std::string& password);
        bool loadAccountsFromFile(const std::string& filePath) override;
        std::vector<MailInfo> getMailsForUser(const int& userId);
        void loadMailsToQueue(const int& userId, std::queue<MailInfo>& mailQueue);
        std::vector<MailInfo> getMailsFromQueue(std::queue<MailInfo>& mailQueue);
};

#endif