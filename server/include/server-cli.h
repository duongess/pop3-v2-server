#ifndef SERVERCLI_H_INCLUDED
#define SERVERCLI_H_INCLUDED

#include "cli.h"
#include "pop3-v2-server.h"
#include "utils.h"
#ifdef _WIN32
#include <windows.h>
#endif // _WIN32

// typedef void (SocketCmdLineInterface::*SOCK_CLI_CMD_FUNC)(char*[], int);

class ServerCLI : public CmdLineInterface
{
private:
    Pop3V2Server * pop3V2;;
public:
    ServerCLI();
    ~ServerCLI();
protected:
    virtual void initCmd();
private:
    void doStart(std::string cmd_argv[], int cmd_argc);
    void doStop(std::string cmd_argv[], int cmd_argc);

    void doSignUp(std::string cmd_argv[], int cmd_argc);
    // void doStatus(std::string cmd_argv[], int cmd_argc);
    // void doRestart(std::string cmd_argv[], int cmd_argc);
    void doHelp(std::string cmd_argv[], int cmd_argc);
};

#endif // SERVERCLI_H_INCLUDED
