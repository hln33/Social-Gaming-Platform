#pragma once

#include <map>
#include <any>
#include <string>

// Description: class defines maps that exist for each player of the game
class PerPlayer{
   public:
        std::map<std::string, std::any> perplayers;
        
        PerPlayer(std::map<std::string, std::any> map);
};

