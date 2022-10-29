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

void initLogging();

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

bool recieveMessage(std::string& message);

bool storeParsedValues(json& text);

bool storeParsedValuesRevised(std::string& message);