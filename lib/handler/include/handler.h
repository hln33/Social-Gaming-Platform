#pragma once
#include <string>
#include <nlohmann/json.hpp>
#include <unordered_map>


enum class parse_event_t : std:: uint8_t{
    object_start,
    object_end,
    array_start,
    array_end,
    key,
    value
};

// struct Config {
//     // for storing key->value for configuration, constant, etc...
//     std::string name;
//     int maxPlayer;
//     int minPlayer;
//     bool allowsAudience;

//     std::unordered_map<std::string, std::string> setup;
// };
struct Config {
    // for storing key->value for configuration, constant, etc...
    std::unordered_map<std::string, std::string> setup;
    
    int getMaxPlayer(){
        if (setup.find("max") != setup.end()){
            return std::stoi(setup["max"]);
        }    
        return -1;
    }

    int getMinPlayer(){
        if (setup.find("min") != setup.end()){
            return std::stoi(setup["min"]);
        }    
        return -1;
    }

    std::string getName(){
        if (setup.find("name") != setup.end()){
            return setup["name"];
        }
        return "";    
    }
};

struct JsonLookup {
    std::unordered_map<std::string, std::string> valueLookUp;
};

struct Pool {
    //Usage: weaponLookUp["name"] = {rock, paper, scissor}, weaponLookUp["beats"] = {scissors, rock, paper}
    std::unordered_map<std::string, std::vector<std::string>> weaponLookUp;
};

struct Constant {
    std::unordered_map<std::string, Pool> lists;

    void printConstant(){
        Pool pool = (*lists.begin()).second; // access the list inside weapon

        std::cout << "=================================\n";
        

        for (auto const &pair: pool.weaponLookUp) {
            std::cout <<"TYPE: " << pair.first << "\n";
            std::cout <<"SIZE: " << pair.second.size() << "\n";
            for (auto const& v : pair.second){
                std::cout << " items = " << v;
            }
            std::cout << "\n";
        }
        std::cout << "=================================\n";

    }
};



using json = nlohmann::json;


void initLogging();

bool isJSON(const std::string& text);

bool isString(const json j_value);

bool isInt(const json j_value);

bool isArray(const json j_value);

bool isBoolean(const json j_value);

json completeParse(const std::string& text);

bool MessageContains(const std::string& string, const std::string& substring);

// void handleConfig(const json j_complete);

// void handleConstant(const json j_complete);

// void handleRules(const json j_complete);

//  void performBusinessLogic(const std::string& message);

bool recieveMessage(std::string& message);

// bool storeParsedValues(json& text);

bool storeParsedValuesRevised(std::string& message);

