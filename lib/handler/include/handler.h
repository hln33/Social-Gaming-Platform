#pragma once

#include <string>
#include <glog/logging.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include "../../gameLogic/include/gameLogic.h"
#include "../../gameLogic/include/player_storage.h"
#include "../../gameLogic/include/room.h"

enum class parse_event_t : std:: uint8_t{
    object_start,
    object_end,
    array_start,
    array_end,
    key,
    value
};

using json = nlohmann::json;


void recieveMessage(std::string& message, GameLogic& gamelogic);
json completeParse(const std::string& text);