#include "pop3-v2-server.h"


Pop3V2Server::Pop3V2Server(unsigned short localPort)
    : TCPServer(localPort) // Gọi hàm khởi tạo của lớp cha
{
    console.info("POP3 Server is initializing on port ", localPort);
    initCmd();
}

Pop3V2Server::~Pop3V2Server()
{
    console.stopping("POP3 Server is shutting down.");
}

int Pop3V2Server::readCmd(TcpSocket& slave, std::string& cmdLine)
{
    try
    {
        char cmdBuffer[256];
        int byteRead = slave.recvLine(cmdBuffer,256);
        if(byteRead >= 2) // loai bo CRLF (\r\n) o cuoi xau chua lenh
        {
            cmdBuffer[byteRead-2] = 0;
            cmdLine = cmdBuffer;
        }
        return byteRead;
    }
    catch(SocketException&e )
    {
        std::cerr << e.what() << std::endl;
        return -1;
    }
}

unsigned short Pop3V2Server::parseCmd(const std::string& cmdLine, std::string cmd_argv[], int& cmd_argc)
{
    cmd_argc = 0; // so tham so = 0

    // read command arguments into array of string
    cmd_argv[cmd_argc].clear();
    std::istringstream iss(cmdLine);
    while (cmd_argc < SERVER_CMD_ARG_NUM && iss.good())
    {
        iss >> cmd_argv[cmd_argc];
        console.debug(cmd_argv[cmd_argc]);
        cmd_argc++;
    }

    // check command name and return command ID
    if(cmd_argc>0 && !cmd_argv[0].empty())
    {
        for(int i = 0; i<this->numCmd; i++)
        {
            console.debug(cmdNameList[i]);
            if(cmd_argv[0] == cmdNameList[i])
                return i ;
        }
    }
    return SERVER_CMD_UNKNOWN;
}

void Pop3V2Server::initCmd() {
    addCmd("USER", FUNC_CAST(&Pop3V2Session::doUser));
    addCmd("PASS", FUNC_CAST(&Pop3V2Session::doPass));
    addCmd("LIST", FUNC_CAST(&Pop3V2Session::doLiss));
    
}

void Pop3V2Server::startNewSession(TcpSocket slave)
{
    // create new session
    Pop3V2Session* session = new Pop3V2Session(slave,conf);
    std::string cmdLine;
    std::string cmdArgv[SERVER_CMD_ARG_NUM];
    int cmdArgc;  // number of command arguments
    unsigned short cmdId; // ID of command
    int cmdLen; // length of a SMTP command
    try
    {
        while(!session->isQuit())
        {
            // Nhan lenh
            cmdLen = readCmd(slave,cmdLine);
            // Kiem tra lenh
            if(cmdLen <= 0)
                break;
            // Phan tich lenh
            cmdId = parseCmd(cmdLine, cmdArgv, cmdArgc );
            // Thuc hien lenh trong session
            console.debug(cmdId);
            doCmd(session,cmdId,cmdArgv,cmdArgc);
        }
        // session finish
        delete session;
    }
    catch(SocketException&e)
    {
        console.error(e.what());
        delete session;
    }
}

bool Pop3V2Server::configServer() {
    this->conf = new Pop3V2ServerConfig();
    bool loadOK = this->conf->loadAccountsFromDB();
    if (!loadOK) {
        Console::error("Khong load duoc file config tai khoan!");
        return false;
    }
    this->conf->setTimeOut(30);
    return true;
}