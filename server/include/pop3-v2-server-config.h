#ifndef POP3V2ACCOUNT_H_
#define POP3V2ACCOUNT_H_
#include "serverconfig.h"
#include "db.h"

class Pop3V2Account: public Account {
public:
    int userId;
    std::string host;

private:
    

};

class Pop3V2ServerConfig: public ServerConfig {
    private:
        DB db;

    public:
        Pop3V2ServerConfig();
        bool verify(std::string username, std::string password);
        bool loadAccountsFromDB();
        bool loadAccountsFromFile(const std::string& filePath) override;
};

#endif