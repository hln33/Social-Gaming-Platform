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

    EXPECT_TRUE(ExecutionStatus::WAITING == rule.getRuleAt(0)->execute());

}
