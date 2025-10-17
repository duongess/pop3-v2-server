#include "tcp.h"

class Pop3V2: public TCP {
    private:
        // login
        void user(std::string& username);
        void pass(std::string& password);
        
}