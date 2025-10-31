#ifndef SERVER_CONF_H_
#define SERVER_CONF_H_
#include <string>
#include <unordered_map>


bool readAttribute(const std::string& strLine, std::string& name, std::string& value);


class Account
{
protected:
    std::string username;
private:
    std::string password;
/*
    std::string name;
    time_t start;
    time_t finish;
    bool loggedin;
    unsigned long numIP;
    std::string ipAddr;
*/
public:
    Account();
    virtual ~Account();
    Account(const std::string& username);
    const std::string& getUserName() const
    {
        return this->username;
    }
    void setUserName(const std::string& username)
    {
        this->username = username;
    }
    void setPassword(const std::string& pass)
    {
        this->password = pass;
    }
    bool isValidPassword(const std::string& pass) const
    {
        return this->password==pass;
    }
/*
    void setStartTime(time_t start)
    {
        this->start = start;
    }
    void setFinishTime(time_t finish)
    {
        this->finish = finish;
    }
    time_t getStartTime()
    {
        return start;
    }
    time_t getFinishTime()
    {
        return finish;
    }
    void setLogin(bool login)
    {
        this->loggedin = login;
    }
    bool isLogin()
    {
        return loggedin;
    }
    void setIpNumber(unsigned long ipNum)
    {
        this->numIP = ipNum;
    }
    unsigned long getIpNumber()
    {
        return numIP;
    }
*/
};

class ServerConfig
{
protected:
    int timeout;
    std::unordered_map<std::string,Account*> accMap;
public:
    ServerConfig();
    virtual ~ServerConfig();
    void addAccount(Account* acc);
    Account* getAccount(const std::string& username);
    bool isValidUser(const std::string& username);
    bool authenticate(const std::string& username, const std::string& password);
    //Account* getAccount(int index);
    void removeAccount(const std::string& username);
    void removeAllAccount();
    void setTimeOut(int t) {timeout = t;}
    int getTimeout() {return timeout;}
    virtual bool loadAccountsFromFile(const std::string& filename) = 0;
};



#endif // SERVER_CONF_H_
