// #pragma once

// #include <unordered_map>
// #include <memory>
// #include <nlohmann/json.hpp>

// #include "Server.h"
// #include "controller.h"

// using json = nlohmann::json;
// using networking::Connection;

// enum ActionType {
//   join,
//   quit,
//   shutdown,
//   create_game,
//   end_game,
//   send_chat
// };

// class Action {
//     public:
//         // Action() = default;
//         // Action(const Action&) = delete;
//         // Action(Action&&) = delete;
//         // virtual ~Action() = default;

//         json execute(json data, Connection sender, Controller& controller) {
//             return executeImpl(data, sender, controller);
//         }

//     private:
//         virtual json executeImpl(json data, Connection sender, Controller& controller) = 0;
// };

// class ActionHandler {
//     public:
//         using ActionPointer = std::unique_ptr<Action>;

//         ActionHandler() {
//             init();
//         }

//         json executeAction(std::string action, json data, Connection sender);

//     private:
//         Controller controller;
//         std::unordered_map<std::string, ActionPointer> actions;

//         void registerAction(std::string action, std::unique_ptr<Action>);
//         void init();
// };