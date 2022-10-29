%{
#include "tokenDefinitions.h"

%}

ws                          [ \t\n\r\v\f]+
identifier                  [_a-zA-Z][_a-zA-Z0-9]*
int_lit                     [0-9]+

%option noyywrap

%%

\.                          { return T_DOT; }
==                          { return T_EQ; }
false                       { return T_FALSE; }
>=                          { return T_GEQ; }
>                           { return T_GT; }
{int_lit}                   { return T_INTCONSTANT; }
\<=                         { return T_LEQ; }
\(                          { return T_LPAREN; }
\[                          { return T_LSB; }
\<                          { return T_LT; }
!=                          { return T_NEQ; }
!                           { return T_NOT; }
\)                          { return T_RPAREN; }
\]                          { return T_RSB; }
true                        { return T_TRUE; }
,                           { return T_COMMA; }

{ws}                        { } /* ignore whitespace */
{identifier}                { return T_ID; }
.                           { return -1; }

%%

void* setUpBuffer(char const* text)
{
    YY_BUFFER_STATE buffer  = yy_scan_string(text);
    yy_switch_to_buffer(buffer);

    return buffer;
}

void tearDownBuffer(void* buffer)
{
    yy_delete_buffer((YY_BUFFER_STATE)buffer);
}
