#include <gtest/gtest.h>

#include <handler.h>

TEST(exampleTests, DemonstrateGTestMacros) {
    EXPECT_TRUE(true);
    EXPECT_TRUE(2 == 2);

    std::string test;
    recieveMessage(test);
}
/***
 * THESE TESTS WERE MOVED FROM MAIN.CPP TO HERE, AND THEN THE CODE IN MAIN.CPP WAS DELETED.
 * GENERAL RULE: DO NOT PUT TESTS IN MAIN.CPP. PUT THEM IN THE TEST FOLDER.
 */

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