#include "pop3-v2-server.h"


Pop3V2Server::Pop3V2Server(unsigned short localPort)
    : TCPServer(localPort) // Gọi hàm khởi tạo của lớp cha
{
    console.info("POP3 Server is initializing on port ", localPort);
}

void Pop3V2Server::initCmd() {
    
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
        cmd_argc++;
    }

    // check command name and return command ID
    if(cmd_argc>0 && !cmd_argv[0].empty())
    {
        for(int i = 0; i<this->numCmd; i++)
        {
            if(cmd_argv[0] == cmdNameList[i])
                return i ;
        }
    }
    return SERVER_CMD_UNKNOWN;
}


void Pop3V2Server::startNewSession(TcpSocket slave)
{
    // 1. Tạo đối tượng session mới trên heap
    Pop3V2Session* session = new Pop3V2Session(slave, conf);
    
    // 2. Chuẩn bị các biến cục bộ cho vòng lặp
    std::string cmdLine;
    std::string cmdArgv[SERVER_CMD_ARG_NUM]; //
    int cmdArgc;
    unsigned short cmdId;
    int cmdLen;

    try
    {
        // 3. Gửi lời chào (greeting) CỤ THỂ của POP3

        // 4. Bắt đầu vòng lặp chính của session (giống hệt SMTP)
        while(!session->isQuit()) //
        {
            // 5. Đọc lệnh từ client (dùng helper)
            cmdLen = readCmd(slave, cmdLine);
            
            // Nếu recvLine trả về 0 hoặc lỗi, client đã ngắt kết nối
            if (cmdLen <= 0)
                break;

            // 6. Phân tích lệnh (dùng helper)
            cmdId = parseCmd(cmdLine, cmdArgv, cmdArgc);

            // 7. Thực thi lệnh (dùng hàm của TCPServer)
            doCmd(session, cmdId, cmdArgv, cmdArgc); //
        }
        
        // 8. Dọn dẹp khi vòng lặp kết thúc (client thoát bình thường)
        delete session;
        Console::info("Session closed for: ", slave.getRemoteAddress());
    }
    catch(SocketException& e)
    {
        // 9. Dọn dẹp nếu có lỗi socket (client ngắt đột ngột)
        Console::error("SocketException for ", slave.getRemoteAddress(), ": ", e.what());
        delete session;
    }
}
