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

    private:
        String logics;
        String address;
        String port;
        String message;
        String userinfos;
        String serverinfos;
        String clientinfos;
};

Class Rules{
    public:
        Rules();
        ~Rules();
        void run();
        void stop();
        string getJasonAsRules();
        String setRuleToJason();
        String getRuleFromJason();
        bool checkRule();
        bool checkRule(String rule);
        bool checkRule(String rule, String message);
        int getRuleCount();
        String removeAllRule();
        String removeRule(String rule);
        String removeRule(int index);
        String addRule(String rule);
        String addRule(String rule, int index);
        String getRule(int index);
    private:
        String rules;
        String rule;
        String message;
        String userinfos;
        String serverinfos;
        String clientinfos;
        String Infos;
        String PlayerAssociated;
        String PlayerNotAssociated;
        String spectateRule;
        String clientRule;
};
