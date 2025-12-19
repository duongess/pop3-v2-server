#ifndef POP3V2ACCOUNT_H_
#define POP3V2ACCOUNT_H_
#include "serverconfig.h"
#include "db.h"
#include "types/db.h"
#include <queue>


class Pop3V2Account: public Account {
public:
    int userId;
    std::queue<MailInfo> liveQueue;

    void setUserId(const int userId);    

    bool isLocked();
    void lock();
    void unLock();

private:
    bool locked = false;
};

class Pop3V2ServerConfig: public ServerConfig {
    private:
        DB db;
        std::map<int, Pop3V2Account*> toIdMap;

    public:
        Pop3V2ServerConfig();
        bool loadAccountsFromDB();
        bool createAccount(const std::string& username, const std::string& password);
        bool loadAccountsFromFile(const std::string& filePath) override;
        std::vector<MailInfo> getMailsForUser(const int& userId);
        void loadMailsToQueue(const int& userId);
        void addMailToUserQueue(const int& userId, const MailInfo& mail);
        int addMailToDB(const Mail& mail);

        bool getAccountLockedStatus(const int& userId);
};

#endif