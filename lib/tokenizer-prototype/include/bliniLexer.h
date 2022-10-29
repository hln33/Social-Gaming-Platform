
#include <variant>
#include <string>

extern int   yylex();
extern char* yytext;
extern int   yyleng;

//https://stackoverflow.com/questions/8013755/using-lex-generated-source-code-in-another-file
extern void* setUpBuffer(char const* text);
extern void  tearDownBuffer(void* buffer);


struct Token {

    int tokenType;
    std::string value;

};


class BliniLexer {
public:
    BliniLexer(std::string const& str) {
        bufferHandle = setUpBuffer(str.c_str());
    }
    ~BliniLexer() {
        tearDownBuffer(this->bufferHandle);
    }

    Token getNextToken() {
        int v = yylex();

        if (v <= 0) {
            // do error handling here if v == -1
        }

        return Token{v, std::string(yytext, yyleng)};
    }

private:
    void* bufferHandle;
};


