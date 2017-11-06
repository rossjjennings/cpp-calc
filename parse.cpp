#include <stdexcept>
#include "arithmetic.h"
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
	static weak_ptr<const binary_op> plus;
	static weak_ptr<const binary_op> minus;
	static weak_ptr<const binary_op> times;
	static weak_ptr<const binary_op> divided_by;
	static weak_ptr<const binary_op> to_power;
	
    if(chars.empty()) return nullptr;
    
    switch(chars.front())
    {
        case '+':
            chars.pop();
            if(plus.expired())
            {
                auto strong_plus = make_shared<const binary_op>('+', 0, false, &add);
                plus = strong_plus;
                return strong_plus;
            }
            else return plus.lock();
            break;
        case '-':
            chars.pop();
            if(minus.expired())
            {
                auto strong_minus = make_shared<const binary_op>('-', 0, false, &subtract);
                minus = strong_minus;
                return strong_minus;
            }
            else return minus.lock();
            break;
        case '*':
            chars.pop();
            if(times.expired())
            {
                auto strong_times = make_shared<const binary_op>('*', 1, false, &multiply);
                times = strong_times;
                return strong_times;
            }
            else return times.lock();
            break;
        case '/':
            chars.pop();
            if(divided_by.expired())
            {
                auto strong_divided_by = make_shared<const binary_op>('/', 1, false, &divide);
                divided_by = strong_divided_by;
                return strong_divided_by;
            }
            else return divided_by.lock();
            break;
        case '^':
            chars.pop();
            if(to_power.expired())
            {
                auto strong_to_power = make_shared<const binary_op>('^', 2, true, &exponentiate);
                to_power = strong_to_power;
                return strong_to_power;
            }
            else return to_power.lock();
            break;
        default:
            return nullptr;
    }
}

shared_ptr<const unary_op> parse_unary_op(std::queue<char> &chars)
{
	static weak_ptr<const unary_op> negative;
	
    if(chars.empty()) return nullptr;
    
    if(chars.front() == '-')
    {
        chars.pop();
        if(negative.expired())
        {
            auto strong_negative = make_shared<const unary_op>('-', 3, &negate);
            negative = strong_negative;
            return strong_negative;
        }
        else return negative.lock();
    }
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
