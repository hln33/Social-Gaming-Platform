#pragma once
#include <string>
#include <nlohmann/json.hpp>
#include <unordered_map>
#include <memory>

using json = nlohmann::json;

class RuleAction {
    public:
        void execute(json data) {
            return executeImpl(data);
        }

    private:
        virtual void executeImpl(json data) = 0;
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

