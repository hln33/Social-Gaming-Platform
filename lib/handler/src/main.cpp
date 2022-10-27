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
}  