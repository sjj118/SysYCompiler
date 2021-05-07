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
        if (!func)return logError("global expression must be constexpr");
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
        if (!func)return logError("global expression must be constexpr");
        auto dst = std::make_shared<EeyoreTempSymbol>(t_cnt++);
        func->push_decl(std::make_shared<EeyoreDeclaration>(dst));
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
        if (!func)return logError("global expression must be constexpr");
        if (op == ADD) {
            return rhs;
        } else {
            auto dst = std::make_shared<EeyoreTempSymbol>(t_cnt++);
            func->push_decl(std::make_shared<EeyoreDeclaration>(dst));
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
    auto entry = vars.find(ast->ident());
    if (entry->dim()) {
        SysYExpression *offset_sysy = new SysYBinary(ast->indices()[0], MUL, new SysYNumber(entry->stride(0)));
        for (int i = 1; i < ast->indices().size(); i++) {
            auto step = new SysYBinary(ast->indices()[i], MUL, new SysYNumber(entry->stride(i)));
            offset_sysy = new SysYBinary(offset_sysy, ADD, step);
        }
        auto offset = offset_sysy->genEeyore(this);
        delete offset_sysy;
        auto offset_num = std::dynamic_pointer_cast<EeyoreNumber>(offset);
        if (offset_num && entry->is_const()) return std::make_shared<EeyoreNumber>(entry->value(offset_num->num() / 4));
        if (!func)return logError("global expression must be constexpr");
        auto dst = std::make_shared<EeyoreTempSymbol>(t_cnt++);
        func->push_stmt(std::make_shared<EeyoreAssignStmt>(dst, entry->symbol(), nullptr, offset));
        return dst;
    }
    if (!ast->indices().empty())return logError("scalar variable cannot be accessed with indices");
    return entry->symbol();
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

void EeyoreGenerator::generateInit(const std::vector<SysYInitVal *> &inits, int dim, const int *stride,
                                   std::shared_ptr<EeyoreValue> *dst, int begin, int end) {
    if (dim == 0) logError("initexpr too much dimension");
    for (auto *init:inits) {
        if (begin >= end) logError("initexpr too long");
        if (init->exp()) {
            dst[begin++] = init->exp()->genEeyore(this);
        } else {
            if (begin % (stride[0] / 4) != 0) logError("initexpr stride mismatch");
            generateInit(*init->list(), dim - 1, stride + 1, dst, begin, begin + stride[0] / 4);
            begin += stride[0] / 4;
        }
    }
    while (begin < end) dst[begin++] = std::make_shared<EeyoreNumber>(0);
}

std::shared_ptr<EeyoreValue> EeyoreGenerator::generateOn(const SysYVarDef *ast) {
    int dim = ast->indices().size();
    if (dim) {
        auto symbol = std::make_shared<EeyorePrimSymbol>(func ? T_cnt++ : gT_cnt++);
        int *stride = new int[dim];
        int *indice = new int[dim];
        for (int i = 0; i < dim; i++) {
            auto num = std::dynamic_pointer_cast<EeyoreNumber>(ast->indices()[i]->genEeyore(this));
            if (!num)return logError("global expression must be constexpr");
            indice[i] = num->num();
        }
        stride[dim - 1] = 4;
        for (int i = dim - 2; i >= 0; i--) {
            stride[i] = stride[i + 1] * indice[i + 1];
        }
        int size = stride[0] * indice[0];
        delete[] indice;
        auto decl = std::make_shared<EeyoreDeclaration>(symbol, size);
        if (func)func->push_decl(decl);
        else root->push_decl(decl);
        if (ast->init()) {
            auto *value = new std::shared_ptr<EeyoreValue>[size / 4];
            int *value_num = nullptr;
            if (!ast->init()->list())return logError("array variable must be initialized by list");
            generateInit(*ast->init()->list(), dim, stride, value, 0, size / 4);
            for (int i = 0; i < size / 4; i++) {
                auto assign = std::make_shared<EeyoreAssignStmt>(symbol, value[i],
                                                                 std::make_shared<EeyoreNumber>(i * 4));
                if (func) func->push_stmt(assign);
                else {
                    auto num = std::dynamic_pointer_cast<EeyoreNumber>(value[i]);
                    if (!(num && num->num() == 0))root->push_init(assign);
                }
            }
            if (ast->is_const()) {
                value_num = new int[size / 4];
                for (int i = 0; i < size / 4; i++) {
                    auto num = std::dynamic_pointer_cast<EeyoreNumber>(value[i]);
                    if (!num)return logError("const variable must be initialized with constexpr");
                    value_num[i] = num->num();
                }
            }
            delete[] value;
            vars.insert(ast->ident(), SysY2Eeyore_SymbolEntry(dim, size, stride, ast->is_const(), value_num, symbol));
            return symbol;
        }
        if (ast->is_const())return logError("const variable must be initialized");
        vars.insert(ast->ident(), SysY2Eeyore_SymbolEntry(dim, size, stride, false, nullptr, symbol));
        return symbol;
    }
    if (ast->is_const()) {
        if (!ast->init())return logError("const variable must be initialized");
        if (!ast->init()->exp())return logError("scalar variable must be initialized by scalar");
        auto num = std::dynamic_pointer_cast<EeyoreNumber>(ast->init()->exp()->genEeyore(this));
        if (!num)return logError("const variable must be initialized with constexpr");
        vars.insert(ast->ident(), SysY2Eeyore_SymbolEntry(0, 0, nullptr, true, nullptr, num));
        return num;
    }
    auto symbol = std::make_shared<EeyorePrimSymbol>(func ? T_cnt++ : gT_cnt++);
    auto decl = std::make_shared<EeyoreDeclaration>(symbol);
    if (func)func->push_decl(decl);
    else root->push_decl(decl);
    if (ast->init()) {
        if (!ast->init()->exp())return logError("scalar variable must be initialized by scalar");
        auto init = std::dynamic_pointer_cast<EeyoreNumber>(ast->init()->exp()->genEeyore(this));
        auto assign = std::make_shared<EeyoreAssignStmt>(symbol, init);
        if (func)func->push_stmt(assign);
        else root->push_init(assign);
    }
    vars.insert(ast->ident(), SysY2Eeyore_SymbolEntry(0, 0, nullptr, false, nullptr, symbol));
    return symbol;
}

std::shared_ptr<EeyoreValue> EeyoreGenerator::generateOn(const SysYFuncDef *ast) {
    func = new EeyoreFunc("f_" + ast->ident(), ast->params().size());
    return nullptr;
}

std::shared_ptr<EeyoreValue> EeyoreGenerator::generateOn(const SysYCompUnit *ast) {
    for (auto *def:ast->defs()) def->genEeyore(this);
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

std::shared_ptr<EeyoreValue> SysYVarDef::genEeyore(EeyoreGenerator *gen) const { return gen->generateOn(this); }

std::shared_ptr<EeyoreValue> SysYFuncDef::genEeyore(EeyoreGenerator *gen) const { return gen->generateOn(this); }

std::shared_ptr<EeyoreValue> SysYCompUnit::genEeyore(EeyoreGenerator *gen) const { return gen->generateOn(this); }