#include "session.h"
#include "pop3-v2-server-config.h"

class Pop3V2Session : public Session {
    public: 
        Pop3V2Session(TcpSocket& slave, ServerConfig* conf);
        virtual void doUnknown(std::string cmd_argv[], int cmd_argc) override;
        // virtual void reset() override;

        void doUser(std::string cmd_argv[], int cmd_argc);
        void doPass(std::string cmd_argv[], int cmd_argc);
        void doLiss(std::string cmd_argv[], int cmd_argc);
};