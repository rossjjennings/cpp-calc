#ifndef parse_h
#define parse_h

#include <memory>
#include <optional>
#include <queue>
#include "syntax-tree.h"

std::unique_ptr<node> parse_expression(std::queue<char> &chars, int prec);
std::unique_ptr<node> parse_atom(std::queue<char> &chars);
std::optional<std::unique_ptr<number>> parse_num(std::queue<char> &chars);
std::optional<std::unique_ptr<binary_op>> parse_binary_op(std::queue<char> &chars);
std::optional<std::unique_ptr<unary_op>> parse_unary_op(std::queue<char> &chars);
std::unique_ptr<node> parse(std::string str);

#endif