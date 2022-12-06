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
    ruleHandler.processRules(incoming);

    
            // wait for business interface for create rule objects 


}