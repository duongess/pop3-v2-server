#include "common/utils.h"
#include "types/state.h"

class State {
    public:
        AccountState account;
        std::vector<EmailState> email;
        State();
        ~State();
        void setAccountState(const std::string& username, const std::string& host, const std::string& port);
        void addEmailState(int mailId, const std::string& subject, const std::string& body, ssize_t receivedAt);
}