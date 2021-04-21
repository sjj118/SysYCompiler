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

int FunCallAST::eval(Context *ctx) const {
    // todo
    printf("func call: %s(", ident.c_str());
    for (auto *i:params)printf("%d, ", i->eval(ctx));
    puts(")");
    return 0;
}

int LValAST::eval(Context *ctx) const {
    // todo
    printf("lval: %s", ident.c_str());
    for (auto *i:indices)printf("[%d]", i->eval(ctx));
    puts("");
    return 0;
}

int BlockStmtAST::eval(Context *ctx) const {
    // todo
    for (auto *i:stmts)i->eval(ctx);
    return -1;
}

int ExpStmtAST::eval(Context *ctx) const {
    // todo
    if (exp)printf("%d\n", exp->eval(ctx));
    return -1;
}

int AssignStmtAST::eval(Context *ctx) const {
    // todo
    printf("assign: %s = %d\n", lhs->getIdent().c_str(), rhs->eval(ctx));
    return -1;
}

int IfStmtAST::eval(Context *ctx) const {
    // todo
    if (cond->eval(ctx))then_stmt->eval(ctx);
    else if (else_stmt)else_stmt->eval(ctx);
    return -1;
}

int WhileStmtAST::eval(Context *ctx) const {
    // todo
    while (cond->eval(ctx))stmt->eval(ctx);
    return -1;
}

int ControlStmtAST::eval(Context *ctx) const {
    switch (type) {
        case BREAK:
            puts("break");
            break;
        case CONTINUE:
            puts("continue");
            break;
        case RETURN:
            if (exp)printf("return %d\n", exp->eval(ctx));
            else puts("return");
            break;
    }
    return -1;
}
