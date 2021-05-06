//
// Created by 史记 on 2021/5/1.
//

#include <sysy.h>

#include "gen_eeyore.h"

extern void yyerror(const char *s);

EeyoreValue *EeyoreGenerator::generateOn(const SysYCompUnit *ast) {
    for (auto *def:*ast->defs()) {
        def->generateEeyore(this);
    }
    return nullptr;
}


EeyoreValue *EeyoreGenerator::generateOn(const SysYVarDef *ast) {
    if (ast->is_const() && ast->indices()->empty()) {
        if (!ast->init())return logError("const variable must be initialized");
        if (!ast->init()->exp())return logError("scalar variable's init value must be scalar");
        auto *dest = ast->init()->exp()->generateEeyore(this);
        vars.insert(*ast->ident(), dest);
        return dest;
    } else if (ast->indices()->empty()) {
        auto *dest = new EeyoreSymbol('T', T_cnt++);
        vars.insert(*ast->ident(), dest);
        if (!ast->init()) {
            if (!ast->init()->exp())return logError("scalar variable's init value must be scalar");
            inits.push_back(new EeyoreAssignStmt(dest, ast->init()->exp()->generateEeyore(this)));
        }
        return dest;
    }
}

EeyoreValue *EeyoreGenerator::generateOn(const SysYFuncDef *ast) {

}

EeyoreValue *EeyoreGenerator::generateOn(const SysYBinary *ast) {
    int op = ast->op();
    auto lhs = ast->lhs()->generateEeyore(this);
    if (op == LAND || op == LOR) {
        if (lhs->is_const()) {
            int lval = ((EeyoreNumber *) lhs)->val();
            delete lhs;
            if (lval == (op == LOR)) return new EeyoreNumber(bool(lval));
            else return ast->rhs()->generateEeyore(this);
        }
        // todo
        return nullptr;
    }
    auto rhs = ast->rhs()->generateEeyore(this);
    if (not(lhs->is_const() && rhs->is_const())) {
        // todo
        return nullptr;
    }
    int lval = ((EeyoreNumber *) lhs)->val();
    int rval = ((EeyoreNumber *) rhs)->val();
    delete lhs;
    delete rhs;
    int val;
    switch (op) {
        case ADD:
            val = lval + rval;
            break;
        case SUB:
            val = lval - rval;
            break;
        case MUL:
            val = lval * rval;
            break;
        case DIV:
            val = lval / rval;
            break;
        case MOD:
            val = lval % rval;
            break;
        case EQ:
            val = lval == rval;
            break;
        case NEQ:
            val = lval != rval;
            break;
        case LESS:
            val = lval < rval;
            break;
        case GREAT:
            val = lval > rval;
            break;
        case LESSEQ:
            val = lval <= rval;
            break;
        case GREATEQ:
            val = lval >= rval;
            break;
        default:
            return logError("invalid binary operator");
    }
    return new EeyoreNumber(val);
}

EeyoreValue *EeyoreGenerator::generateOn(const SysYUnary *ast) {
    int op = ast->op();
    auto rhs = ast->rhs()->generateEeyore(this);
    if (!rhs->is_const()) {
        //todo
    }
    int rval = ((EeyoreNumber *) rhs)->val();
    int val;
    switch (op) {
        case ADD:
            val = rval;
            break;
        case SUB:
            val = -rval;
            break;
        case LNOT:
            val = !rval;
            break;
        default:
            return logError("invalid unary operator");
    }
    return new EeyoreNumber(val);
}

EeyoreValue *EeyoreGenerator::generateOn(const SysYNumber *ast) {
    return new EeyoreNumber(ast->val());
}

EeyoreValue *SysYCompUnit::generateEeyore(EeyoreGenerator *gen) const { return gen->generateOn(this); }

EeyoreValue *SysYVarDef::generateEeyore(EeyoreGenerator *gen) const { return gen->generateOn(this); }

EeyoreValue *SysYFuncDef::generateEeyore(EeyoreGenerator *gen) const { return gen->generateOn(this); }

EeyoreValue *SysYBinary::generateEeyore(EeyoreGenerator *gen) const { return gen->generateOn(this); }

EeyoreValue *SysYUnary::generateEeyore(EeyoreGenerator *gen) const { return gen->generateOn(this); }

EeyoreValue *SysYNumber::generateEeyore(EeyoreGenerator *gen) const { return gen->generateOn(this); }