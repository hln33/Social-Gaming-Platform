#pragma once

#include<string>

struct messageType{
    std::string JOIN = "Join";
    std::string CREATE = "Create";
    std::string DATA = "Data";
    std::string SUCCESS = "Success";
    std::string QUIT = "Quit";
    std::string CLOSE_GAME = "close game";
    std::string SHUTDOWN = "shutdown";
}; 
