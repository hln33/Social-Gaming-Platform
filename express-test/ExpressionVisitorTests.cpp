#include <gtest/gtest.h>
#include <string>
#include <string_view>
#include <memory>
#include <exception>

#include "bliniAST.h"
#include "expressionVisitor.h"


TEST(expressionVisitorTests, SingleVariableTest) {
    // ast
    std::string variableName = "config";
    SingleVariable ast = SingleVariable{&variableName};

    // game data
    GameDataObject variableValue = 0;
    GameDataStore dataStore;

    dataStore.store(variableName, variableValue);

    ExpressionVisitor visitor{dataStore};
    ast.evaluate(visitor);

    EXPECT_TRUE(boost::apply_visitor(are_strict_equals{}, visitor.result, variableValue));
}
