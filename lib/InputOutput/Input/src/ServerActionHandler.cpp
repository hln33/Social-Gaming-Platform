#include "ServerActionHandler.h"

#include <spdlog/spdlog.h>


json createaJSONMessage(std::string type, std::string message){
  json payload = json{{"type", type}, {"message", message}};
  return payload;
}

class JoinAction : public Server::Action {
    private:
        recipientsWrapper executeImpl(json data, Connection sender, Controller& controller) override {
            SPDLOG_INFO("Join Action Detected");

            std::string roomCode = data.at("code");
            auto res = controller.joinRoom(roomCode, sender);
            return res;
        }
};

class QuitAction : public Server::Action {
    private:
        recipientsWrapper executeImpl(json data, Connection sender, Controller& controller) override {
            SPDLOG_INFO("Quit Action Detected");

            auto res = controller.leaveRoom(sender);
            return res;
        }
};

class CreateGameAction : public Server::Action {
    private:
        recipientsWrapper executeImpl(json data, Connection sender, Controller& controller) override {
            SPDLOG_INFO("CreateGame Action Detected");

            auto res = controller.createRoom(data, sender);            
            return res;
        }
};

class StartGameAction : public Server::Action {
    private:
        recipientsWrapper executeImpl(json data, Connection sender, Controller& controller) override {
            SPDLOG_INFO("StartGame Action Detected");

            auto res = controller.startGame(sender);
            return res;
        }
};

class EndGameAction : public Server::Action {
    private:
        recipientsWrapper executeImpl(json data, Connection sender, Controller& controller) override {
            SPDLOG_INFO("End Game Action Detected");

            auto res = controller.endGame(sender);
            return res;
        }
};

class GameInputAction : public Server::Action {
    private:
        recipientsWrapper executeImpl(json data, Connection sender, Controller& controller) override {
            SPDLOG_INFO("Game Input Action Detected");

            auto res = controller.handleUserInput(sender, data);
            return res;
        }
};


json Server::ActionHandler::executeAction(std::string type, json data, Connection sender, std::set<Connection>& recipients) {    
    auto action = actions.find(type);
    if (action == actions.end()) {
        recipients.insert(sender);
        return createaJSONMessage(ResponseCode::ERROR, "No action found");
    }

    auto wrapper = action->second->execute(data, sender, this->controller);
    recipients = wrapper.recipientList;

    SPDLOG_INFO(wrapper.responseCode);
    return createaJSONMessage(wrapper.responseCode, wrapper.data.message);
}

void Server::ActionHandler::registerAction(std::string type, std::unique_ptr<Action> action) {
    actions[type] = std::move(action);
}

void Server::ActionHandler::init() {
    registerAction("Join", std::make_unique<JoinAction>());
    registerAction("Quit", std::make_unique<QuitAction>());
    // registerAction("Shutdown", std::make_unique<ShutdownAction>());
    registerAction("Create", std::make_unique<CreateGameAction>());
    registerAction("Start Game", std::make_unique<StartGameAction>());
    registerAction("End Game", std::make_unique<EndGameAction>());
    registerAction("Input", std::make_unique<GameInputAction>());
    // registerAction("Send Chat", std::make_unique<SendChatAction>());
}