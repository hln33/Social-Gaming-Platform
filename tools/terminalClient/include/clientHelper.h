#pragma once
#include <iostream>
#include <unistd.h>
#include <fstream>

#include "clientWindow.h"
#include "Client.h"
#include <nlohmann/json.hpp>
#include <iostream>
#include <sstream>


using json = nlohmann::json;

json createJSONMessage(std::string type, std::string message);

std::string printMenu();

void createRoom(networking::Client* client);

void joinRoom(networking::Client* client);