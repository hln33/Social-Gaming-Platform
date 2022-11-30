#pragma once

#include <unordered_map>
#include <memory>
#include <nlohmann/json.hpp>

using json = nlohmann::json;


namespace Client {

enum ActionType {
  // put output action types here
};

class Action {
    public:
        std::string execute(std::string data) {
            return executeImpl(data);
        }

    private:
        virtual std::string executeImpl(std::string data) = 0;
};

class ActionHandler {
    public:
        using ActionPointer = std::unique_ptr<Action>;

        ActionHandler() {
            init();
        }

        std::string executeAction(std::string action, std::string data);

    private:
        std::unordered_map<std::string, ActionPointer> actions;

        void registerAction(std::string action, std::unique_ptr<Action>);
        void init();
};

}