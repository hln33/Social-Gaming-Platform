#include <gtest/gtest.h>
#include <fstream>
#include <handler.h>
#include <nlohmann/json.hpp>


// TEST(exampleTests, DemonstrateGTestMacros) {
//     EXPECT_TRUE(true);
//     EXPECT_TRUE(2 == 2);
// }

// TEST(exampleTests, GetJsonFromCorrectFilePath){
//     json empty = json();
//     json jsonFile = getJsonFromFilePath("test.json");
//     EXPECT_TRUE(jsonFile != empty);
// }

// TEST(exampleTests, GetJsonFromWrongFilePath){
//     json empty = json();
//     json jsonFile = getJsonFromFilePath("wrong_test.json");
//     EXPECT_TRUE(jsonFile == empty);
// }

// TEST(exampleTests, ExtractConfigFromFile) {
//     bool testResult = true;
//     try{
//         json jsonFile = getJsonFromFilePath("test.json");
//         Config config = extractConfig(jsonFile);
        
//     } catch(std::exception& e){
//         testResult = false;
//         std::cout << e.what() << "\n";
//     }
//     EXPECT_TRUE(testResult);
// }

// TEST(exampleTests, ExtractConstantFromFile) {
    
//     bool testResult = true;
//     try{
//         json jsonFile = getJsonFromFilePath("test.json");
//         Constant constant = extractConstant(jsonFile);
//         constant.printConstant();

//     } catch(std::exception& e){
//         testResult = false;
//         std::cout << e.what() << "\n";
//     }
//     EXPECT_TRUE(testResult);
// }
