#ifndef POP3V2_SERVER_H
#define POP3V2_SERVER_H

#include "tcpserver.h"
#include "utils.h"
#include "pop3-v2-session.h"

class Pop3V2Server: public TCPServer {
public:
    Pop3V2Server(unsigned short localPort);
    virtual ~Pop3V2Server();
protected:
    virtual void startNewSession(TcpSocket slave) override;
    virtual void initCmd() override;

private:
    int readCmd(TcpSocket& slave, std::string& cmdLine);
    unsigned short parseCmd(const std::string& cmdLine, std::string cmd_argv[], int& cmd_argc);
};
#endif