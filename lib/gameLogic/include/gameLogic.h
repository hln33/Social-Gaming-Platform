#ifndef INC_373PROJECT_GAMELOGIC_H
#define INC_373PROJECT_GAMELOGIC_H
// Description: gameLogic class that creates and manages business logic objects


#include<string>
#include <vector>
#include "room.h"
using namespace std;

class GameLogic {
    private:
        std::vector<Room> rooms;
    public:
        GameLogic(std::vector<Room> rooms){}; 
        GameLogic(){};
        std::vector<Room> getAllRooms(){return rooms;}; 
        void addRooms(Room r){
            rooms.push_back(r);
        }
};


#endif //INC_373PROJECT_GAMELOGIC_H