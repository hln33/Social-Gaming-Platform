/////////////////////////////////////////////////////////////////////////////
//                         Business Logic Layer
//
// This file contains the implementation of the business logic layer
/////////////////////////////////////////////////////////////////////////////

#include <deque>
#include <functional>
#include <memory>
#include <unordered_map>
#include <memory>
#include <string>



class Logic{
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

    private:
        std::string logics;
        std::string address;
        std::string port;
        std::string message;
        std::string userinfos;
        std::string serverinfos;
        std::string clientinfos;
};

class Rules{
    public:
        Rules();
        ~Rules();
        void run();
        void stop();
        std::string getJasonAsRules();
        std::string setRuleToJason();
        std::string getRuleFromJason();
        bool checkRule();
        bool checkRule(std::string rule);
        bool checkRule(std::string rule, std::string message);
        int getRuleCount();
        std::string removeAllRule();
        std::string removeRule(std::string rule);
        std::string removeRule(int index);
        std::string addRule(std::string rule);
        std::string addRule(std::string rule, int index);
        std::string getRule(int index);
    private:
        std::string rules;
        std::string rule;
        std::string message;
        std::string userinfos;
        std::string serverinfos;
        std::string clientinfos;
        std::string Infos;
        std::string PlayerAssociated;
        std::string PlayerNotAssociated;
        std::string spectateRule;
        std::string clientRule;
};

class Player{
    public:
        Player();
        ~Player();
        void run();
        void stop();
        void setPlayerName(std::string name);
        void setPlayerId(std::string id);
        void setPlayerPassword(std::string password);
        void setPlayerEmail(std::string email);
        void setPlayerAge(int age);
        void readJason();
        void writeJason();
        void setPlayerInfos(std::string infos);
        void setPlayerInfos(std::string infos, std::string value);

    private:
        std::string player;
        std::string name;
        std::string id;
        std::string password;
        std::string email;
        std::string age;
        std::string infos;
        std::string value;
        std::string ipaddress;
        std::string port;
        
};
