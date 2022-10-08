/////////////////////////////////////////////////////////////////////////////
//                         Business Logic Layer
//
// This file contains the implementation of the business logic layer
/////////////////////////////////////////////////////////////////////////////

#include "Logic.h"

class Logic{
    public:

        Logic(){
            // Constructor
        };
        ~Logic(){
            // Destructor
        };
        void run(){
            // Run the logic
            // server.run();
        };
        void stop(){
            // Stop the logic
            // server.stop();
        };
        void send(std::string message){
            std::string data;

            // Send a message
        };
        void receive(std::string message){
            std::string response = "Received: " + message;
            //insert Jason reader here
            // Receive a message
        };
        void connect(std::string address, std::string port){
            // player.connect(address, port);
            // Connect to a server
        };
        void disconnect(){
            // Disconnect from a server
            // player.disconnect();
        };
        void setCallback(std::function<void(std::string)> callback){
            // player.setCallback(callback);
            // Set the callback function
        };

    private:
        std::string logics;
        std::string address;
        std::string port;
        std::string message;
        std::string userinfos;
        std::string serverinfos;
        std::string clientinfos;
};
