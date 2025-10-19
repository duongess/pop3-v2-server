#include "../common/utils.h"

enum class Status : int {
    OK = 200,
    BadRequest = 400,
    ServerError = 500
};

struct Response {
    Status status;
    std::string data;
    std::string error = "";
};


