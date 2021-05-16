//
// Created by 史记 on 2021/5/1.
//

#include "sysy.h"
#include "sysy2eeyore.h"
#include "operator.h"
#include "parser.tab.hpp"

std::shared_ptr<EeyoreValue> S2ETransformer::generateOn(const SysYBinary *ast) {
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
        auto dst = newTempSymbol();
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
        auto dst = newTempSymbol();
        func->push_stmt(std::make_shared<EeyoreBinaryStmt>(op, dst, lhs, rhs));
        return dst;
    }
    return std::make_shared<EeyoreNumber>(calc_bin(op, lnum->num(), rnum->num()));
}

std::shared_ptr<EeyoreValue> S2ETransformer::generateOn(const SysYUnary *ast) {
    int op = ast->op();
    auto rhs = ast->rhs()->genEeyore(this);
    auto rnum = std::dynamic_pointer_cast<EeyoreNumber>(rhs);
    if (!rnum) {
        if (!func)return logError("global expression must be constexpr");
        if (op == ADD) {
            return rhs;
        } else {
            auto dst = newTempSymbol();
            func->push_stmt(std::make_shared<EeyoreUnaryStmt>(op, dst, rhs));
            return dst;
        }
    }
    return std::make_shared<EeyoreNumber>(calc_un(op, rnum->num()));
}

std::shared_ptr<EeyoreValue> S2ETransformer::generateOn(const SysYNumber *ast) {
    return std::make_shared<EeyoreNumber>(ast->num());
}

std::shared_ptr<EeyoreValue> S2ETransformer::generateOn(const SysYFunCall *ast) {
    auto *entry = funcs.find(ast->ident());
    if (ast->params().size() != entry->params().size())return logError("function call arguments number mismatch");
    std::vector<std::shared_ptr<EeyoreValue>> params;
    for (int i = 0; i < ast->params().size(); i++) {
        auto arg = ast->params()[i];
//        auto param = &entry->params()[i];   // todo: check param
        params.push_back(arg->genEeyore(this));
    }
    for (auto param:params) {
        auto assign = std::make_shared<EeyoreParamStmt>(param);
        func->push_stmt(assign);
    }
    std::shared_ptr<EeyoreTempSymbol> dst = nullptr;
    if (!entry->is_void())dst = newTempSymbol();
    func->push_stmt(std::make_shared<EeyoreFunCall>(ast->ident(), dst));
    return dst;
}

std::shared_ptr<EeyoreValue> S2ETransformer::generateOn(const SysYLVal *ast) {
    auto *it = vars.find(ast->ident());
    auto *entry = &it->first;
    auto symbol = it->second;
    if (!ast->indices().empty()) {
        std::shared_ptr<SysYExpression> offset_sysy = std::make_shared<SysYBinary>(
                ast->indices()[0], MUL,
                std::make_shared<SysYNumber>(entry->stride(0)));
        for (int i = 1; i < ast->indices().size(); i++) {
            auto step = std::make_shared<SysYBinary>(ast->indices()[i], MUL,
                                                     std::make_shared<SysYNumber>(entry->stride(i)));
            offset_sysy = std::make_shared<SysYBinary>(offset_sysy, ADD, step);
        }
        auto offset = offset_sysy->genEeyore(this);
        auto offset_num = std::dynamic_pointer_cast<EeyoreNumber>(offset);
        if (offset_num && entry->is_const()) return std::make_shared<EeyoreNumber>(entry->value(offset_num->num() / 4));
        if (!func)return logError("global expression must be constexpr");
        auto dst = newTempSymbol();
        if (entry->dim() == ast->indices().size()) {
            func->push_stmt(std::make_shared<EeyoreAssignStmt>(dst, symbol, nullptr, offset));
        } else {
            func->push_stmt(std::make_shared<EeyoreBinaryStmt>(ADD, dst, symbol, offset));
        }
        return dst;
    }
    if (!ast->indices().empty())return logError("scalar variable cannot be accessed with indices");
    return symbol;
}

