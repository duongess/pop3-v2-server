#include "pop3-v2-server-config.h"

Pop3V2ServerConfig::Pop3V2ServerConfig() {
    if (db.initSchema()) {
        console.log("Load DB \n");
        db.dumpTables();
    }
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

// Trong tệp pop3-v2-server-config.cpp

bool Pop3V2ServerConfig::loadAccountsFromDB()
{
    std::unordered_map<std::string, std::string> usersFromDB = this->db.user.getAllUser();

    this->removeAllAccount(); //

    // Lặp qua map lấy từ DB
    for (const auto& pair : usersFromDB) 
    {
        std::string username = pair.first;
        std::string password = pair.second;

        Pop3V2Account* acc = new Pop3V2Account();
        acc->setUserName(username);
        acc->setPassword(password);
        
        this->addAccount(acc);
    }
    return true;
}

std::vector<MailInfo> Pop3V2ServerConfig::getMailsForUser(const int& userId) {
    return this->db.mail.listMailsForUser(userId);
}