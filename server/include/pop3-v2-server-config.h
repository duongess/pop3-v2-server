#ifndef POP3V2ACCOUNT_H_
#define POP3V2ACCOUNT_H_
#include "serverconfig.h"

class Pop3V2Account: public Account {
protected:
    int userId;
    std::string host;
}