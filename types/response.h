#include "../common/utils.h"

enum class Status : int {
    Continue = 100,
    OK = 200,
    Redirect = 300,
    BadRequest = 400,
    ServerError = 500
};

struct Response {
    Status status;
    std::string data;
    std::string error = "";
};


