#ifndef syntax_tree_h
#define syntax_tree_h

#include <memory>

struct op 
{
    const int prec;
    op(int prec);
};

struct unary_op : op 
{
    double (*function)(double);
    unary_op(int prec, double (*func_ptr)(double));
};

struct binary_op : op 
{
    const bool right_assoc;
    double (*function)(double, double);
    binary_op(int prec, bool right_assoc,
              double (*func_ptr)(double, double));
};

struct node
{
    virtual double evaluate() const = 0;
};

struct unary : node
{
    std::shared_ptr<const unary_op> uop;
    std::unique_ptr<const node> operand;
    double evaluate() const;
    unary(std::shared_ptr<const unary_op> uop,
          std::unique_ptr<const node> operand);
};

struct binary : node
{
    std::shared_ptr<const binary_op> bop;
    std::unique_ptr<const node> left_operand;
    std::unique_ptr<const node> right_operand;
    double evaluate() const;
    binary(std::shared_ptr<const binary_op> bop,
           std::unique_ptr<const node> left_operand, 
           std::unique_ptr<const node> right_operand);
};

struct number : node
{
    const double value;
    double evaluate() const;
    number(double value);
};

#endif