void S2ETransformer::generateOn(const SysYBlockStmt *ast) {
    vars.nest();
    for (const auto &item:ast->items())item->genEeyore(this);
    vars.unnest();
}

void S2ETransformer::generateOn(const SysYExpStmt *ast) {
    ast->exp()->genEeyore(this);
}

void S2ETransformer::generateOn(const SysYAssignStmt *ast) {
    auto val = ast->rhs()->genEeyore(this);
    auto *it = vars.find(ast->lhs()->ident());
    auto *entry = &it->first;
    if (entry->is_const())logError("const variable cannot be assigned");
    auto symbol = std::dynamic_pointer_cast<EeyoreSymbol>(it->second);
    std::shared_ptr<EeyoreValue> offset = nullptr;
    if (entry->dim()) {
        std::shared_ptr<SysYExpression> offset_sysy = std::make_shared<SysYBinary>(
                ast->lhs()->indices()[0], MUL, std::make_shared<SysYNumber>(entry->stride(0)));
        for (int i = 1; i < ast->lhs()->indices().size(); i++) {
            auto step = std::make_shared<SysYBinary>(ast->lhs()->indices()[i], MUL,
                                                     std::make_shared<SysYNumber>(entry->stride(i)));
            offset_sysy = std::make_shared<SysYBinary>(offset_sysy, ADD, step);
        }
        offset = offset_sysy->genEeyore(this);
        if ((entry->dim() != ast->lhs()->indices().size()))logError("array access with wrong dimension");
    } else if (!ast->lhs()->indices().empty()) logError("scalar variable cannot be accessed with indices");
    if (!func) logError("global expression must be constexpr");
    func->push_stmt(std::make_shared<EeyoreAssignStmt>(symbol, val, offset));
}

void S2ETransformer::generateOn(const SysYIfStmt *ast) {
    auto cond = ast->cond()->genEeyore(this);
    if (ast->else_stmt()) {
        auto else_label = std::make_shared<EeyoreLabel>(l_cnt++);
        auto end_label = std::make_shared<EeyoreLabel>(l_cnt++);
        func->push_stmt(std::make_shared<EeyoreIfStmt>(EQ, cond, std::make_shared<EeyoreNumber>(0), else_label));
        ast->then_stmt()->genEeyore(this);
        func->push_stmt(std::make_shared<EeyoreGotoStmt>(end_label));
        func->push_stmt(std::make_shared<EeyoreLabelStmt>(else_label));
        ast->else_stmt()->genEeyore(this);
        func->push_stmt(std::make_shared<EeyoreLabelStmt>(end_label));
    } else {
        auto end_label = std::make_shared<EeyoreLabel>(l_cnt++);
        func->push_stmt(std::make_shared<EeyoreIfStmt>(EQ, cond, std::make_shared<EeyoreNumber>(0), end_label));
        ast->then_stmt()->genEeyore(this);
        func->push_stmt(std::make_shared<EeyoreLabelStmt>(end_label));
    }
}

void S2ETransformer::generateOn(const SysYWhileStmt *ast) {
    auto continue_label = std::make_shared<EeyoreLabel>(l_cnt++);
    auto break_label = std::make_shared<EeyoreLabel>(l_cnt++);
    continue_stack.push(continue_label);
    break_stack.push(break_label);
    func->push_stmt(std::make_shared<EeyoreLabelStmt>(continue_label));
    auto cond = ast->cond()->genEeyore(this);
    func->push_stmt(std::make_shared<EeyoreIfStmt>(EQ, cond, std::make_shared<EeyoreNumber>(0), break_label));
    ast->stmt()->genEeyore(this);
    func->push_stmt(std::make_shared<EeyoreGotoStmt>(continue_label));
    func->push_stmt(std::make_shared<EeyoreLabelStmt>(break_label));
    continue_stack.pop();
    break_stack.pop();
}

