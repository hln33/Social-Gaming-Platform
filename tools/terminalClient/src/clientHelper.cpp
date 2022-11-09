#include "clientHelper.h"


using json = nlohmann::json;

json createJSONMessage(std::string type, std::string message){
  json payload = json{{"type", type}, {"message", message}};
  return payload;
}

std::string printMenu(){
  std::stringstream ss;
  ss << "------MENU------- \n";
  ss << "1 - Create \n";
  ss << "2 - Join \n";
  ss << "0 - To close game \n";

  return ss.str();
}

void createRoom(networking::Client* client){
  std::cout << "Enter path to JSON file \n";
  std::string filePath;
  std::cin.ignore();
  std::getline(std::cin, filePath);

  std::ifstream file(filePath);
  while(!file.is_open()) {
    std::cout << "failed to open " << filePath << '\n' << "Enter path to JSON file \n";
    std::getline(std::cin, filePath);
    file = std::ifstream(filePath);
  }
  json gameRules = json::parse(file);
  json payload = createJSONMessage("Create", gameRules.dump());
  client->send(payload.dump());
}

void joinRoom(networking::Client* client){
  std::string code;
  std::string userName;

  std::cout << "Enter Code:" << std::endl;
  std::cin >> code;

  //json payload = createJSONMessage("Join", code);
  json payload = {
    {"type", "join"},
    {"message", {
      {"code", code}
    }}
  };
  client->send(payload.dump());
}