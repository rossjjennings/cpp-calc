#include <iostream>
#include <list>
#include <stack>
#include <queue>
#include <cmath>
#include <optional>
using std::string;

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

struct syntax_tree
{
    const node &root;
    
    syntax_tree(const node &root): root(root) {}
};

struct op 
{
    const int prec;
    
    op(int prec): prec(prec) {}
};

struct unary_op : op 
{
    double (&function)(double);
    
    unary_op(int prec, double (*func_ptr)(double)):
     op(prec), function(*func_ptr) {}
};

struct binary_op : op 
{
    const bool right_assoc;
    double (&function)(double, double);
    
    binary_op(int prec, bool right_assoc, double (*func_ptr)(double, double)):
     op(prec), right_assoc(right_assoc), function(*func_ptr) {}
};

struct unary : node
{
    const unary_op &uop;
    const node &operand;
    double evaluate() const
    { return uop.function(operand.evaluate()); }

    unary(const unary_op &uop, const node &operand):
     uop(uop), operand(operand) {}
};

struct binary : node
{
    const binary_op &bop;
    const node &left_operand;
    const node &right_operand;
    double evaluate() const
    { return bop.function(left_operand.evaluate(), right_operand.evaluate()); }

    binary(const binary_op &bop, const node &left_operand, const node &right_operand):
     left_operand(left_operand), right_operand(right_operand) {}
};

struct number : node
{
    const double value;
    double evaluate() const { return value; }
    
    number(double value): value(value) {}
};

enum class charcat { digit, dot, op, space, paren, other };

charcat categorize(char c)
{
    const string digits = "0123456789";
    const string op_chars = "+-*/^";
    const string paren_chars = "()";
    if(c == ' ') return charcat::space;
    else if(c == '.') return charcat::dot;
    else if(digits.find(c) != string::npos) return charcat::digit;
    else if(op_chars.find(c) != string::npos) return charcat::op;
    else if(paren_chars.find(c) != string::npos) return charcat::paren;
    else return charcat::other;
}

node parse_expression(std::queue<char> &chars, std::stack<op> &ops, std::queue<node> &nodes, int prec)
{
    node cur_node = parse_atom(chars, ops, nodes);
    
    op tmp;
    binary_op bop;
    for(binary_op bop; bop = parse_op(chars); bop.prec >= prec)
    {
        node expression = 
          parse_expression(bop.right_assoc ? bop.prec : bop.prec + 1)
        cur_node = binary(bop, cur_node, expression)
    }
    
    return cur_node;
}

node parse_atom(std::queue<char> &chars, std::stack<op> &ops, std::queue<node> &nodes)
{
    if next is a unary operator
         op = unary(next)
         consume
         q = prec( op )
         t = parse_expression( q )
         return mkNode( op, t )
    else if next = "("
         consume
         t = parse_expression( 0 )
         expect ")"
         return t
    else if next is a v
         t = mkLeaf( next )
         consume
         return t
    else
         error
}

node parse_num(std::queue<char> &chars)
{
}

node parse_word(std::queue<char> &chars)
{
}

std::optional<binary_op> parse_binary_op(std::queue<char> &chars)
{
    switch(chars.front())
    {
        case '+':
            return binary_op(0, false, &add);
            break;
        case '-':
            return binary_op(0, false, &subtract);
            break;
        case '*':
            return binary_op(1, false, &multiply);
            break;
        case '/':
            return binary_op(1, false, &divide);
            break;
        case '^':
            return binary_op(2, true, &exponentiate);
            break;
    }
}

std::optional<unary_op> parse_unary_op(std::queue<char> &chars)
{
    return unary_op(3, &negate);
}

syntax_tree parse(string str)
{
    std::queue<char> chars;
    std::stack<op> ops;
    std::queue<node> nodes;
    node root;
    
    for(char c : str) chars.push(c);
    root = parse_expression(chars, ops, nodes, 0);
    
    expect( end )
    
    return syntax_tree(root);
}

int main()
{
    string expr;
    
    std::cout << "Enter an expression:" << std::endl;
    std::getline(std::cin, expr);
    std::cout << "You entered: " << expr << std::endl;
    string empty;
    std::cout << empty << std::endl;
    
    std::list<token> tokens = tokenize(expr);
    std::cout << "Tokens in your input were:" << std::endl;
    for(token t : tokens)
    {
        std::cout << "\"" << t.content << "\" " << "(";
        switch(t.kind)
        {
            case tokenkind::num:
                std::cout << "num";
                break;
            case tokenkind::word:
                std::cout << "word";
                break;
            case tokenkind::op:
                std::cout << "op";
                break;
            default:
                std::cout << "none";
        }
        std::cout << ") ";
    }
    std::cout << std::endl;
    return 0;
}

