%{
#include <iostream>
#include <string>
#include <cstdlib>

#include "bliniAST.h"

int yylex(void);
int yyerror(char *); 

BliniAST* bliniASTResult = nullptr;

%}

%union{
    class BliniAST* ast;
    std::string* str;
}

%token T_DOT
%token T_EQ
%token T_FALSE
%token T_GEQ
%token T_GT
%token T_LEQ
%token T_LPAREN
%token T_LSB
%token T_LT
%token T_NEQ
%token T_NOT
%token T_RPAREN
%token T_RSB
%token T_TRUE
%token T_COMMA

%token <str> T_INTCONSTANT
%token <str> T_ID

%type <ast> bool_expression
%type <ast> dot_expression
%type <ast> dot_expression_after
%type <ast> method_call
%type <ast> method_args
%type <ast> constant


%left  T_EQ T_NEQ T_LT T_GT T_LEQ T_GEQ
%right UnaryNot
%left  T_DOT

%%

start: bool_expression { bliniASTResult = $1; }

bool_expression: bool_expression T_EQ bool_expression  { $$ = new EqExpression{$1, $3};  }
               | bool_expression T_NEQ bool_expression { $$ = new NeqExpression{$1, $3}; }
               | T_NOT bool_expression %prec UnaryNot  { $$ = new NotExpression{$2};     }               
               | dot_expression T_GT dot_expression    { $$ = new GtExpression{$1, $3};  }
               | dot_expression T_GEQ dot_expression   { $$ = new GtExpression{$1, $3};  } // change this
               | dot_expression T_LEQ dot_expression   { $$ = new GtExpression{$1, $3};  } // change this
               | dot_expression T_LT dot_expression    { $$ = new GtExpression{$1, $3};  } // change this
               | dot_expression                        { $$ = $1;                        }
               ;

dot_expression: dot_expression T_DOT dot_expression_after { $$ = new DotExpression{$1, $3};   } // dot expression
              | dot_expression T_RSB dot_expression T_LSB { $$ = new IndexExpression{$1, $3}; } // indexing
              | T_RPAREN dot_expression T_LPAREN          { $$ = $2;                          } // parenthesis/precedence
              | T_ID                                      { $$ = new SingleVariable{$1};      } // single variable
              | constant                                  { $$ = $1;                          } // constants
              ;


dot_expression_after: method_call { $$ = $1;                  } // method calls
                    | T_ID        { $$ = new DotProperty{$1}; } // property names
                    ;


method_call: T_ID T_RPAREN method_args T_LPAREN { $$ = new MethodCall{$1, $3};               }
           | T_ID T_RPAREN T_LPAREN             { $$ = new MethodCall{$1, new MethodArgs{}}; }
           ;


method_args: bool_expression T_COMMA method_args 
           { 
                MethodArgs* t = static_cast<MethodArgs*>($3);
                t->addFront($1);
                $$ = t;
           }
           | bool_expression
           { 
                MethodArgs* t = new MethodArgs{};
                t->addFront($1);
                $$ = t;
           }
           ;


constant: T_INTCONSTANT { $$ = new NumberConstant{$1};                 }
        | T_TRUE        { $$ = new NumberConstant{new std::string{}};  }
        | T_FALSE       { $$ = new NumberConstant{new std::string{}};  }
        ;

%%