void S2ETransformer::generateOn(const SysYControlStmt *ast) {
    if (ast->type() == RETURN) {
        std::shared_ptr<EeyoreValue> val = nullptr;
        if (ast->exp())val = ast->exp()->genEeyore(this);
        func->push_stmt(std::make_shared<EeyoreReturnStmt>(val));
    } else if (ast->type() == CONTINUE) {
        func->push_stmt(std::make_shared<EeyoreGotoStmt>(continue_stack.top()));
    } else if (ast->type() == BREAK) {
        func->push_stmt(std::make_shared<EeyoreGotoStmt>(break_stack.top()));
    }
}

void S2ETransformer::generateInit(const std::vector<std::shared_ptr<SysYInitVal>> &inits, int dim,
                                  const std::vector<int> &stride,
                                  std::vector<std::shared_ptr<EeyoreValue>> &dst, int begin, int end) {
    if (dim == stride.size()) logError("initexpr too much dimension");
    for (const auto &init:inits) {
        if (begin >= end) logError("initexpr too long");
        if (init->exp()) {
            dst[begin++] = init->exp()->genEeyore(this);
        } else {
            if (begin % (stride[dim] / 4) != 0) logError("initexpr stride mismatch");
            generateInit(*init->list(), dim + 1, stride, dst, begin, begin + stride[dim] / 4);
            begin += stride[dim] / 4;
        }
    }
    while (begin < end) dst[begin++] = std::make_shared<EeyoreNumber>(0);
}

void S2ETransformer::generateOn(const SysYVarDef *ast) {
    int dim = ast->indices().size();
    if (dim) {
        std::vector<int> stride(dim);
        std::vector<int> indice(dim);
        for (int i = 0; i < dim; i++) {
            auto num = std::dynamic_pointer_cast<EeyoreNumber>(ast->indices()[i]->genEeyore(this));
            if (!num) logError("global expression must be constexpr");
            indice[i] = num->num();
        }
        stride[dim - 1] = 4;
        for (int i = dim - 2; i >= 0; i--) {
            stride[i] = stride[i + 1] * indice[i + 1];
        }
        int size = stride[0] * indice[0];
        std::shared_ptr<EeyoreSymbol> symbol;
        if (func_entry)symbol = newArgSymbol();
        else symbol = newPrimSymbol(size);
        if (func_entry)func_entry->push_param(S2ESymbolEntry(dim, size, &stride, ast->is_const(), nullptr));
        if (ast->init()) {
            std::vector<std::shared_ptr<EeyoreValue>> value(size / 4);
            std::vector<int> value_num;
            if (!ast->init()->list()) logError("array variable must be initialized by list");
            generateInit(*ast->init()->list(), 0, stride, value, 0, size / 4);
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
                value_num.resize(size / 4);
                for (int i = 0; i < size / 4; i++) {
                    auto num = std::dynamic_pointer_cast<EeyoreNumber>(value[i]);
                    if (!num) logError("const variable must be initialized with constexpr");
                    value_num[i] = num->num();
                }
            }
            if (!vars.insert(ast->ident(),
                             std::make_pair(S2ESymbolEntry(dim, size, &stride, ast->is_const(), &value_num), symbol)))
                logError("redefinition of same variable");
            return;
        }
        if (ast->is_const()) logError("const variable must be initialized");
        if (!vars.insert(ast->ident(), std::make_pair(S2ESymbolEntry(dim, size, &stride, false, nullptr), symbol)))
            logError("redefinition of same variable");
        return;
    }
    if (ast->is_const()) {
        if (!ast->init()) logError("const variable must be initialized");
        if (!ast->init()->exp()) logError("scalar variable must be initialized by scalar");
        auto num = std::dynamic_pointer_cast<EeyoreNumber>(ast->init()->exp()->genEeyore(this));
        if (!num) logError("const variable must be initialized with constexpr");
        if (!vars.insert(ast->ident(), std::make_pair(S2ESymbolEntry(0, 0, nullptr, true, nullptr), num)))
            logError("redefinition of same variable");
        return;
    }
    std::shared_ptr<EeyoreSymbol> symbol;
    if (func_entry)symbol = newArgSymbol();
    else symbol = newPrimSymbol();
    if (func_entry)func_entry->push_param(S2ESymbolEntry(0, 0, nullptr, false, nullptr));
    if (ast->init()) {
        if (!ast->init()->exp()) logError("scalar variable must be initialized by scalar");
        auto init = ast->init()->exp()->genEeyore(this);
        auto assign = std::make_shared<EeyoreAssignStmt>(symbol, init);
        if (func)func->push_stmt(assign);
        else root->push_init(assign);
    }
    if (!vars.insert(ast->ident(), std::make_pair(S2ESymbolEntry(0, 0, nullptr, false, nullptr), symbol)))
        logError("redefinition of same variable");
}

