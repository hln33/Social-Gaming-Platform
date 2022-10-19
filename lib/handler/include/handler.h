#pragma once
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

bool isJSON(const std::string& text);

bool isString(const json j_value);

bool isInt(const json j_value);

bool isArray(const json j_value);

bool isBoolean(const json j_value);

json completeParse(const std::string& text);

bool MessageContains(const std::string& string, const std::string& substring);

void handleConfig(const json j_complete);

void handleConstant(const json j_complete);

void handleRules(const json j_complete);

void performBusinessLogic(const std::string& message);

void recieveMessage(std::string& message);

void storeParsedValues(json& text);
