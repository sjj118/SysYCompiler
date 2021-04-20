//
// Created by 史记 on 2021/4/20.
//

#include "ast.h"
#include "parser.hpp"

int BinaryAST::eval(Context *ctx) const {
    switch (op) {
        case ADD:
            return lhs->eval(ctx) + rhs->eval(ctx);
        case SUB:
            return lhs->eval(ctx) - rhs->eval(ctx);
        case MUL:
            return lhs->eval(ctx) * rhs->eval(ctx);
        case DIV:
            return lhs->eval(ctx) / rhs->eval(ctx);
        case MOD:
            return lhs->eval(ctx) % rhs->eval(ctx);
        case EQ:
            return lhs->eval(ctx) == rhs->eval(ctx);
        case NEQ:
            return lhs->eval(ctx) != rhs->eval(ctx);
        case LESS:
            return lhs->eval(ctx) < rhs->eval(ctx);
        case GREAT:
            return lhs->eval(ctx) > rhs->eval(ctx);
        case LESSEQ:
            return lhs->eval(ctx) <= rhs->eval(ctx);
        case GREATEQ:
            return lhs->eval(ctx) >= rhs->eval(ctx);
        case LAND:
            return lhs->eval(ctx) && rhs->eval(ctx);
        case LOR:
            return lhs->eval(ctx) || rhs->eval(ctx);
    }
    assert(false);
}

int UnaryAST::eval(Context *ctx) const {
    switch (op) {
        case ADD:
            return rhs->eval(ctx);
        case SUB:
            return -rhs->eval(ctx);
        case LNOT:
            return !rhs->eval(ctx);
    }
    assert(false);
}

int NumberAST::eval(Context *ctx) const { return val; }
