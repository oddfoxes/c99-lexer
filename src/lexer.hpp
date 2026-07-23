#include <iostream>
#include <vector>

enum class TokenType
{
    Keyword,
    Identifier,
    Constant,
    StringLiteral,
    Punctuator,
};

enum class LexerState
{
    None, // Nothing
    BlockComment, // Comment block
    LineComment, // Line comment
    String, // String
};

struct Token
{
    TokenType t;
    std::string l;
    int i;
};

std::vector<Token> tokenize(std::string s);
