#include <string>

enum Status {
    Success,
    Fail,
};

struct Response {
    Status code;
    std::string message;
};