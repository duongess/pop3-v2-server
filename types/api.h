// TCP
struct Send
{
    /* data */
};


enum class Status : int {
    Continue = 100,
    OK = 200,
    Redirect = 300,
    BadRequest = 400,
    ServerError = 500
};

// enum class PROTOCOL {
//     TCP
// };

struct Response {
    Status status;
    string data;
    string error = "";
};


