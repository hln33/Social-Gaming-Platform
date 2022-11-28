#pragma once
#include <string>
#include <nlohmann/json.hpp>
#include <unordered_map>
#include <memory>
#include "RulesLogic.h"



using json = nlohmann::json;

struct ruleHandlerWrapper{
    // Rule rule;
    std::string ruleName;
    json moreRules;
};

class RuleAction {
    public:
        ruleHandlerWrapper execute(json data) {
            return executeImpl(data);
        }

    private:
        virtual ruleHandlerWrapper executeImpl(json data) = 0;
};

class RuleHandler {
    public:
        using RulePointer = std::unique_ptr<RuleAction>;

        RuleHandler() {
            init();
        }

        void executeRuleAction(std::string ruleType, json data);
        void processRules(const json& incoming);

    private:
        std::unordered_map<std::string, RulePointer> rules;

        void registerRuleAction(std::string ruleType, std::unique_ptr<RuleAction>);

        
        void init();
};

