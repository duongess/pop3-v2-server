#include "pop3-v2.h"

static bool verify() {

}

bool Pop3V2::user(std::string& username) {
    this.sendData("USER " + username + "\r\n");
    Response<string> res = this.receiveData();
    return res.status == Status::OK
}

void Pop3V2::pass(std::string& password) {
    this.sendData("PASS " + password + "\r\n")
}