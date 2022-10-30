#include <iostream>
#include <memory>

#include "bliniExpressionParser.h"

int main() {

    BliniExprParser parser = BliniExprParser{"players.elements.collect(player, player.weapon == weapon.beats)"};

    std::unique_ptr<BliniAST> ast = parser.parse();

    ast->print();
}