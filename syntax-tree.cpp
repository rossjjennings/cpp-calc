#include "syntax-tree.h"
using std::unique_ptr;
using std::shared_ptr;

//op

op::op(char name, int prec): name(name), prec(prec) {}

unary_op::unary_op(char name, int prec, double (*func_ptr)(double)):
    op(name, prec), function(func_ptr) {}

binary_op::binary_op(char name, int prec, bool right_assoc,
                     double (*func_ptr)(double, double)):
    op(name, prec),
    right_assoc(right_assoc),
    function(func_ptr) {}

//unary

double unary::evaluate() const
{ return (*uop->function)(operand->evaluate()); }

unary::unary(shared_ptr<const unary_op> uop,
             unique_ptr<const node> operand):
    uop(uop), operand(std::move(operand)) {}

//binary

double binary::evaluate() const
{
    return (*bop->function)(left_operand->evaluate(),
                            right_operand->evaluate());
}
    
binary::binary(shared_ptr<const binary_op> bop, 
               unique_ptr<const node> left_operand, 
                unique_ptr<const node> right_operand):
    bop(bop),
    left_operand(std::move(left_operand)),
    right_operand(std::move(right_operand)) {}

//number

double number::evaluate() const { return value; }
    
number::number(double value): value(value) {}
