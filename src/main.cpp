#include <iostream>
#include <sys/stat.h>
#include "lexer.hpp"
#include <fstream>

bool file_exists(std::string p)
{
    struct stat sb;

    return stat(p.c_str(), &sb) == 0;
}

std::string read_file(std::string p)
{
    std::string t;
    std::string l;
    std::ifstream File(p);

    while (getline(File, l))
    {
        t += l + '\n';
    }

    File.close();

    return t;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "Error: No input file provided. (usage: main.cpp <input_file>)\n";
        return 1;
    }

    if (file_exists(argv[1]) == false)
    {
        std::cout << "Error: Input filepath doesn't exist. (usage: main.cpp <input_file>)\n";
        return 1;
    }

    std::string source = read_file(argv[1]);

    std::vector<Token> tokens = tokenize(source);

    for (Token t : tokens)
    {
        std::string tt = "";

        switch (t.t)
        {
        case 0:
            tt = "keyword";
            break;
        case 1:
            tt = "identifier";
            break;
        case 2:
            tt = "constant";
            break;
        case 3:
            tt = "string literal";
            break;
        case 4:
            tt = "punctuator";
            break;
        default:
            break;
        }

        std::cout << tt << " | " << t.l << " | " << t.i << '\n';
    }

    return 0;
}