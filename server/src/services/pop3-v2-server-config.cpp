#include "pop3-v2-server-config.h"

Pop3V2ServerConfig::Pop3V2ServerConfig() {
    if (db.initSchema()) {
        console.log("Load DB \n");
        db.dumpTables();
    }
    this->setTimeOut(1800000);
}

void Pop3V2Account::setUserId(const int userId) {
    this->userId = userId;
}

void Pop3V2Account::setHost(const std::string& host) {
    this->host = host;
}

bool Pop3V2Account::isLocked() {
    return this->locked;
}

void Pop3V2Account::lock() {
    locked = true;
}

void Pop3V2Account::unLock() {
    locked = false;
}

bool Pop3V2ServerConfig::loadAccountsFromFile(const std::string& filePath) {
    return true;
}

bool Pop3V2ServerConfig::createAccount(const std::string& username, const std::string& password) {
    return this->db.user.createUser(username, password);
}

bool Pop3V2ServerConfig::loadAccountsFromDB()
{
    std::vector<SetUser> usersFromDB = this->db.user.getAllUser();

    this->removeAllAccount(); //

    // Lặp qua map lấy từ DB
    for (const SetUser user : usersFromDB) 
    {

        Pop3V2Account* acc = new Pop3V2Account();
        acc->setUserId(user.userId);
        acc->setUserName(user.username);
        acc->setPassword(user.password);
        
        this->addAccount(acc);
    }
    return true;
}

std::vector<MailInfo> Pop3V2ServerConfig::getMailsForUser(const int& userId) {
    return this->db.mail.listMailsForUser(userId);
}