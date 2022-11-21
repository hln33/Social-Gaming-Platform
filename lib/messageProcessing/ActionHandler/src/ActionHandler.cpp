#include "ActionHandler.h"

#include <spdlog/spdlog.h>


json createaJSONMessage(std::string type, std::string message){
  json payload = json{{"type", type}, {"message", message}};
  return payload;
}

class JoinAction : public Action {
    private:
        recipientsWrapper executeImpl(json data, Connection sender, Controller& controller) override {
            SPDLOG_INFO("Join Action Detected");

            std::string roomCode = data.at("code");

            SPDLOG_INFO("Connection:[{}] attempting to join room:{}", sender.id, roomCode);
            auto res = controller.joinRoom(roomCode, sender);
            res.actionName = "Player joined";

            return res;
        }
};

class QuitAction : public Action {
    private:
        recipientsWrapper executeImpl(json data, Connection sender, Controller& controller) override {
            SPDLOG_INFO("Quit Action Detected");

            std::string roomcode = data.at("code");

            SPDLOG_INFO("Connection:[{}] attempting to leave room:", sender.id, roomcode);
            auto res = controller.leaveRoom(sender);
            res.actionName = "Quit";

            return res;
        }
};

class CreateGameAction : public Action {
    private:
        recipientsWrapper executeImpl(json data, Connection sender, Controller& controller) override {
            SPDLOG_INFO("CreateGame Action Detected");

            recipientsWrapper res = controller.createRoom(data, sender);            
            if(res.data.status == Status::SUCCESS) {
                //return room code to client
                SPDLOG_INFO("Game Created: " + res.data.message);
                res.actionName = "Game Created";
            } else {
                SPDLOG_ERROR("Error: room not created");
                res.actionName = "Error";
            }

            return res;
        }
};

class StartGameAction : public Action {
    private:
        recipientsWrapper executeImpl(json data, Connection sender, Controller& controller) override {
            SPDLOG_INFO("StartGame Action Detected");

            auto res = controller.startGame(sender);
            if (res.data.status == Status::SUCCESS) {
                SPDLOG_INFO("Game Started");
                res.actionName = "Game Started";
            } else {
                SPDLOG_ERROR("Error: game not started");
                res.actionName = "Error";
            }

            return res;
        }
};

// class ShutdownAction : public Action {
//     private:
//         recipientsWrapper executeImpl(json data, Connection sender, Controller& controller) override {
//             SPDLOG_INFO("Shutdown Action Detected");
//             return createaJSONMessage("shutdown", "server shutting down");
//         }
// };

class EndGameAction : public Action {
    private:
        recipientsWrapper executeImpl(json data, Connection sender, Controller& controller) override {
            SPDLOG_INFO("End Game Action Detected");

            auto res = controller.endGame(sender);
            if (res.data.status == Status::SUCCESS) {
                SPDLOG_INFO("Game Ended");
                res.actionName = "Game Ended";
            } else {
                SPDLOG_ERROR("Error: game not ended");
                res.actionName = "Error";
            }

            return res;
        }
};

// class SendChatAction : public Action {
//     private:
//         recipientsWrapper executeImpl(json data, Connection sender, Controller& controller) override {
//             SPDLOG_INFO("Send Chat Action Detected");
         
//             return createaJSONMessage("chat", "");
//         }
// };



json ActionHandler::executeAction(std::string type, json data, Connection sender, std::set<Connection>& recipients) {    
    auto action = actions.find(type);
    if (action == actions.end()) {
        return createaJSONMessage("Error", "No action found");
    }

    auto wrapper = action->second->execute(data, sender, this->controller);
    recipients = wrapper.recipientList;

    return createaJSONMessage(wrapper.actionName, wrapper.data.message);

}

void ActionHandler::registerAction(std::string type, std::unique_ptr<Action> action) {
    actions[type] = std::move(action);
}

void ActionHandler::init() {
    registerAction("Join", std::make_unique<JoinAction>());
    registerAction("Quit", std::make_unique<QuitAction>());
    // registerAction("Shutdown", std::make_unique<ShutdownAction>());
    registerAction("Create", std::make_unique<CreateGameAction>());
    registerAction("End Game", std::make_unique<EndGameAction>());
    // registerAction("Send Chat", std::make_unique<SendChatAction>());
}