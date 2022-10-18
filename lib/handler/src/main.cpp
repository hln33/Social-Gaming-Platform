#include "../include/handler.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <../json.hpp>

// use this main function to test handler code for now. In the future we will probably use unit tests instead.
int main() {
    std::string test;
    recieveMessage(test);

    //the following code is used on a test JSON input to make sure handler functions are working as intended.
    auto valid_text = R"(
        {
            "numbers": [1, 2, 3]
        }
    )";

    std::cout << std::boolalpha
              << isJSON(valid_text);
}  