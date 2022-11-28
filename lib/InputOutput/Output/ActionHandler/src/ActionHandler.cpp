#include "ActionHandler.h"
#include <iostream>

#include <spdlog/spdlog.h>


class JoinAction : public Action {
    private:
        json executeImpl(json data) override {
           // print message to chat window
        }
};

class QuitAction : public Action {
    private:
        json executeImpl(json data) override {
            
        }
};

class CreateGameAction : public Action {
    private:
        json executeImpl(json data) override {
            
        }
};

// class ShutdownAction : public Action {
//     private:
//         recipientsWrapper executeImpl(json data) override {
//             SPDLOG_INFO("Shutdown Action Detected");
//             return createaJSONMessage("shutdown", "server shutting down");
//         }
// };

// class EndGameAction : public Action {
//     private:
//         recipientsWrapper executeImpl(json data override {
//             SPDLOG_INFO("End Game Action Detected");

//             return createaJSONMessage("close game", "game ended");
//         }
// };

// class SendChatAction : public Action {
//     private:
//         recipientsWrapper executeImpl(json data) override {
//             SPDLOG_INFO("Send Chat Action Detected");
         
//             return createaJSONMessage("chat", "");
//         }
// };



json ActionHandler::executeAction(std::string type, json data, Connection sender, std::set<Connection>& recipients) {    
    auto action = actions.find(type);
    if (action == actions.end()) {
        //return error;
    }

    auto res = action->second->execute(data);

    return res;
}

void ActionHandler::registerAction(std::string type, std::unique_ptr<Action> action) {
    actions[type] = std::move(action);
}

void ActionHandler::init() {
    registerAction("Join", std::make_unique<JoinAction>());
    registerAction("Quit", std::make_unique<QuitAction>());
    registerAction("Create", std::make_unique<CreateGameAction>());
}