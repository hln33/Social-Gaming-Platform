#include "ruleHandler.h"
#include <iostream>
#include <spdlog/spdlog.h>


// json createaJSONMessage(std::string type, std::string message){
//   json payload = json{{"type", type}, {"message", message}};
//   return payload;
// }

class ForEach : public RuleAction {
    private:
        void executeImpl(json data) override{
            std::cout << "For Each rule \n"; 
            
        }
};

class InputChoice : public RuleAction {
    private:
        void executeImpl(json data) override{
            std::cout << "Input Choice rule \n"; 
            
        }
};

class ParallelFor : public RuleAction {
    private:
        void executeImpl(json data) override{
            std::cout << "ParallelFor rule \n"; 
            
        }
};

class Scores : public RuleAction {
    private:
        void executeImpl(json data) override{
            std::cout << "Scores rule \n"; 
            
        }
};

class GlobalMessage : public RuleAction {
    private:
        void executeImpl(json data) override{
            std::cout << "Global Message rule \n"; 
            
        }
};

class When : public RuleAction {
    private:
        void executeImpl(json data) override{
            std::cout << "When rule \n"; 
            
        }
};

class Extend : public RuleAction {
    private:
        void executeImpl(json data) override{
            std::cout << "Extend rule \n"; 
            
        }
};

class Discard : public RuleAction {
    private:
        void executeImpl(json data) override{
            std::cout << "Discard rule \n"; 
            
        }
};

class Add : public RuleAction {
    private:
        void executeImpl(json data) override{
            std::cout << "Add rule \n"; 
            
        }
};


void RuleHandler::executeRuleAction(std::string ruleType, json data) {    
    auto action = rules.find(ruleType);
    if (action == rules.end()) {
        std::cout << "no rule found \n"; 
        // return createaJSONMessage("Error", "No action found");
    }

    action->second->execute(data);
    // return rule object
}

void RuleHandler::registerRuleAction(std::string ruleType, std::unique_ptr<RuleAction> rule) {
    rules[ruleType] = std::move(rule);
}

void RuleHandler::init() {
    registerRuleAction("foreach", std::make_unique<ForEach>());
    registerRuleAction("scores", std::make_unique<Scores>());
    registerRuleAction("scores", std::make_unique<GlobalMessage>());
    registerRuleAction("when", std::make_unique<When>());
    registerRuleAction("extend", std::make_unique<Extend>());
    registerRuleAction("discard", std::make_unique<Discard>());
    registerRuleAction("parallelfor", std::make_unique<ParallelFor>());
    registerRuleAction("input-choice", std::make_unique<InputChoice>());
    registerRuleAction("add", std::make_unique<Add>());

}