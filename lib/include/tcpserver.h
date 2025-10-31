#ifndef SERVER_H__
#define SERVER_H__
// Base class for generic TCP server
#include "tcpserversocket.h"
#include "session.h"
#include <cstdint>

class TCPServer
{
protected:
    ServerConfig* conf;
    TcpServerSocket master;
    bool running;
    bool autoStart;
    bool logging;
    unsigned short port;
    unsigned short numCmd;
    std::string  cmdNameList[SERVER_CMD_MAX_NUM];
    CMD_FUNC cmdDoFunc[SERVER_CMD_MAX_NUM];
    std::string logFileName;
private:

public:
    TCPServer(unsigned short localPort);
    virtual ~TCPServer();
    bool start();
    void stop();
    bool restart();
    bool isRunning();
    bool isAutoStart() {return autoStart;}
    bool getLogging() {return logging;}
    const std::string& getLogFileName() const {return logFileName;}
    virtual bool configServer();
protected:
    void run();
    virtual void startNewSession(TcpSocket slave)=0;
    virtual bool loadServerConfig(const std::string& confFileName);
    void addCmd(const std::string& cmdName,CMD_FUNC f);
    void doCmd(Session* session,uint8_t cmdId, std::string cmdArgv[], int cmdArgc);
    virtual void initCmd();
};

#endif // SERVER_H_INCLUDED
