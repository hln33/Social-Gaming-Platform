/**
 * 
 * Business logic interface 
 * for handler layer
 * 
 * 
*/

#include "room.h"
#include "unique_id.h"

// creates a room
// use the config builder to pass in any configuration
// to the room object that is returned
IRoom createRoom(IRoomConfig config);

// returns a struct where you can set any configuration
// options for the room
IRoomConfig createRoomConfigBuilder();

// If the room doesnt exist it will throw an exception
// or if the room doesnt allow the member to join for 
// whatever reason
IUniqueId registerRoomMember(std::string_view roomCode, 
                             std::string_view memberName);


// Throws exception if room doesnt exist or if member
// doesnt exist
void unregisterRoomMember(std::string_view roomCode, 
                          IUniqueId &memberUid);


// Throws exception if room doesnt exist
void startGame(std::string_view roomCode);

// Throws exception if room doesnt exist
void sendLiveGameData(std::string_view roomCode,
                      JObject data);
