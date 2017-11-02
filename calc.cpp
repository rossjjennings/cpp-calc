#include <iostream>
#include <memory>
#include <list>
#include <stack>
#include <queue>
#include <cmath>
#include <optional>
#include <stdexcept>
using std::string;
using std::unique_ptr;
using std::make_unique;

double add(double augend, double addend) { return augend + addend; }
double subtract(double minuend, double subtrahend) { return minuend - subtrahend; }
double multiply(double multiplier, double multiplicand) { return multiplier * multiplicand; }
double divide(double dividend, double divisor) { return dividend / divisor; }
double negate(double argument) { return -argument; }
double exponentiate(double base, double exponent) { return std::pow(base, exponent); }

struct node
{
    virtual double evaluate() const = 0;
};

struct op 
{
    const int prec;
    
    op(int prec): prec(prec) {}
};

struct unary_op : op 
{
    double (*function)(double);
    
    unary_op(int prec, double (*func_ptr)(double)):
     op(prec), function(func_ptr) {}
};

struct binary_op : op 
{
    const bool right_assoc;
    double (*function)(double, double);
    
    binary_op(int prec, bool right_assoc, double (*func_ptr)(double, double)):
     op(prec), right_assoc(right_assoc), function(func_ptr) {}
};

struct unary : node
{
    const unary_op &uop;
    const node &operand;
    double evaluate() const
    {
        std::cout << "Evaluating unary node..." << std::endl;
        return (*uop.function)(operand.evaluate());
    }

    unary(const unary_op &uop, const node &operand):
     uop(uop), operand(operand) {}
};

struct binary : node
{
    const binary_op &bop;
    const node &left_operand;
    const node &right_operand;
    double evaluate() const
    {
        std::cout << "Evaluating binary node..." << std::endl;
        return (*bop.function)(left_operand.evaluate(), right_operand.evaluate());
    }

    binary(const binary_op &bop, const node &left_operand, const node &right_operand):
     bop(bop), left_operand(left_operand), right_operand(right_operand) {}
};

struct number : node
{
    const double value;
    double evaluate() const { return value; }
    
    number(double value): value(value) {}
};

unique_ptr<node> parse_expression(std::queue<char> &chars);
unique_ptr<node> parse_atom(std::queue<char> &chars);
std::optional<unique_ptr<number>> parse_num(std::queue<char> &chars);
std::optional<unique_ptr<unary_op>> parse_unary_op(std::queue<char> &chars);
std::optional<unique_ptr<binary_op>> parse_binary_op(std::queue<char> &chars);

unique_ptr<node> parse_expression(std::queue<char> &chars, int prec)
{
    std::queue tmp_q = chars;
    while (!tmp_q.empty())
    {
        auto q_element = tmp_q.front();
        std::cout << q_element;
        tmp_q.pop();
    } 
    std::cout << "'." << std::endl;
    unique_ptr<node> cur_node = parse_atom(chars);
    
    std::optional<unique_ptr<binary_op>> bop;
    while(true)
    {
        bop = parse_binary_op(chars);
        if(!bop || (*bop)->prec < prec)
        {
            break;
        }
        unique_ptr<node> expression = 
          parse_expression(chars, (*bop)->right_assoc ? (*bop)->prec : (*bop)->prec + 1);
        cur_node = make_unique<binary>(*(*bop), *cur_node, *expression);
    }
    
    return cur_node;
}

unique_ptr<node> parse_atom(std::queue<char> &chars)
{
    std::cout << "Parsing atom..." << std::endl;
    unique_ptr<node> cur_node;
    
    if(auto uop = parse_unary_op(chars))
    {
        cur_node = parse_expression(chars, (*uop)->prec);
        return make_unique<unary>(*(*uop), *cur_node);
    }
    else if(chars.front() == '(')
    {
        chars.pop();
        cur_node = parse_expression(chars, 0);
        if(chars.front() != ')') throw std::invalid_argument("Invalid syntax: expected ')'.");
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
    std::cout << "Parsing..." << std::endl;
    std::queue<char> chars;
    unique_ptr<node> root;
    
    for(char c : str) chars.push(c);
    root = parse_expression(chars, 0);
    std::cout << "Returned to parse() call!" << std::endl;
    
    if(!chars.empty()) throw std::invalid_argument("Invalid syntax: expected EOF");
    
    return root;
}

int main()
{
    string expr;
    
    std::cout << "Enter an expression:" << std::endl;
    std::getline(std::cin, expr);
    std::cout << "You entered: " << expr << std::endl;
    
    unique_ptr<node> syntax_tree = parse(expr);
    std::cout << "Returned to main()!" << std::endl;
    std::cout << "This evaluates to " << syntax_tree->evaluate() << "." << std::endl;
    return 0;
}

