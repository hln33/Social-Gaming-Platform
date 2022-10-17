#pragma once
#include <string>

#include "player_storage.h"

#include "unique_id.h"
#include "unique_id_builder.h"

// room.h
// responsibilities:
// - keep track of players
// - store room info 
// - has rules on which players can join
class IRoom {
public:
    // virtual int getNumPlayers() const;

    // virtual bool addPlayer(IPlayer&);

    // virtual void removePlayer(IPlayer&);

    // virtual IPlayer& getPlayer(IUniqueId&) const;

    virtual ~IRoom(){ }
};


class IRoomConfig {
public:
    // virtual void setContext(Room&);

    virtual bool allow(IPlayer&) const{return false;};

    // pure virtual need to have virtual constructor, otherwise cause error
    virtual ~IRoomConfig(){ }

};


// class RoomConfig : public IRoomConfig {
// public:

//     int maxAllowedPlayers;

//     RoomConfig(JObject);

//     virtual void setContext(Room&) override;
//     virtual bool allow(IPlayer&) const override;

// private:
//     Room &context;

//     void parseConfigRules(JObject);
// };


class RoomConfig : public IRoomConfig {
public:

    int maxAllowedPlayers;
    int minAllowedPlayers; 
    std::string roomName;
    RoomConfig(std::string roomName, int playerMin, int playerMax){
        this->maxAllowedPlayers = playerMax;
        this->maxAllowedPlayers = playerMin;
        this->roomName = roomName;
    };


    // virtual void setContext(Room&) override;
    // virtual bool allow(IPlayer&) const override{return false;};

     virtual ~RoomConfig(){ }

private:
    // Room &context;

};

class Room : public IRoom {
    public:
        Room(IRoomConfig config, IPlayerStorage ipstorage){
            this->config = config;
            this->players = ipstorage;
        };

        // int getNumPlayers() const override{return 0;};

        // bool addPlayer(IPlayer&) override{return false;};

        // void removePlayer(IPlayer&) override{};

        // IPlayer& getPlayer(IUniqueId&) const override;

        IRoomConfig getConfig(){
            return config;
        }

        virtual ~Room(){ }
    private:
        IPlayerStorage players;
        IRoomConfig config;
};


class JObject {
public:
    std::string get(std::string_view);
};
