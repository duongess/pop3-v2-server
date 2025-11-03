#include "session.h"
#include "pop3-v2-server-config.h"

class Pop3V2Session : public Session {
    private:
        Pop3V2Account* account;
        std::string username;
        Pop3V2ServerConfig* pop3V2Conf;
    public: 
        Pop3V2Session(TcpSocket& slave, Pop3V2ServerConfig* conf);
        virtual void doUnknown(std::string cmd_argv[], int cmd_argc) override;
        // virtual void reset() override;
        void disconnect();

        void doUser(std::string cmd_argv[], int cmd_argc);
        void doPass(std::string cmd_argv[], int cmd_argc);
        void doList(std::string cmd_argv[], int cmd_argc);
};