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

template<typename T>
struct Response {
    Status status;
    T data;
    std::string error = "";
};


