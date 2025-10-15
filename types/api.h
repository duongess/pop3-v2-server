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

template<typename T>
struct Response {
    Status status;
    // PROTOCOL protocol;
    T data;
    std::string error = "";
};


