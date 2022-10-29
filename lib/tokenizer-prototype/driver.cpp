#include <iostream>
#include "bliniLexer.h"

int main() {

    BliniLexer lexer = BliniLexer{"players.elements.collect(player, player.weapon == weapon.beats)"};

    int token = 0;
    do {

        auto tmp = lexer.getNextToken();
        token = tmp.tokenType;
        std::cout << "token-type:" << token << ", token-value:" << tmp.value << "\n";

    } while(token != 0);

}