#pragma once
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void recieveMessage(std::string& message);

bool isJSON(const std::string& text);

json completeParse(const std::string& text);

bool MessageContains(const std::string& string, const std::string& substring);

void performBusinessLogic(const std::string& message);

void storeParsedValues(json& text);