#include "../include/player_storage.h"
#include "../include/unique_id.h"
#include "../include/room.h"

// Room::Room(IRoomConfig rc, IPlayerStorage ps) : 
//     config{rc}, players{ps} {

// }

// int Room::getNumPlayers() const {
//     return this->players.getNumPlayerRecords();
// }

// bool Room::addPlayer(IPlayer &p) {

//     if (this->config.allow(p)) {

//         this->players.addPlayerRecord(p);

//         return true;
//     }
//     return false;
// }

// IPlayer& Room::getPlayer(IUniqueId &pId) const {
//     auto p = this->players.getPlayerRecord(pId);
//     return p;
// }

// void Room::removePlayer(IPlayer &p) {
//     auto pId = p.getPublicId();
//     this->players.removePlayerRecord(pId);
// }

// RoomConfig::RoomConfig(std::string roomName, int playerMin, int playerMax){
//      this->maxAllowedPlayers = playerMax;
//         this->minAllowedPlayers = playerMin;
//         this->roomName = roomName;
// }

// void RoomConfig::setContext(Room &r) {
//     this->context = r;
// }

// bool RoomConfig::allow(IPlayer &p) const {
//     // return this->maxAllowedPlayers > this->context.getNumPlayers();
//     return false;
// }

// void RoomConfig::parseConfigRules(JObject jsonRules) {
//     auto maxPlayers = jsonRules.get("max-players");
//     this->maxAllowedPlayers = std::stoi(maxPlayers);
// }

