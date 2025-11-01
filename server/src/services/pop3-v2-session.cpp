#include "pop3-v2-session.h"

Pop3V2Session::Pop3V2Session(TcpSocket& slave, ServerConfig* conf) : Session(slave, conf) {
    // slave.send("+OK POP3 server ready\r\n");
}

void Pop3V2Session::doUnknown(std::string cmd_argv[], int cmd_argc) {
    slave.send("-ERR Unknown command\r\n");
}

void Pop3V2Session::doUser(std::string cmd_argv[], int cmd_argc) {
    slave.send("+OK OK User");
    
}

void Pop3V2Session::doPass(std::string cmd_argv[], int cmd_argc) {

}

void Pop3V2Session::doLiss(std::string cmd_argv[], int cmd_argc) {
    
}