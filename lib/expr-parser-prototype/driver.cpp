#include <iostream>
#include <memory>

#include "bliniExpressionParser.h"
#include "prettyPrintASTVisitor.h"

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

    PrettyPrinter printer = PrettyPrinter{};

    ast->evaluate(printer);

    std::cout << "\n";
}
