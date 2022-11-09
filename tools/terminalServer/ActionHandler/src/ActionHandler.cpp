#include "ActionHandler.h"

// #include "controller.h" is in the header

#include <spdlog/spdlog.h>

json createaJSONMessage(std::string type, std::string message){
  json payload = json{{"type", type}, {"message", message}};
  return payload;
}

class JoinAction : public Action {
    private:
        json executeImpl(json data, Connection sender) override {
            SPDLOG_INFO("Join Action Detected");

            //std::string placeholder_roomcode = "123";
            std::string roomCode = data["code"];
            controller.joinRoom(roomCode, sender);

            return createaJSONMessage("Player joined", "");
        }
};

class QuitAction : public Action {
    private:
        json executeImpl(json data, Connection sender) override {
            SPDLOG_INFO("Quit Action Detected");

            std::string placeholder_roomcode = "123";
            std::string roomcode = (std::string) data;
            controller.leaveRoom(roomcode, sender);

            return createaJSONMessage("Quit", "Player has left");
        }
};

class CreateGameAction : public Action {
    private:
        json executeImpl(json data, Connection sender) override {
            SPDLOG_INFO("CreateGame Action Detected");

            controller.createRoom(data, sender);

            return createaJSONMessage("Game Created", "");
        }
};

class ShutdownAction : public Action {
    private:
        json executeImpl(json data, Connection sender) override {
            SPDLOG_INFO("Shutdown Action Detected");
            return createaJSONMessage("shutdown", "server shutting down");
        }
};

class EndGameAction : public Action {
    private:
        json executeImpl(json data, Connection sender) override {
            SPDLOG_INFO("End Game Action Detected");

            return createaJSONMessage("close game", "game ended");
        }
};

class SendChatAction : public Action {
    private:
        json executeImpl(json data, Connection sender) override {
            SPDLOG_INFO("Send Chat Action Detected");
         
            return createaJSONMessage("chat", "");
        }
};



json ActionHandler::executeAction(std::string type, json data, Connection sender) {    
    auto action = actions.find(type);
    if (action == actions.end()) {
        return createaJSONMessage("Error", "No action found");
    }

    return action->second->execute(data, sender);
}

void ActionHandler::registerAction(std::string type, std::unique_ptr<Action> action) {
    actions[type] = std::move(action);
}

void ActionHandler::init() {
    registerAction("Join", std::make_unique<JoinAction>());
    registerAction("Quit", std::make_unique<QuitAction>());
    registerAction("Shutdown", std::make_unique<ShutdownAction>());
    registerAction("Create", std::make_unique<CreateGameAction>());
    registerAction("End Game", std::make_unique<EndGameAction>());
    registerAction("Send Chat", std::make_unique<SendChatAction>());
}