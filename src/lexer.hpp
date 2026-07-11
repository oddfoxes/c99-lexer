#include <iostream>
#include <vector>

enum TokenType
{
    TT_KEYWORD,
    TT_IDENT,
    TT_CONST,
    TT_STRLIT,
    TT_PUNC,
};

// Pre-processor token types
enum class PP_TokenType
{
    HeaderName,
    Identifier,
    PPNumber,
    CharConst,
    StringLit,
    Punctuator,
    Unknown,
};

enum LexerState
{
    LS_NONE, // Nothing
    LS_BLOCK_COMM, // Comment block
    LS_LINE_COMM, // Line comment
    LS_STRING, // String
};

struct Token
{
    TokenType t;
    std::string l;
    int i;
};

std::vector<Token> tokenize(std::string s);