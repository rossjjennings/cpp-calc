#ifndef parse_h
#define parse_h

#include <memory>
#include <queue>
#include "syntax-tree.h"

std::unique_ptr<const node> parse_expression(std::queue<char> &chars, int prec);
std::unique_ptr<const node> parse_atom(std::queue<char> &chars);
std::unique_ptr<const number> parse_num(std::queue<char> &chars);
std::shared_ptr<const binary_op> parse_binary_op(std::queue<char> &chars);
std::shared_ptr<const unary_op> parse_unary_op(std::queue<char> &chars);
std::unique_ptr<const node> parse(std::string str);

#endif
