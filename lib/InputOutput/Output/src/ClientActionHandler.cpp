#include "ClientActionHandler.h"
#include "Response.h"

#include <iostream>

#include <spdlog/spdlog.h>


class JoinAction : public Client::Action {
    private:
        std::string executeImpl(std::string playerName) override {
            std::string message;
            message.append(playerName + " Successfully Joined Room\n");
            message.append("Enter 'start' to start game\n");

            return message;
        }
};

class JoinFailAction : public Client::Action {
    private:
        std::string executeImpl(std::string string) override {
            std::string message;
            message.append("Failed to join game\n");

            return message;
        }
};

class QuitAction : public Client::Action {
    private:
        std::string executeImpl(std::string playerName) override {
            return playerName + " Left the Room\n";
        }
};

class CreateGameAction : public Client::Action {
    private:
        std::string executeImpl(std::string roomCode) override {
            std::string message;
            message.append("Game Created with Code:" + roomCode + "\n");
            message.append("Enter 'start' to start game\n");

            return message;
        }
}; 

class StartGameAction : public Client::Action {
    private:
        std::string executeImpl(std::string string) override {
            std::string message;
            message.append("Game has started\n");

            return message;
        }
};

class StartGameFailAction : public Client::Action {
    private:
        std::string executeImpl(std::string error) override {
            std::string message;
            message.append("Game could not start because: ");
            message.append(error);
            message.append("\nPlease try again\n");

            return message;
        }
};

class ErrorAction : public Client::Action {
    private:
        std::string executeImpl(std::string errorMessage) override {
            return errorMessage;
        }
};

class RequestInputAction : public Client::Action {
    private:
        std::string executeImpl(std::string instructions) override {
            return instructions;
        }
};

class SentInputAction : public Client::Action {
    private:
        std::string executeImpl(std::string message) override {
            return message;
        }
};


std::string Client::ActionHandler::executeAction(std::string type, std::string data) {    
    auto action = actions.find(type);
    if (action == actions.end()) {
        SPDLOG_ERROR("Unknown action: {}", type);
        return "unknown action";
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
    registerAction(ResponseCode::START_GAME_SUCCESS, std::make_unique<StartGameAction>());
    registerAction(ResponseCode::GAME_INPUT_SUCCESS, std::make_unique<SentInputAction>());

    registerAction(ResponseCode::JOIN_GAME_FAIL, std::make_unique<JoinFailAction>());
    registerAction(ResponseCode::START_GAME_FAIL, std::make_unique<StartGameFailAction>());

    registerAction(ResponseCode::ERROR, std::make_unique<ErrorAction>());
}