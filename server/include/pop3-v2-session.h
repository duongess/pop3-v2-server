#include "session.h"

class Pop3V2Session : public Session {
    public:
        Pop3V2Session(TcpSocket& slave, ServerConfig* conf);
        virtual void doUnknown(std::string cmd_argv[], int cmd_argc) override;
        // virtual void reset() override;

    private:

};