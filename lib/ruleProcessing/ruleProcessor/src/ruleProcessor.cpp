#include "ruleProcessor.h"

#include <spdlog/spdlog.h>
#include <iostream>

void ruleProcessor::processRules(const json& incoming) {
    // std::cout << incoming.dump();
//   for (auto& rule : incoming) {
//     json parsedMessage = json::parse(rule);

//     const std::string ruleType = parsedMessage["rule"];

//     // json response = ruleHandler.executeRuleAction(ruleType, rule);
//     ruleHandler.executeRuleAction(ruleType, rule);
//   }
  
    std::vector<json> individual_rules = incoming;
    // loop through each individual rule
    for (auto rule : individual_rules){
        std::cout << "something" << "\n";
        // json parsedMessage = json::parse(rule.text);
        const std::string ruleType = rule["rule"];
        ruleHandler.executeRuleAction(ruleType, rule);
    }
            // wait for business interface for create rule objects 


}