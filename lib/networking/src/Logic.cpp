/////////////////////////////////////////////////////////////////////////////
//                         Business Logic Layer
//
// This file contains the implementation of the business logic layer
/////////////////////////////////////////////////////////////////////////////

#include "Logic.h"

Class Logic{
    public:
        Logic();
        ~Logic();
        void run();
        void stop();
        void send(std::string message);
        void receive(std::string message);
        void connect(std::string address, std::string port);
        void disconnect();
        void setCallback(std::function<void(std::string)> callback);

    
};

