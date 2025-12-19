#include "pop3-v2-server-config.h"

Pop3V2ServerConfig::Pop3V2ServerConfig() {
    if (db.initSchema()) {
        console.log("Load DB \n");
        db.dumpTables();
    }
}

void Pop3V2Account::setUserId(const int userId) {
    this->userId = userId;
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

bool Pop3V2ServerConfig::getAccountLockedStatus(const int& userId) {
    if (this->toIdMap.find(userId) != this->toIdMap.end()) {
        return this->toIdMap[userId]->isLocked();
    }
    return false;
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
        this->toIdMap[user.userId] = acc;
    }
    return true;
}

std::vector<MailInfo> Pop3V2ServerConfig::getMailsForUser(const int& userId) {
    return this->db.mail.listMailsForUser(userId);
}

void Pop3V2ServerConfig::loadMailsToQueue(const int& userId) {
    std::vector<MailInfo> mails = this->db.mail.listMailsForUser(userId);

    for (const MailInfo& mail : mails) {
        console.debug("Loading mail ID: " + std::to_string(mail.mailId) + " Header: " + mail.header);
        this->toIdMap[userId]->liveQueue.push(mail);
    }
}

void Pop3V2ServerConfig::addMailToUserQueue(const int& userId, const MailInfo& mail) {
    this->toIdMap[userId]->liveQueue.push(mail);
}

int Pop3V2ServerConfig::addMailToDB(const Mail& mail) {
    return this->db.mail.addMail(mail);
}