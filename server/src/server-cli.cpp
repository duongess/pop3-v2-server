#include "server-cli.h"

ServerCLI::ServerCLI():CmdLineInterface("server>")
{
    console.log("Simple POP3-V2 Server v1.0\n");
    initCmd();
    pop3V2 = new Pop3V2Server(21);
    if(!pop3V2->configServer())
    {
        console.error("Cannot load server configuration");
        delete pop3V2;
        exit(-1);
    }

}
ServerCLI::~ServerCLI()
{
    delete pop3V2;
}

void ServerCLI::initCmd()
{
    addCmd("start",CLI_CAST(&ServerCLI::doStart));
    addCmd("stop",CLI_CAST(&ServerCLI::doStop));
    addCmd("help", CLI_CAST(&ServerCLI::doHelp));
    addCmd("signup", CLI_CAST(&ServerCLI::doSignUp));
    addCmd("addmail", CLI_CAST(&ServerCLI::doAddMail));
    addCmd("addmaildemo", CLI_CAST(&ServerCLI::doAddMailDemo));
}

void ServerCLI::doStart(std::string cmd_argv[], int cmd_argc)
{
    if(pop3V2->isRunning())
        console.running("POP3-V2 server is running!");
    else if(pop3V2->start())
        console.success("POP3-V2 Server started");
    else
        console.error("POP3-V2 Server failed to start");
}

void ServerCLI::doStop(std::string cmd_argv[], int cmd_argc)
{
    pop3V2->stop();
    console.stopping("POP3-V2 Server stopped");
}

void ServerCLI::doHelp(std::string cmd_argv[], int cmd_argc)
{
    console.log("Cac lenh cua chuong trinh: \n");
    console.log("- start               Bat server\n");
    console.log("- stop                Tat server\n");
    console.log("- help                ho tro\n");
    console.log("- signup <username> <password>              tao tai khoan\n");
    console.log("- addmail             them mail\n");
    console.log("- addmaildemo         them mail demo\n");
}

void ServerCLI::doSignUp(std::string cmd_argv[], int cmd_argc) {
    if (this->pop3V2->pop3V2Conf->createAccount(cmd_argv[1], cmd_argv[2])) {
        this->pop3V2->pop3V2Conf->loadAccountsFromDB();
        console.success("Create user: " + cmd_argv[1]);
    } else {
        console.error("Create fail");
    }
}

void ServerCLI::doAddMail(std::string cmd_argv[], int cmd_argc) {
    // Implementation for adding mail
}

void ServerCLI::doAddMailDemo(std::string cmd_argv[], int cmd_argc) {
    // Implementation for adding demo mail
}
