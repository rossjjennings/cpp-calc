#include "syntax-tree.h"
using std::unique_ptr;

//op

op::op(int prec): prec(prec) {}

unary_op::unary_op(int prec, double (*func_ptr)(double)):
    op(prec), function(func_ptr) {}

binary_op::binary_op(int prec, bool right_assoc,
                     double (*func_ptr)(double, double)):
    op(prec),
    right_assoc(right_assoc),
    function(func_ptr) {}

//unary

double unary::evaluate() const
{ return (*uop->function)(operand->evaluate()); }

unary::unary(unique_ptr<const unary_op> uop,
             unique_ptr<const node> operand):
    uop(std::move(uop)), operand(std::move(operand)) {}

//binary

double binary::evaluate() const
{
    return (*bop->function)(left_operand->evaluate(),
                            right_operand->evaluate());
}
    
binary::binary(unique_ptr<const binary_op> bop, 
               unique_ptr<const node> left_operand, 
                unique_ptr<const node> right_operand):
    bop(std::move(bop)),
    left_operand(std::move(left_operand)),
    right_operand(std::move(right_operand)) {}

//number

double number::evaluate() const { return value; }
    
number::number(double value): value(value) {}
