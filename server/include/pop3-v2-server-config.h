#ifndef POP3V2ACCOUNT_H_
#define POP3V2ACCOUNT_H_
#include "serverconfig.h"
#include "db.h"
#include "types/db.h"

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

    void setSessionToken(const std::string& token) { this->sessionToken = token; }
    const std::string& getSessionToken() const { return this->sessionToken; }

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
        bool setSessionToken(int userId, const std::string& token);
        // bool validToken(int userId, const std::string& token);
};

#endif