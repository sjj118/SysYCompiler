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
    printf("func call: %s(", ident->c_str());
    for (auto *i:*params)printf("%d, ", i->eval(ctx));
    puts(")");
    return 0;
}

int LValAST::eval(Context *ctx) const {
    // todo
    printf("%s", ident->c_str());
    for (auto *i:*indices)printf("[%d]", i->eval(ctx));
    return 0;
}

int BlockStmtAST::eval(Context *ctx) const {
    // todo
    for (auto *i:*stmts)i->eval(ctx);
    return -1;
}

int ExpStmtAST::eval(Context *ctx) const {
    // todo
    if (exp)printf("%d\n", exp->eval(ctx));
    return -1;
}

int AssignStmtAST::eval(Context *ctx) const {
    // todo
    lhs->eval(ctx);
    printf(" = %d\n", rhs->eval(ctx));
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

int VarDefAST::eval(Context *ctx) const {
    if (is_const)printf("const ");
    printf("int %s", ident->c_str());
    for (auto *i:*indices)if (i)printf("[%d]", i->eval(ctx)); else printf("[]");
    if (init) {
        printf(" = ");
        init->eval(ctx);
    }
    return -1;
}

int FuncDefAST::eval(Context *ctx) const {
    if (is_void)printf("void ");
    else printf("int ");
    printf("%s(", ident->c_str());
    for (auto *i:*params) i->eval(ctx), printf(", ");
    puts("){");
    stmts->eval(ctx);
    puts("}");
    return -1;
}

int InitValAST::eval(Context *ctx) const {
    if (exp)printf("%d", exp->eval(ctx));
    else {
        printf("{");
        for (auto *i:*list) {
            i->eval(ctx);
            printf(", ");
        }
        printf("}");
    }
    return -1;
}

int CompUnitAST::eval(Context *ctx) const {
    for (auto *i:*defs)i->eval(ctx), puts("");
    return -1;
}

int IdentAST::eval(Context *ctx) const {
    // todo
    return -1;
}
