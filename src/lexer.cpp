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
    static const std::vector<std::string> PUNCTUATORS = {
        "%:%:", "...", "<<=", ">>=", "->", "++", "--", "<<", ">>", "<=", ">=", "==", 
        "!=", "&&", "||", "*=", "/=", "%=", "+=", "-=", "&=", "^=", "|=", "<:", ":>", 
        "<%", "%>", "%:", "##", "[", "]", "(", ")", "{", "}", ".", "&", "*", "+", "-", 
        "~", "!", "/", "%", "<", ">", "?", ":", ";", "=", ",", "#"
    };
    static const std::unordered_set<char> WHITESPACE = {
        ' ', '\t', '\n'
    };

    LexerState state = LS_NONE;
    std::vector<Token> tokens;
    std::string buff = "";
    int i = 0;
    int i0 = 0;

    while (i < s.size())
    {
        // starts comments
        if (state == LS_NONE && s[i] == '/' && i+1<s.size())
        {
            if (s[i+1] == '/')
            {
                state = LS_LINE_COMM;
                i += 2;
                continue;
            }
            else if (s[i+1] == '*')
            {
                state = LS_BLOCK_COMM;
                i += 2;
                continue;
            }
        }

        // line comments
        if (state == LS_LINE_COMM)
        {
            if (s[i] == '\n')
                state = LS_NONE;

            i++;
            continue;
        }

        // block comments
        if (state == LS_BLOCK_COMM)
        {
            if (s[i] == '*' && i+1<s.size() && s[i+1] == '/')
            {
                state = LS_NONE;
                i += 2;
            } else {
                i++;
            }

            continue;
        }

        // string buffers and ends
        if (state == LS_STRING)
        {
            if (s[i] == '\"' && s[i-1] != '\\')
            {
                state = LS_NONE;
                tokens.push_back(Token{
                    t: TT_STRLIT,
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

        if (state == LS_NONE)
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
                state = LS_STRING;
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
                    t: TT_CONST,
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
                    t : TT_CONST,
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
                        t : TT_KEYWORD,
                        l : buff,
                        i : i0,
                    });
                }
                // identifier
                else
                {
                    tokens.push_back(Token{
                        t : TT_IDENT,
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
                    t : TT_PUNC,
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