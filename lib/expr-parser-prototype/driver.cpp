// example of the expression parser prototype
// and the print visitor that prints out the ast

#include <iostream>
#include <memory>

#include "bliniExpressionParser.h"
#include "prettyPrintASTVisitor.h"
#include "expressionVisitor.h"

int main() {

    BliniExprParser parser = BliniExprParser{"players.elements.collect(player, player.weapon == weapon.beats)"};
    // BliniExprParser parser = BliniExprParser{"player.weapon == weapon.beats"};
    // BliniExprParser parser = BliniExprParser{"player.collect()"};

    std::unique_ptr<BliniAST> ast = parser.parse();

    if (ast == nullptr) {
        std:: cout << "null\n";
        return -1;
    }

    // ast->print();

    // PrettyPrinter printer = PrettyPrinter{};
    GameDataStore datastore;
    ExpressionVisitor visitor{datastore};
    
    ast->evaluate(visitor);

    std::cout << "\n";
}
