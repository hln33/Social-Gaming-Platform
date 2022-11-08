#include "ActionHandler.h"

#include <spdlog/spdlog.h>

json createaJSONMessage(std::string type, std::string message){
  json payload = json{{"type", type}, {"message", message}};
  return payload;
}

class JoinAction : public Action {
    private:
        json executeImpl(json data, Connection sender) override {
            SPDLOG_INFO("Join Action Detected");
            // roomManager.join();
            return createaJSONMessage("Player joined", "");
        }
};

class QuitAction : public Action {
    private:
        json executeImpl(json data, Connection sender) override {
            SPDLOG_INFO("Quit Action Detected");
            // roomCode = roomManager.getRoomCode(connection);
            // roomManager.leaveRoom(roomCode, connection)
            return createaJSONMessage("Quit", "Player has left");
        }
};

class CreateGameAction : public Action {
    private:
        json executeImpl(json data, Connection sender) override {
            SPDLOG_INFO("CreateGame Action Detected");
            //roomManager.createRoom(gameRules, host);
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
            // check if sender is allowed to end the game (i.e. if they are a host)

            // if so, find all connections in their room and disconnect them

                // for (auto client: roomClients) {
                //     server.disconnect(client);
                // }

            // remove the room from roomManager
                // this->roomManager.removeRoom(roomCode)

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



json ActionHandler::executeAction(ActionType type, json data, Connection sender) {    
    auto action = actions.find(type);
    if (action == actions.end()) {
        return createaJSONMessage("Error", "No action found");
    }

    return action->second->execute(data,sender);
}

void ActionHandler::registerAction(ActionType type, std::unique_ptr<Action> action) {
    actions[type] = std::move(action);
}

void ActionHandler::init() {
    registerAction(ActionType::join, std::make_unique<JoinAction>());
    registerAction(ActionType::quit, std::make_unique<QuitAction>());
    registerAction(ActionType::shutdown, std::make_unique<ShutdownAction>());
    registerAction(ActionType::create_game, std::make_unique<CreateGameAction>());
    registerAction(ActionType::end_game, std::make_unique<EndGameAction>());
    registerAction(ActionType::send_chat, std::make_unique<SendChatAction>());
}