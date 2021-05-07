//
// Created by 史记 on 2021/5/1.
//

#include <sysy.h>

#include "gen_eeyore.h"
#include "operator.h"

std::shared_ptr<EeyoreValue> EeyoreGenerator::generateOn(const SysYBinary *ast) {
    int op = ast->op();
    auto lhs = ast->lhs()->genEeyore(this);
    if (op == LAND || op == LOR) {
        auto lnum = std::dynamic_pointer_cast<EeyoreNumber>(lhs);
        if (lnum) {
            if (lnum->num() == (op == LOR)) return std::make_shared<EeyoreNumber>(bool(lnum->num()));
            else return ast->rhs()->genEeyore(this);
        }
        if (!func)return logError("global variable initialization must be constexpr");
        auto label = std::make_shared<EeyoreLabel>(l_cnt++);
        auto dst = std::make_shared<EeyoreTempSymbol>(t_cnt++);
        func->push_stmt(std::make_shared<EeyoreAssignStmt>(dst, lhs));
        func->push_stmt(std::make_shared<EeyoreIfStmt>(
                op == LAND ? EQ : NEQ, lhs, std::make_shared<EeyoreNumber>(0), label));
        auto rhs = ast->rhs()->genEeyore(this);
        func->push_stmt(std::make_shared<EeyoreAssignStmt>(dst, rhs));
        func->push_stmt(std::make_shared<EeyoreLabelStmt>(label));
        return dst;
    }
    auto rhs = ast->rhs()->genEeyore(this);
    auto lnum = std::dynamic_pointer_cast<EeyoreNumber>(lhs);
    auto rnum = std::dynamic_pointer_cast<EeyoreNumber>(rhs);
    if (!(lnum && rnum)) {
        if (!func)return logError("global variable initialization must be constexpr");
        auto dst = std::make_shared<EeyoreTempSymbol>(t_cnt++);
        func->push_var(std::make_shared<EeyoreDeclaration>(dst));
        func->push_stmt(std::make_shared<EeyoreBinaryStmt>(op, dst, lhs, rhs));
        return dst;
    }
    return std::make_shared<EeyoreNumber>(calc_bin(op, lnum->num(), rnum->num()));
}

std::shared_ptr<EeyoreValue> EeyoreGenerator::generateOn(const SysYUnary *ast) {
    int op = ast->op();
    auto rhs = ast->rhs()->genEeyore(this);
    auto rnum = std::dynamic_pointer_cast<EeyoreNumber>(rhs);
    if (!rnum) {
        if (!func)return logError("global variable initialization must be constexpr");
        if (op == ADD) {
            return rhs;
        } else {
            auto dst = std::make_shared<EeyoreTempSymbol>(t_cnt++);
            func->push_var(std::make_shared<EeyoreDeclaration>(dst));
            func->push_stmt(std::make_shared<EeyoreUnaryStmt>(op, dst, rhs));
            return dst;
        }
    }
    return std::make_shared<EeyoreNumber>(calc_un(op, rnum->num()));
}

std::shared_ptr<EeyoreValue> EeyoreGenerator::generateOn(const SysYNumber *ast) {
    return std::make_shared<EeyoreNumber>(ast->num());
}

std::shared_ptr<EeyoreValue> EeyoreGenerator::generateOn(const SysYFunCall *ast) {
    return nullptr;
}

std::shared_ptr<EeyoreValue> EeyoreGenerator::generateOn(const SysYLVal *ast) {
    return nullptr;
}

std::shared_ptr<EeyoreValue> EeyoreGenerator::generateOn(const SysYBlockStmt *ast) {
    return nullptr;
}

std::shared_ptr<EeyoreValue> EeyoreGenerator::generateOn(const SysYExpStmt *ast) {
    return nullptr;
}

std::shared_ptr<EeyoreValue> EeyoreGenerator::generateOn(const SysYAssignStmt *ast) {
    return nullptr;
}

std::shared_ptr<EeyoreValue> EeyoreGenerator::generateOn(const SysYIfStmt *ast) {
    return nullptr;
}

std::shared_ptr<EeyoreValue> EeyoreGenerator::generateOn(const SysYWhileStmt *ast) {
    return nullptr;
}

std::shared_ptr<EeyoreValue> EeyoreGenerator::generateOn(const SysYControlStmt *ast) {
    return nullptr;
}

std::shared_ptr<EeyoreValue> EeyoreGenerator::generateOn(const SysYInitVal *ast) {
    return nullptr;
}

std::shared_ptr<EeyoreValue> EeyoreGenerator::generateOn(const SysYVarDef *ast) {
    return nullptr;
}

std::shared_ptr<EeyoreValue> EeyoreGenerator::generateOn(const SysYFuncDef *ast) {
    func = new EeyoreFunc("f_" + ast->ident(), ast->params().size());
    return nullptr;
}

std::shared_ptr<EeyoreValue> EeyoreGenerator::generateOn(const SysYCompUnit *ast) {
    return nullptr;
}


std::shared_ptr<EeyoreValue> SysYBinary::genEeyore(EeyoreGenerator *gen) const { return gen->generateOn(this); }

std::shared_ptr<EeyoreValue> SysYUnary::genEeyore(EeyoreGenerator *gen) const { return gen->generateOn(this); }

std::shared_ptr<EeyoreValue> SysYNumber::genEeyore(EeyoreGenerator *gen) const { return gen->generateOn(this); }

std::shared_ptr<EeyoreValue> SysYFunCall::genEeyore(EeyoreGenerator *gen) const { return gen->generateOn(this); }

std::shared_ptr<EeyoreValue> SysYLVal::genEeyore(EeyoreGenerator *gen) const { return gen->generateOn(this); }

std::shared_ptr<EeyoreValue> SysYBlockStmt::genEeyore(EeyoreGenerator *gen) const { return gen->generateOn(this); }

std::shared_ptr<EeyoreValue> SysYExpStmt::genEeyore(EeyoreGenerator *gen) const { return gen->generateOn(this); }

std::shared_ptr<EeyoreValue> SysYAssignStmt::genEeyore(EeyoreGenerator *gen) const { return gen->generateOn(this); }

std::shared_ptr<EeyoreValue> SysYIfStmt::genEeyore(EeyoreGenerator *gen) const { return gen->generateOn(this); }

std::shared_ptr<EeyoreValue> SysYWhileStmt::genEeyore(EeyoreGenerator *gen) const { return gen->generateOn(this); }

std::shared_ptr<EeyoreValue> SysYControlStmt::genEeyore(EeyoreGenerator *gen) const { return gen->generateOn(this); }

std::shared_ptr<EeyoreValue> SysYInitVal::genEeyore(EeyoreGenerator *gen) const { return gen->generateOn(this); }

std::shared_ptr<EeyoreValue> SysYVarDef::genEeyore(EeyoreGenerator *gen) const { return gen->generateOn(this); }

std::shared_ptr<EeyoreValue> SysYFuncDef::genEeyore(EeyoreGenerator *gen) const { return gen->generateOn(this); }

std::shared_ptr<EeyoreValue> SysYCompUnit::genEeyore(EeyoreGenerator *gen) const { return gen->generateOn(this); }