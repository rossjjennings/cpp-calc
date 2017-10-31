#include <iostream>
#include <list>
using std::string;

struct node
{
    virtual double evaluate() = 0;
};

struct syntax_tree
{
    node root;
};

struct op { int prec; };

struct unary_op : op { double (*function)(double arg); };

struct binary_op : op 
{
    bool right_assoc;
    double (*function)(double left_arg, double right_arg);
};

struct unary : node
{
    unary_op &uop;
    node operand;
    double evaluate()
    { return op->function(operand.evaluate()); }

    unary::unary(unary_op uop, node operand)
    {
    }
};

struct binary : node
{
    binary_op &bop;
    node left_operand;
    node right_operand;
    double evaluate() 
    { return op->function(left_operand.evaluate(), right_operand.evaluate()); }
    

    binary(binary_op bop, node left_operand, node right_operand)
    {
    }
};

struct number : node
{
    double value;
    double evaluate() { return value; }
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

std::list<token> tokenize(string str)
{
    std::list<token> tokens;
    tokenkind state = tokenkind::none;
    string cur_content;
    charcat cat;
    
    for(int i = 0; i < str.length(); )
    {
        char c = str[i];
        cat = categorize(c);
        switch(state)
        {
            case tokenkind::num:
                if(cat == charcat::digit || cat == charcat::dot)
                {
                    cur_content += c;
                    i++;
                }
                else
                {
                    tokens.push_back(token{state, cur_content});
                    cur_content = string();
                    if(cat == charcat::op || cat == charcat::paren)
                        state = tokenkind::op;
                    else if(cat == charcat::other) state = tokenkind::word;
                    else if(cat == charcat::space) state = tokenkind::none;
                }
                break;
            case tokenkind::word:
                if(cat == charcat::other || cat == charcat::digit || cat == charcat::dot)
                {
                    cur_content += c;
                    i++;
                }
                else
                {
                    tokens.push_back(token{state, cur_content});
                    cur_content = string();
                    if(cat == charcat::op || cat == charcat::paren)
                        state = tokenkind::op;
                    else if(cat == charcat::space) state = tokenkind::none;
                }
                break;
            case tokenkind::op:
                if(cur_content.length() > 0)
                {
                    tokens.push_back(token{state, cur_content});
                    cur_content = string();
                }
                if(cat != charcat::op && cat != charcat::paren)
                {
                    state = tokenkind::none;
                    break;
                }
                cur_content += c;
                i++;
                break;
            case tokenkind::none:
                if(cat == charcat::space) i++;
                else if(cat == charcat::digit || cat == charcat::dot)
                    state = tokenkind::num;
                else if(cat == charcat::op || cat == charcat::paren)
                    state = tokenkind::op;
                else if(cat == charcat::other)
                    state = tokenkind::word;
                break;
        }
    }
    
    tokens.push_back(token{state, cur_content});
    return tokens;
}

node parse_expression(std::deque<char> &deq, int prec)
{
    node cur_node = parse_atom(str);
    
    binary_op bop;
    do
    {
        bop = parse_op(deq);
        node expression = 
          parse_expression(bop.right_assoc ? bop.prec : bop.prec + 1)
        cur_node = binary(bop, cur_node, expression)
    }
    while(cur_op.prec >= prec)
    
    return cur_node;
}

node parse_atom(std::deque<char> &deq)
{
}

node parse_num(std::deque<char> &deq)
{
}

node parse_word(std::deque<char> &deq)
{
}

op parse_op(std::deque<char> &deq)
{
    
}

syntax_tree parse(string str)
{
    std::deque<char> deq;
    node root;
    
    for(char c : str) deq.push_back(c);
    return syntax_tree{root};
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

