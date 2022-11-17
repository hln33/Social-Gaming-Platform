#include <gtest/gtest.h>
#include <fstream>
#include <Rules.h>
#include <memory>
#include <vector>
#include <nlohmann/json.hpp>



TEST(rulesTest, createForEachRule){
    // outer rule
    ForEachRule rule;
    rule.setListExpression("configuration.Rounds.upfrom(1)");
    rule.setElement("round");
    
    // inner rule
    auto another_foreach_rule = std::make_unique<ForEachRule>();
    another_foreach_rule.get()->setListExpression("test expression");
    another_foreach_rule.get()->setElement("test element");
    
    
    // add inner rule to outer rule
    auto variant_ptr = std::make_unique<RuleVariant>(std::move(another_foreach_rule));
    rule.addRule(std::move(variant_ptr));

    // auto first = rule.list.at(0).get();

    // auto added = std::get<Cases> (first->rulesOrCases);


    EXPECT_TRUE(true);
}

