#include <string>
#include <nlohmann/json.hpp>
#include "ruleHandler.h"
#include "helper.h"

using json = nlohmann::json;

class ruleProcessor {
  public:
    ruleProcessor() { }

    void processRules(const json& incoming);


    private:
    RuleHandler ruleHandler;
    
};

