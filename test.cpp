#include <iostream>
#include <list>
using std::string;

enum class char_category { num, op, space, other };

char_category categorize(char c)
{
    const string num_chars = "0123456789.";
    const string op_chars = "+-*/^";
    const string space_chars = " ";
    if(num_chars.find(c) != string::npos) return char_category::num;
    else if(op_chars.find(c) != string::npos) return char_category::op;
    else if(space_chars.find(c) != string::npos) return char_category::space;
    else return char_category::other;
}

int main()
{
    string changeable = "This string will change.";
    std::cout << changeable << std::endl;
    changeable = string();
    changeable += "And now it has changed.";
    std::cout << changeable << std::endl;
    
    string line;
    std::cout << "Type something:" << std::endl;
    std::getline(std::cin, line);
    
    std::cout << "The first character was ";
    switch(categorize(line.at(0)))
    {
        case char_category::num:
            std::cout << "a numeric character (0-9 or .)." << std::endl;
            break;
        case char_category::op:
            std::cout << "an operator character (such as + or -)." << std::endl;
            break;
        case char_category::space:
            std::cout << "a space." << std::endl;
            break;
        case char_category::other:
            std::cout << "not a numeric or operator character, or a space." << std::endl;
            break;
    }
    
    std::list<string> l;
    string tmp;
    for(int i = 0; i < line.length(); i++)
    {
        tmp += line[i];
        if(i % 2 == 1 || i == line.length() - 1) 
        {
            l.push_back(tmp);
            tmp = string();
        }
    }
    
    std::cout << "In two-character groups, your input was:" << std::endl;
    for(string s : l) std::cout << "\"" << s << "\" ";
    std::cout << std::endl;
}
