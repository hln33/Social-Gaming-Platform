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
    auto another = std::make_unique<ForEachRule>(ForEachRule("testExpression", "testElement"));
    rule.addForEachRule(std::move(another));

    EXPECT_TRUE(ExecutionStatus::WAITING_FOR_INPUT == rule.getRuleAt(0)->execute());

}

TEST(ruleTest, createWhenRule){
    ForEachRule foreach_rule;
    
    WhenRule when_rule;

    auto pair1 = std::make_pair(Condition("testCondition"),Rules());
    auto ptr = std::make_unique<std::pair<Condition, Rules>> (pair1);

    when_rule.addCase(std::move(ptr));

    Condition c = when_rule.getFirstCaseCondition();
    
    EXPECT_TRUE(c.expression == "testCondition");
}