#include <stdexcept>
#include "arithmetic.h"
#include "parse.h"

using std::string;
using std::unique_ptr;
using std::make_unique;

unique_ptr<node> parse_expression(std::queue<char> &chars, int prec)
{
    unique_ptr<node> cur_node = parse_atom(chars);
    
    std::optional<unique_ptr<binary_op>> bop;
    while(true)
    {
        if(chars.front() == ' ')
        {
            chars.pop();
            break;
        }
        
        bop = parse_binary_op(chars);
        if(!bop || (*bop)->prec < prec)
        {
            break;
        }
        unique_ptr<node> expression = 
          parse_expression(chars, (*bop)->right_assoc ? (*bop)->prec : (*bop)->prec + 1);
        cur_node = make_unique<binary>(std::move(*bop), std::move(cur_node),
                                       std::move(expression));
    }
    
    return cur_node;
}

unique_ptr<node> parse_atom(std::queue<char> &chars)
{
    unique_ptr<node> cur_node;
    
    if(auto uop = parse_unary_op(chars))
    {
        cur_node = parse_expression(chars, (*uop)->prec);
        return make_unique<unary>(std::move(*uop), std::move(cur_node));
    }
    else if(chars.front() == '(')
    {
        chars.pop();
        cur_node = parse_expression(chars, 0);
        if(chars.front() != ')') 
            throw std::invalid_argument("Invalid syntax: expected ')'.");
        chars.pop();
        return cur_node;
    }
    else if(auto num = parse_num(chars))
        return std::move(*num);
    else throw std::invalid_argument("Invalid syntax: "
                                     "expected number or parenthesized expression.");
}

std::optional<unique_ptr<number>> parse_num(std::queue<char> &chars)
{
    const static string numchars = "1234567890.";
    string buf;
    
    if(chars.empty() || numchars.find(chars.front()) == string::npos)
        return std::nullopt;
    
    while(!chars.empty() && numchars.find(chars.front()) != string::npos)
    {
        buf += chars.front();
        chars.pop();
    }
    
    return make_unique<number>(std::stod(buf));
}

std::optional<unique_ptr<binary_op>> parse_binary_op(std::queue<char> &chars)
{
    if(chars.empty()) return std::nullopt;
    
    switch(chars.front())
    {
        case '+':
            chars.pop();
            return make_unique<binary_op>(0, false, &add);
            break;
        case '-':
            chars.pop();
            return make_unique<binary_op>(0, false, &subtract);
            break;
        case '*':
            chars.pop();
            return make_unique<binary_op>(1, false, &multiply);
            break;
        case '/':
            chars.pop();
            return make_unique<binary_op>(1, false, &divide);
            break;
        case '^':
            chars.pop();
            return make_unique<binary_op>(2, true, &exponentiate);
            break;
        default:
            return std::nullopt;
    }
}

std::optional<unique_ptr<unary_op>> parse_unary_op(std::queue<char> &chars)
{
    if(chars.empty()) return std::nullopt;
    
    if(chars.front() == '-')
    {
        chars.pop();
        return make_unique<unary_op>(3, &negate);
    }
    else
    {
        return std::nullopt;
    }
}

unique_ptr<node> parse(string str)
{
    std::queue<char> chars;
    unique_ptr<node> root;
    
    for(char c : str) chars.push(c);
    root = parse_expression(chars, 0);
    
    if(!chars.empty()) throw std::invalid_argument("Invalid syntax: expected EOF");
    
    return root;
}
