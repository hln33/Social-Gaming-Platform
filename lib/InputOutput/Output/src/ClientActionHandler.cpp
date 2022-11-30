#include "ClientActionHandler.h"
#include "Response.h"

#include <iostream>

#include <spdlog/spdlog.h>


class JoinAction : public Client::Action {
    private:
        std::string executeImpl(std::string data) override {
           return "";
        }
};

class QuitAction : public Client::Action {
    private:
        std::string executeImpl(std::string data) override {
            return "";
        }
};

class CreateGameAction : public Client::Action {
    private:
        std::string executeImpl(std::string roomCode) override {
            return "Game Created with Code:" + roomCode;
        }
}; 


std::string Client::ActionHandler::executeAction(std::string type, std::string data) {    
    auto action = actions.find(type);
    if (action == actions.end()) {
        SPDLOG_ERROR("Unknown action");
        //return error
    }

    auto res = action->second->execute(data);

    return res;
}

void Client::ActionHandler::registerAction(std::string type, std::unique_ptr<Action> action) {
    actions[type] = std::move(action);
}

void Client::ActionHandler::init() {
    registerAction(ResponseCode::JOIN_GAME_SUCCESS, std::make_unique<JoinAction>());
    registerAction(ResponseCode::LEFT_ROOM_SUCCESS, std::make_unique<QuitAction>());
    registerAction(ResponseCode::CREATE_GAME_SUCCESS, std::make_unique<CreateGameAction>());
}