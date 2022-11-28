#include "ruleHandler.h"
#include <iostream>
#include <spdlog/spdlog.h>

// json createaJSONMessage(std::string type, std::string message){
//   json payload = json{{"type", type}, {"message", message}};
//   return payload;
// }



class ForEach : public RuleAction {
    private:
        ruleHandlerWrapper executeImpl(json data) override{
            std::cout << "For Each rule \n"; 
            json rules = data["rules"];
            
            return ruleHandlerWrapper{"ForEach", rules};
        }
};

class InputChoice : public RuleAction {
    private:
        ruleHandlerWrapper executeImpl(json data) override{
            
            std::cout << "Input Choice rule \n"; 
            json rules = data["rules"];
            
            return ruleHandlerWrapper{"InputChoice", rules};
        }
};

class ParallelFor : public RuleAction {
    private:
        ruleHandlerWrapper executeImpl(json data) override{
            std::cout << "ParallelFor rule \n"; 
            
            json rules = data["rules"];
            
            return ruleHandlerWrapper{"ParallelFor", rules};
        }
};

class Scores : public RuleAction {
    private:
        ruleHandlerWrapper executeImpl(json data) override{
            std::cout << "Scores rule \n"; 
            
            json rules = data["rules"];
            
            return ruleHandlerWrapper{"Scores", rules};
        }
};

class GlobalMessage : public RuleAction {
    private:
        ruleHandlerWrapper executeImpl(json data) override{
            std::cout << "Global Message rule \n"; 
            
            json rules = data["rules"];
            return ruleHandlerWrapper{"GlobalMessage", rules};
        }
};

class When : public RuleAction {
    private:
        ruleHandlerWrapper executeImpl(json data) override{
            std::cout << "When rule \n"; 
            
            json rules = data["rules"];
            
            return ruleHandlerWrapper{"When", rules};
        }
};

class Extend : public RuleAction {
    private:
        ruleHandlerWrapper executeImpl(json data) override{
            std::cout << "Extend rule \n"; 
            
            json rules = data["rules"];
            
            return ruleHandlerWrapper{"Extend", rules};
        }
};

class Discard : public RuleAction {
    private:
        ruleHandlerWrapper executeImpl(json data) override{
            std::cout << "Discard rule \n"; 
            
            json rules = data["rules"];
            
            return ruleHandlerWrapper{"Discard", rules};
        }
};

class Add : public RuleAction {
    private:
        ruleHandlerWrapper executeImpl(json data) override{
            std::cout << "Add rule \n"; 
            
            json rules = data["rules"];
            
            return ruleHandlerWrapper{"Add", rules};
        }
};


void RuleHandler::executeRuleAction(std::string ruleType, json data) {    
    auto action = rules.find(ruleType);
    if (action == rules.end()) {
        std::cout << "no rule found \n"; 
        // return createaJSONMessage("Error", "No action found");
    }


    auto temp = action->second->execute(data);
    
    // std::cout << data.dump() << " executeRuleAction \n" ;

    if(temp.moreRules != nullptr){
        processRules(temp.moreRules);
    }
   
      
    // return rule object
}

void RuleHandler::registerRuleAction(std::string ruleType, std::unique_ptr<RuleAction> rule) {
    rules[ruleType] = std::move(rule);
}

void RuleHandler::init() {
    registerRuleAction("foreach", std::make_unique<ForEach>());
    registerRuleAction("scores", std::make_unique<Scores>());
    registerRuleAction("global-message", std::make_unique<GlobalMessage>());
    registerRuleAction("when", std::make_unique<When>());
    registerRuleAction("extend", std::make_unique<Extend>());
    registerRuleAction("discard", std::make_unique<Discard>());
    registerRuleAction("parallelfor", std::make_unique<ParallelFor>());
    registerRuleAction("input-choice", std::make_unique<InputChoice>());
    registerRuleAction("add", std::make_unique<Add>());
}

void RuleHandler::processRules(const json& incoming){
    std::vector<json> individual_rules = incoming;
    // loop through each individual rule
    // std::cout << incoming.dump();
    std::cout << "---------" << "\n";
    for (auto rule : individual_rules){
        // json parsedMessage = json::parse(rule.text);
        const std::string ruleType = rule["rule"];
        
        this->executeRuleAction(ruleType, rule);
    }
}