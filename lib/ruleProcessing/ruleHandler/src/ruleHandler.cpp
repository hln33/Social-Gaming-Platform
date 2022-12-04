#include "ruleHandler.h"
#include <iostream>
#include <spdlog/spdlog.h>

class ForEach : public RuleAction {
    private:
        Rule* executeImpl(json data) override{
            std::cout << "For Each rule \n"; 
            json rules = data["rules"];

            RuleList rulelist;
            Expression expression;

            ForEachRule* rule = new ForEachRule(expression, rulelist);
            
            return rule;
        }
};

class InputChoice : public RuleAction {
    private:
        Rule* executeImpl(json data) override{
            
            std::cout << "Input Choice rule \n"; 
            json rules = data["rules"];
            
            RuleList rulelist;
            Expression expression;

            InputChoiceRule* rule;    
            return rule;
        }
};

class ParallelFor : public RuleAction {
    private:
        Rule* executeImpl(json data) override{
            std::cout << "ParallelFor rule \n"; 
            
            json rules = data["rules"];
            
            RuleList rulelist;
            Expression expression;

            ParallelforRule* rule;    
            return rule;
        }
};

class Scores : public RuleAction {
    private:
        Rule* executeImpl(json data) override{
            std::cout << "Scores rule \n"; 
            
            json rules = data["rules"];
            
            RuleList rulelist;
            Expression expression;

            ScoresRule* rule;    
            return rule;
        }
};

class GlobalMessage : public RuleAction {
    private:
        Rule* executeImpl(json data) override{
            std::cout << "Global Message rule \n"; 
            
            json rules = data["rules"];
            
            RuleList rulelist;
            Expression expression;

            GlobalmessageRule* rule;    
            return rule;
        }
};

class When : public RuleAction {
    private:
        Rule* executeImpl(json data) override{
            std::cout << "When rule \n"; 
            
            json rules = data["rules"];
            
            RuleList rulelist;
            Expression expression;

            WhenRule* rule;    
            return rule;
        }
};

class Extend : public RuleAction {
    private:
        Rule* executeImpl(json data) override{
            std::cout << "Extend rule \n"; 
            
            json rules = data["rules"];
            
            RuleList rulelist;
            Expression expression;

            ExtendRule* rule;    
            return rule;
        }
};

class Discard : public RuleAction {
    private:
        Rule* executeImpl(json data) override{
            std::cout << "Discard rule \n"; 
            
            json rules = data["rules"];
            
            RuleList rulelist;
            Expression expression;

            DiscardRule* rule;    
            return rule;
        }
};

class Add : public RuleAction {
    private:
        Rule* executeImpl(json data) override{
            std::cout << "Add rule \n"; 
            
            json rules = data["rules"];
            
            RuleList rulelist;
            Expression expression;

            AddRule* rule;    
            return rule;
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

    // if(temp.moreRules != nullptr){
    //     processRules(temp.moreRules);

    //     // call the processHelperRules
        
    // }
   
      
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

//return vector of rule*, need to chenge return type
//this function will be called recursively to go down the rule trees
void RuleHandler::processRules(const json& incoming){
    std::vector<json> individual_rules = incoming;
    // loop through each individual rule
    // std::vector<Rule*> ruleList;

    for (auto rule : individual_rules){
        // json parsedMessage = json::parse(rule.text);
        const std::string ruleType = rule["rule"];
        
        this->executeRuleAction(ruleType, rule);
    }

    //return ruleList
}