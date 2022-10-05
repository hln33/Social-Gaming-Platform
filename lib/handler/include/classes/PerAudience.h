#pragma once

#include <map>
#include <any>
#include <string>
// Description: class defines maps that exist for each audience of the game
class PerAudience{
    public: 
        std::map<std::string, std::any> peraudiences; 
        
        PerAudience(std::map<std::string, std::any> map);
};

