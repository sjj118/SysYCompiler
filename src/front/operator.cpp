//
// Created by 史记 on 2021/5/7.
//

#include "operator.h"
#include "parser.tab.hpp"

#define OPERATORS(e) \
  BINARY_OPERATORS(e) e(LNOT, !)

#define BINARY_OPERATORS(e) \
  e(ADD, +) e(SUB, -) e(MUL, *) e(DIV, /) e(MOD, %) \
  e(EQ, ==) e(NEQ, !=) e(LESS, <) e(GREAT, >) e(LESSEQ, <=) e(GREATEQ, >=) \
  e(LAND, &&) e(LOR, ||)

#define UNARY_OPERATORS(e) \
  e(ADD, +) e(SUB, -) e(LNOT, !)

#define OPERATOR2STRING(i, j)   case i: return #j;

#define BINARY_CALC(i, j)   case i: return lhs j rhs;

#define UNARY_CALC(i, j)    case i: return j rhs;

std::string op2str(int op) {
    switch (op) {
        OPERATORS(OPERATOR2STRING)
        default:
            assert(false);
    }
}

int calc_bin(int op, int lhs, int rhs) {
    switch (op) {
        BINARY_OPERATORS(BINARY_CALC)
        default:
            assert(false);
    }
}

int calc_un(int op, int rhs) {
    switch (op) {
        UNARY_OPERATORS(UNARY_CALC)
        default:
            assert(false);
    }
}