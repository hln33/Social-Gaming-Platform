#include<string>



enum messageType {Join, 
                Create, 
                Data,
                Error, 
                Success,
                Quit
                };

std::string messageType_to_string(messageType type){
    switch(type) {
            case Join:
                return "Join";
            case Create:
                return "Create";
            case Data:
                return "Data";
            case Error:
                return "Error";
            case Success:
                return "Success";
            case Quit:
                return "Quit";
            default:
                return "Invalid message";
    }
}