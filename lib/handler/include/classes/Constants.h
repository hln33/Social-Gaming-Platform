#pragma once

#include <map>
#include <any>
#include <string>
// Description: class defines mapping from names to values
class Constants{
    public: 
        std::map<std::string, std::any> constants; 
        
        Constants(std::map<std::string, std::any> map);
};

