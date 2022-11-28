#include <iostream>
#include <memory>

#include "bliniExpressionParser.h"
#include "prettyPrintASTVisitor.h"

int main() {

    BliniExprParser parser = BliniExprParser{"players.elements.collect(player, player.weapon == weapon.beats)"};

    std::unique_ptr<BliniAST> ast = parser.parse();

    // ast->print();

    PrettyPrinter printer = PrettyPrinter{};

    ast->evaluate(printer);

    std::cout << "\n";
}