void S2ETransformer::generateOn(const SysYFuncDef *ast) {
    func = std::make_shared<EeyoreFunc>(ast->ident(), ast->params().size());
    root->push_func(func);
    vars.nest();
    T_cnt = gT_cnt;
    p_cnt = t_cnt = 0;
    funcs.insert(ast->ident(), S2EFuncEntry(ast->is_void()));
    func_entry = funcs.find(ast->ident());
    for (const auto &param:ast->params())param->genEeyore(this);
    func_entry = nullptr;
    for (const auto &item:ast->items()) item->genEeyore(this);
    func->push_stmt(std::make_shared<EeyoreReturnStmt>(nullptr));
    vars.unnest();
    func = nullptr;
}

std::shared_ptr<EeyoreProgram> S2ETransformer::generateOn(const SysYCompUnit *ast) {
    for (const auto &def:ast->defs())if (std::dynamic_pointer_cast<SysYVarDef>(def))def->genEeyore(this);
    for (const auto &def:ast->defs())if (std::dynamic_pointer_cast<SysYFuncDef>(def))def->genEeyore(this);
    return root;
}

std::shared_ptr<EeyoreValue> SysYBinary::genEeyore(S2ETransformer *gen) const { return gen->generateOn(this); }

std::shared_ptr<EeyoreValue> SysYUnary::genEeyore(S2ETransformer *gen) const { return gen->generateOn(this); }

std::shared_ptr<EeyoreValue> SysYNumber::genEeyore(S2ETransformer *gen) const { return gen->generateOn(this); }

std::shared_ptr<EeyoreValue> SysYFunCall::genEeyore(S2ETransformer *gen) const { return gen->generateOn(this); }

std::shared_ptr<EeyoreValue> SysYLVal::genEeyore(S2ETransformer *gen) const { return gen->generateOn(this); }

void SysYBlockStmt::genEeyore(S2ETransformer *gen) const { return gen->generateOn(this); }

void SysYExpStmt::genEeyore(S2ETransformer *gen) const { return gen->generateOn(this); }

void SysYAssignStmt::genEeyore(S2ETransformer *gen) const { return gen->generateOn(this); }

void SysYIfStmt::genEeyore(S2ETransformer *gen) const { return gen->generateOn(this); }

void SysYWhileStmt::genEeyore(S2ETransformer *gen) const { return gen->generateOn(this); }

void SysYControlStmt::genEeyore(S2ETransformer *gen) const { return gen->generateOn(this); }

void SysYVarDef::genEeyore(S2ETransformer *gen) const { return gen->generateOn(this); }

void SysYFuncDef::genEeyore(S2ETransformer *gen) const { return gen->generateOn(this); }

std::shared_ptr<EeyoreProgram> SysYCompUnit::genEeyore(S2ETransformer *gen) const { return gen->generateOn(this); }