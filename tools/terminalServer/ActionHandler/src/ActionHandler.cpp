#include "ActionHandler.h"
#include <iostream>
// #include "controller.h" is in the header

#include <spdlog/spdlog.h>



json createaJSONMessage(std::string type, std::string message){
  json payload = json{{"type", type}, {"message", message}};
  return payload;
}

class JoinAction : public Action {
    private:
        recipientsWrapper executeImpl(json data, Connection sender, Controller& controller) override {
            SPDLOG_INFO("Join Action Detected");

            std::cout << data << "\n";
            std::string roomCode = data.at("code");

            recipientsWrapper wrapper = controller.joinRoom(roomCode, sender);
            wrapper.actionName = "Player joined";

            return wrapper;
        }
};

class QuitAction : public Action {
    private:
        recipientsWrapper executeImpl(json data, Connection sender, Controller& controller) override {
            SPDLOG_INFO("Quit Action Detected");

            std::string placeholder_roomcode = "123";
            std::string roomcode = (std::string) data;
            recipientsWrapper wrapper = controller.leaveRoom(roomcode, sender);
            wrapper.actionName = "Quit";

            return wrapper;
        }
};

class CreateGameAction : public Action {
    private:
        recipientsWrapper executeImpl(json data, Connection sender, Controller& controller) override {
            SPDLOG_INFO("CreateGame Action Detected");

            recipientsWrapper room = controller.createRoom(data, sender);

            createaJSONMessage("Game Created", room.data.message);
            
            if(room.data.status == Status::SUCCESS){
                //return room code to client
                SPDLOG_INFO("Game Created: " + room.data.message);
                room.actionName = "Game Created";
            }
            else{
                SPDLOG_ERROR("Error: room not created");
                room.actionName = "Error";
            }

            return room;
        }
};

// class ShutdownAction : public Action {
//     private:
//         recipientsWrapper executeImpl(json data, Connection sender, Controller& controller) override {
//             SPDLOG_INFO("Shutdown Action Detected");
//             return createaJSONMessage("shutdown", "server shutting down");
//         }
// };

// class EndGameAction : public Action {
//     private:
//         recipientsWrapper executeImpl(json data, Connection sender, Controller& controller) override {
//             SPDLOG_INFO("End Game Action Detected");

//             return createaJSONMessage("close game", "game ended");
//         }
// };

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
    // registerAction("End Game", std::make_unique<EndGameAction>());
    // registerAction("Send Chat", std::make_unique<SendChatAction>());
}