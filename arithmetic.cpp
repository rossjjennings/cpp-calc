#include <cmath>
#include "arithmetic.h"

double add(double augend, double addend) 
{ return augend + addend; }

double subtract(double minuend, double subtrahend) 
{ return minuend - subtrahend; }

double multiply(double multiplier, double multiplicand) 
{ return multiplier * multiplicand; }

double divide(double dividend, double divisor) 
{ return dividend / divisor; }

double negate(double argument) 
{ return -argument; }

double exponentiate(double base, double exponent) 
{ return std::pow(base, exponent); }
