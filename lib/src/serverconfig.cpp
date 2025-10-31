#include <string>
#include <iostream>
#include "serverconfig.h"


bool readAttribute(const std::string& strLine, std::string& name, std::string& value)
{
    size_t p = strLine.find("=");
    if(p!=std::string::npos)
    {
        name = strLine.substr(0,p);
        value = strLine.substr(p+1);
        return true;
    }
    else
        return false;
}

Account::Account()
{
}

Account::~Account()
{

}

Account::Account(const std::string& user)
{
    this->username = user;
}

void ServerConfig::addAccount(Account* acc)
{
    std::string username = acc->getUserName();
    if(!username.empty())
    {
        try
        {
            accMap.insert({username,acc});
        }
        catch (std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }
}


ServerConfig::ServerConfig()
{
}

ServerConfig::~ServerConfig()
{
    // remove all account from account Map and delete account object
    removeAllAccount();
}

Account* ServerConfig::getAccount(const std::string& username)
{
    try
    {
        std::unordered_map<std::string,Account*>::const_iterator got = accMap.find(username);
        if (got == accMap.end())
            return NULL;
        else
            return got->second;
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return NULL;
    }
}

bool ServerConfig::isValidUser(const std::string& username)
{
    return !(accMap.find(username)==accMap.end());
}

bool ServerConfig::authenticate(const std::string& username, const std::string& pass)
{
    try
    {
        std::unordered_map<std::string,Account*>::const_iterator it = accMap.find(username);
        if(it==accMap.end())
        {
            return false;
        } else
            return it->second->isValidPassword(pass);
    }
    catch(std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return false;
    }
}

void ServerConfig::removeAccount(const std::string& username)
{
    try
    {
        accMap.erase(username);
    }
    catch(std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void ServerConfig::removeAllAccount()
{
    Account* acc;
    for (auto& x: accMap)
    {
        acc = x.second;
        delete acc;
    }
    accMap.clear();
}


