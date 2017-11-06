#include <iostream>
#include <vector>
#include <string>
#include "parse.h"

int main(int argc, char *argv[])
{
    std::string expr;
    
    if(argc == 1)
    {
        std::cout << "Enter an expression:" << std::endl;
        std::getline(std::cin, expr);
    }
    else
    {
        std::vector<std::string> args(argv + 1, argv + argc);
        for(std::string arg : args)
        {
            expr += arg;
            expr += ' ';
        }
    }
    
    std::cout << "You entered: " << expr << std::endl;
    auto syntax_tree = parse(expr);
    std::cout << "This evaluates to " << syntax_tree->evaluate() << "." << std::endl;
    return 0;
}

