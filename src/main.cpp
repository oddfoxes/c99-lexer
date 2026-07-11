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
    std::vector<Token> tokens;

    // has input file in arguments
    if (argc > 1) {
        if (file_exists(argv[1]) == false) {
            std::cout << "Error: Input filepath doesn't exist. (usage: main.cpp <input_file>)\n";
            
            return 1;
        }

        std::string source = read_file(argv[1]);
        tokens = tokenize(source);
    }
    else {
        std::cout << "No input file argument given. Enter input file:\n";
        bool exists = false;
        std::string p;

        while (exists == false) {
            getline(std::cin, p);

            if (file_exists(p)) {
                exists = true;
            } else {
                std::cout << "Error: Entered filepath doesn't exist.\n";
            }

            std::string source = read_file(p);
            tokens = tokenize(source);
        }
    }

    for (Token t : tokens) {
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
