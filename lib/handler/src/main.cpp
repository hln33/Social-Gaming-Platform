#include "../include/handler.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <nlohmann/json.hpp>

//*******************************************************************************************************************************
// the code in this file needs to be converted into unit tests in handler/test. See exampleTests.cpp as a reference.
//********************************************************************************************************************************

// use this main function to test handler code for now. In the future we will probably use unit tests instead.
int main() {
    std::string test;
    recieveMessage(test);

    //the following code is used on a test JSON input to make sure handler functions are working as intended.
    auto valid_text = R"(
    "configuration": {
        "name": "Rock, Paper, Scissors",
        "player count": {"min": 2, "max": 4},
        "audience": false,
        "setup": {
      "Rounds": 10
    }
    )";

    // ^ this is the next test case

    std::cout << std::boolalpha
              << isJSON(valid_text)
              << '\n';

    std::cout << completeParse(valid_text)
              << '\n\n';        



    json::parser_callback_t cb = [](int depth, json::parse_event_t event, json& parsed){
        //this can be used to sort through the parser elements after reading
        //in this case, we are trying to exclude the setup portion of the test input from the result
        if(event == json::parse_event_t::key and parsed == json("setup"))
        {
            return false;
        }
        else
        {   
            return true;
        }
    };

    //test case to exclude the rounds value to understand JSON parsing
    json::parser_callback_t cb2 = [](int depth, json::parse_event_t event, json& parsed){
        if(event == json::parse_event_t::value and parsed == json("10"))
        {
            return false;
        }
        else
        {
            return true;
        }
    };

    json filtered = json::parse(valid_text, cb);
    std::cout << filtered
              << '\n';    

    json filtered = json::parse(valid_text, cb2);
    std::cout << filtered
              << '\n';              
}  