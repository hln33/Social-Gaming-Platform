#pragma once

#include <string>
#include <nlohmann/json.hpp>

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

bool isJSON(const std::string& text);

json completeParse(const std::string& text);