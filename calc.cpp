#include <iostream>
#include <memory>
#include <cmath>
#include "parse.h"

int main()
{
    std::string expr;
    
    std::cout << "Enter an expression:" << std::endl;
    std::getline(std::cin, expr);
    std::cout << "You entered: " << expr << std::endl;
    
    auto syntax_tree = parse(expr);
    std::cout << "This evaluates to " << syntax_tree->evaluate() << "." << std::endl;
    return 0;
}

