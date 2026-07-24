#include "lexer.hpp"
#include <unordered_set>
#include <string>

std::vector<Token> tokenize(std::string s)
{
    static const std::unordered_set<std::string> KEYWORDS = {
        "auto", "break", "case",  "char", "const", "continue", "default", "do", "double", "else",
        "enum", "extern", "float", "for", "goto", "if", "inline", "int", "long", "register",
        "restrict", "return", "short", "signed", "sizeof", "static", "struct", "switch", "typedef", "union",
        "unsigned", "void", "volatile", "while", "_Bool", "_Complex", "_Imaginary",
    };
    // not unordered_set because it needs to be ordered longest to shortest
    static const std::vector<std::string> PUNCTUATORS = {
        "%:%:", "...", "<<=", ">>=", "->", "++", "--", "<<", ">>", "<=", ">=", "==", 
        "!=", "&&", "||", "*=", "/=", "%=", "+=", "-=", "&=", "^=", "|=", "<:", ":>", 
        "<%", "%>", "%:", "##", "[", "]", "(", ")", "{", "}", ".", "&", "*", "+", "-", 
        "~", "!", "/", "%", "<", ">", "?", ":", ";", "=", ",", "#"
    };
    static const std::unordered_set<char> WHITESPACE = {
        ' ', '\t', '\n'
    };

    LexerState state = LexerState::None;
    std::vector<Token> tokens;
    std::string buff = "";
    int i = 0;
    int i0 = 0;

    while (i < s.size())
    {
        // starts comments
        if (state == LexerState::None && s[i] == '/' && i+1<s.size())
        {
            if (s[i+1] == '/')
            {
                state = LexerState::LineComment;
                i += 2;
                continue;
            }
            else if (s[i+1] == '*')
            {
                state = LexerState::BlockComment;
                i += 2;
                continue;
            }
        }

        // line comments
        if (state == LexerState::LineComment)
        {
            if (s[i] == '\n')
                state = LexerState::None;

            i++;
            continue;
        }

        // block comments
        if (state == LexerState::BlockComment)
        {
            if (s[i] == '*' && i+1<s.size() && s[i+1] == '/')
            {
                state = LexerState::None;
                i += 2;
            } else {
                i++;
            }

            continue;
        }

        // string buffers and ends
        if (state == LexerState::String)
        {
            if (s[i] == '\"' && s[i-1] != '\\')
            {
                state = LexerState::None;
                tokens.push_back(Token{
                    t: TokenType::StringLiteral,
                    l: buff,
                    i: i0,
                });
                buff = "";
            } else {
                buff += s[i];
            }

            i++;
            continue;
        }

        if (state == LexerState::None)
        {
            // skip whitespace
            if (WHITESPACE.count(s[i]))
            {
                i++;
                continue;
            }

            // string openers
            if (s[i] == '\"')
            {
                i0 = i;
                state = LexerState::String;
                i++;

                continue;
            }

            // character constants
            if (s[i] == '\'')
            {
                i0 = i;

                while (true)
                {
                    i++;

                    if (s[i]=='\'' || i >= s.size())
                        break;
                }

                tokens.push_back(Token{
                    t: TokenType::Constant,
                    l: "" + s[i-1],
                    i: i0,
                });

                continue;
            }

            // int/float constants
            if (std::isdigit(s[i]) || s[i] == '.')
            {
                i0 = i;

                buff += s[i];

                while (true)
                {
                    i++;

                    if (i < s.size() && std::iswalnum(s[i]) || s[i] == '.')
                    {
                        buff += s[i];
                    } else {
                        break;
                    }
                }

                tokens.push_back(Token{
                    t : TokenType::Constant,
                    l : buff,
                    i : i0,
                });

                buff = "";

                continue;
            }

            // identifiers & keywords
            if (std::isalpha(s[i]) || s[i] == '_')
            {
                i0 = i;

                buff += s[i];
                
                while (true)
                {
                    i++;

                    if (i < s.size() && std::iswalnum(s[i]) || s[i] == '_')
                    {
                        buff += s[i];
                    } else {
                        break;
                    }
                }

                // keyword
                if (KEYWORDS.count(buff))
                {
                    tokens.push_back(Token{
                        t : TokenType::Keyword,
                        l : buff,
                        i : i0,
                    });
                }
                // identifier
                else
                {
                    tokens.push_back(Token{
                        t : TokenType::Identifier,
                        l : buff,
                        i : i0,
                    });
                }

                buff = "";

                continue;
            }
        }

        // punctuators
        bool p_match = false;

        for (std::string p : PUNCTUATORS)
        {
            if (i + p.size() <= s.size() && s.compare(i, p.size(), p) == 0)
            {
                tokens.push_back(Token{
                    t : TokenType::Punctuator,
                    l : p,
                    i : i,
                });

                i += p.size();
                p_match = true;
                break;
            }
        }

        if (p_match)
            continue;

        // loop increment
        i++;
    }

    return tokens;
}
