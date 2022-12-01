#pragma once

#include <string>
#include <memory>
#include <iostream>

#include "bliniAST.h"


extern int   yylex();
extern int   yyparse();
extern int   yyerror(char *); 

//https://stackoverflow.com/questions/8013755/using-lex-generated-source-code-in-another-file
extern void* setUpBuffer(char const* text);
extern void  tearDownBuffer(void* buffer);

extern BliniAST* bliniASTResult;

class BliniExprParser {
public:
    BliniExprParser(std::string str)  {
        this->bufferHandle = setUpBuffer(str.c_str());
    }

    ~BliniExprParser() {
        tearDownBuffer(this->bufferHandle);
    }


    std::unique_ptr<BliniAST> parse() {
        if (yyparse() >= 1) {
            // handle parse errors here
	        std::cout << "parse error\n";
            return nullptr;
        }

        // get the result
        return std::unique_ptr<BliniAST>(bliniASTResult);
    }

private:

    void* bufferHandle;

};


