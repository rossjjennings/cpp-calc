#include <stdexcept>
#include <cmath>
#include "parse.h"

using std::string;
using std::unique_ptr;
using std::shared_ptr;
using std::weak_ptr;
using std::make_unique;
using std::make_shared;

unique_ptr<const node> parse_expression(std::queue<char> &chars, int prec)
{
    unique_ptr<const node> cur_node = parse_atom(chars);
    
    shared_ptr<const binary_op> bop;
    while(true)
    {
        if(!(bop = parse_binary_op(chars)) || bop->prec < prec) break;
        chars.pop();
        auto expression = 
          parse_expression(chars, bop->right_assoc ? bop->prec : bop->prec + 1);
        cur_node = make_unique<const binary>(bop, std::move(cur_node),
                                       std::move(expression));
    }
    
    return cur_node;
}

unique_ptr<const node> parse_atom(std::queue<char> &chars)
{
    unique_ptr<const node> cur_node;
    
    if(auto uop = parse_unary_op(chars))
    {
        chars.pop();
        cur_node = parse_expression(chars, uop->prec);
        return make_unique<const unary>(uop, std::move(cur_node));
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
        return std::move(num);
    else throw std::invalid_argument("Invalid syntax: "
                                     "expected number or parenthesized expression.");
}

unique_ptr<const number> parse_num(std::queue<char> &chars)
{
    const static string numchars = "1234567890.";
    string buf;
    
    if(chars.empty() || numchars.find(chars.front()) == string::npos)
        return nullptr;
    
    while(!chars.empty() && numchars.find(chars.front()) != string::npos)
    {
        buf += chars.front();
        chars.pop();
    }
    
    return make_unique<const number>(std::stod(buf));
}

shared_ptr<const binary_op> parse_binary_op(std::queue<char> &chars)
{
    static double (*add)(double, double) = 
      [](double augend, double addend)
          { return augend + addend; };
    
    static double (*subtract)(double, double) = 
      [](double minuend, double subtrahend)
          { return minuend - subtrahend; };
    
    static double (*multiply)(double, double) = 
      [](double multiplier, double multiplicand)
          { return multiplier * multiplicand; };
    
    static double (*divide)(double, double) = 
      [](double dividend, double divisor)
          { return dividend / divisor; };
    
    static double (*exponentiate)(double, double) = 
      [](double base, double power)
          { return std::pow(base, power); };
    
	static shared_ptr<const binary_op> plus = 
	    make_shared<const binary_op>('+', 0, false, add);
	static shared_ptr<const binary_op> minus =
	    make_shared<const binary_op>('-', 0, false, subtract);
	static shared_ptr<const binary_op> times =
	    make_shared<const binary_op>('*', 1, false, multiply);
	static shared_ptr<const binary_op> divided_by = 
	    make_shared<const binary_op>('/', 1, false, divide);
	static shared_ptr<const binary_op> to_power = 
	    make_shared<const binary_op>('^', 2, true, exponentiate);
	
    if(chars.empty()) return nullptr;
    
    switch(chars.front())
    {
        case '+':
            return plus;
            break;
        case '-':
            return minus;
            break;
        case '*':
            return times;
            break;
        case '/':
            return divided_by;
            break;
        case '^':
            return to_power;
            break;
        default:
            return nullptr;
    }
}

shared_ptr<const unary_op> parse_unary_op(std::queue<char> &chars)
{
    static double (*negate)(double) = 
      [](double argument)
          { return -argument; };
    
	static shared_ptr<const unary_op> negative = 
	    make_shared<const unary_op>('-', 3, negate);
	
    if(chars.empty()) return nullptr;
    
    if(chars.front() == '-') return negative;
    else return nullptr;
}

unique_ptr<const node> parse(string str)
{
    std::queue<char> chars;
    
    for(char c : str)
    {
        if(c != ' ') chars.push(c);
    }
    unique_ptr<const node> root = parse_expression(chars, 0);
    
    if(!chars.empty()) throw std::invalid_argument("Invalid syntax: expected EOF");
    
    return root;
}
