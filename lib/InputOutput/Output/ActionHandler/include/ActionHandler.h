#pragma once

#include <unordered_map>
#include <memory>
#include <nlohmann/json.hpp>

#include "Server.h"

using json = nlohmann::json;

enum ActionType {
  // put output action types here
};

class Action {
    public:
        json execute(json data) {
            return executeImpl(data);
        }

    private:
        virtual json executeImpl(json data) = 0;
};

class ActionHandler {
    public:
        using ActionPointer = std::unique_ptr<Action>;

        ActionHandler() {
            init();
        }

        json executeAction(std::string action, json data);

    private:
        std::unordered_map<std::string, ActionPointer> actions;

        void registerAction(std::string action, std::unique_ptr<Action>);
        void init();
};