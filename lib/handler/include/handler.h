#pragma once

#include <string>
#include <glog/logging.h>
#include <../json.hpp>
#include <iostream>
#include "../../gameLogic/includes/gameLogic.h"


enum class parse_event_t : std:: uint8_t{
    object_start,
    object_end,
    array_start,
    array_end,
    key,
    value
};

using json = nlohmann::json;


void recieveMessage(std::string& message);
json completeParse(const std::string& text);