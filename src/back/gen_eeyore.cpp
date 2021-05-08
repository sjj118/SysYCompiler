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
    auto *entry = funcs.find(ast->ident());
//    if (ast->params().size() != entry->params().size())return logError("function call arguments number mismatch");
    for (int i = 0; i < ast->params().size(); i++) {
        auto arg = ast->params()[i];
//        auto param = &entry->params()[i];   // todo: check param
        auto assign = std::make_shared<EeyoreParamStmt>(arg->genEeyore(this));
        func->push_stmt(assign);
    }
    auto dst = std::make_shared<EeyoreTempSymbol>(t_cnt++);
    func->push_stmt(std::make_shared<EeyoreFunCall>(ast->ident(), dst));
    return dst;
}

std::shared_ptr<EeyoreValue> EeyoreGenerator::generateOn(const SysYLVal *ast) {
    auto *it = vars.find(ast->ident());
    auto *entry = &it->first;
    auto symbol = it->second;
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

void EeyoreGenerator::generateOn(const SysYBlockStmt *ast) {
    vars.nest();
    for (auto item:ast->items())item->genEeyore(this);
    vars.unnest();
}

void EeyoreGenerator::generateOn(const SysYExpStmt *ast) {
    ast->exp()->genEeyore(this);
}

void EeyoreGenerator::generateOn(const SysYAssignStmt *ast) {
    auto val = ast->rhs()->genEeyore(this);
    auto *it = vars.find(ast->lhs()->ident());
    auto *entry = &it->first;
    if (entry->is_const())logError("const variable cannot be assigned");
    auto symbol = std::dynamic_pointer_cast<EeyoreSymbol>(it->second);
    std::shared_ptr<EeyoreValue> offset = nullptr;
    if (entry->dim()) {
        SysYExpression *offset_sysy = new SysYBinary(ast->lhs()->indices()[0], MUL, new SysYNumber(entry->stride(0)));
        for (int i = 1; i < ast->lhs()->indices().size(); i++) {
            auto step = new SysYBinary(ast->lhs()->indices()[i], MUL, new SysYNumber(entry->stride(i)));
            offset_sysy = new SysYBinary(offset_sysy, ADD, step);
        }
        offset = offset_sysy->genEeyore(this);
        delete offset_sysy;
        if ((entry->dim() != ast->lhs()->indices().size()))logError("array access with wrong dimension");
    } else if (!ast->lhs()->indices().empty()) logError("scalar variable cannot be accessed with indices");
    if (!func) logError("global expression must be constexpr");
    func->push_stmt(std::make_shared<EeyoreAssignStmt>(symbol, val, offset));
}

void EeyoreGenerator::generateOn(const SysYIfStmt *ast) {
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

void EeyoreGenerator::generateOn(const SysYWhileStmt *ast) {
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

void EeyoreGenerator::generateOn(const SysYControlStmt *ast) {
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

void EeyoreGenerator::generateInit(const std::vector<SysYInitVal *> &inits, int dim, const std::vector<int> &stride,
                                   std::vector<std::shared_ptr<EeyoreValue>> &dst, int begin, int end) {
    if (dim == stride.size()) logError("initexpr too much dimension");
    for (auto *init:inits) {
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

void EeyoreGenerator::generateOn(const SysYVarDef *ast) {
    int dim = ast->indices().size();
    if (dim) {
        std::shared_ptr<EeyoreSymbol> symbol;
        if (func_entry)symbol = std::make_shared<EeyoreArgSymbol>(p_cnt++);
        else symbol = std::make_shared<EeyorePrimSymbol>(func ? T_cnt++ : gT_cnt++);
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
        auto decl = std::make_shared<EeyoreDeclaration>(symbol, size);
        if (func_entry)func_entry->push_param(SysYSymbolEntry(dim, size, &stride, ast->is_const(), nullptr));
        else if (func)func->push_decl(decl);
        else root->push_decl(decl);
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
                             std::make_pair(SysYSymbolEntry(dim, size, &stride, ast->is_const(), &value_num), symbol)))
                logError("redefinition of same variable");
            return;
        }
        if (ast->is_const()) logError("const variable must be initialized");
        if (!vars.insert(ast->ident(), std::make_pair(SysYSymbolEntry(dim, size, &stride, false, nullptr), symbol)))
            logError("redefinition of same variable");
        return;
    }
    if (ast->is_const()) {
        if (!ast->init()) logError("const variable must be initialized");
        if (!ast->init()->exp()) logError("scalar variable must be initialized by scalar");
        auto num = std::dynamic_pointer_cast<EeyoreNumber>(ast->init()->exp()->genEeyore(this));
        if (!num) logError("const variable must be initialized with constexpr");
        if (!vars.insert(ast->ident(), std::make_pair(SysYSymbolEntry(0, 0, nullptr, true, nullptr), num)))
            logError("redefinition of same variable");
        return;
    }
    std::shared_ptr<EeyoreSymbol> symbol;
    if (func_entry)symbol = std::make_shared<EeyoreArgSymbol>(p_cnt++);
    else symbol = std::make_shared<EeyorePrimSymbol>(func ? T_cnt++ : gT_cnt++);
    auto decl = std::make_shared<EeyoreDeclaration>(symbol);
    if (func_entry)func_entry->push_param(SysYSymbolEntry(0, 0, nullptr, false, nullptr));
    else if (func)func->push_decl(decl);
    else root->push_decl(decl);
    if (ast->init()) {
        if (!ast->init()->exp()) logError("scalar variable must be initialized by scalar");
        auto init = ast->init()->exp()->genEeyore(this);
        auto assign = std::make_shared<EeyoreAssignStmt>(symbol, init);
        if (func)func->push_stmt(assign);
        else root->push_init(assign);
    }
    if (!vars.insert(ast->ident(), std::make_pair(SysYSymbolEntry(0, 0, nullptr, false, nullptr), symbol)))
        logError("redefinition of same variable");
}

void EeyoreGenerator::generateOn(const SysYFuncDef *ast) {
    func = std::make_shared<EeyoreFunc>("f_" + ast->ident(), ast->params().size());
    root->push_func(func);
    vars.nest();
    T_cnt = gT_cnt;
    p_cnt = t_cnt = 0;
    funcs.insert(ast->ident(), SysYFuncEntry());
    func_entry = funcs.find(ast->ident());
    for (auto *param:ast->params())param->genEeyore(this);
    func_entry = nullptr;
    for (auto *item:ast->items()) item->genEeyore(this);
    vars.unnest();
    func = nullptr;
}

std::shared_ptr<EeyoreProgram> EeyoreGenerator::generateOn(const SysYCompUnit *ast) {
    for (auto *def:ast->defs())if (dynamic_cast<SysYVarDef *>(def))def->genEeyore(this);
    for (auto *def:ast->defs())if (dynamic_cast<SysYFuncDef *>(def))def->genEeyore(this);
    return root;
}

std::shared_ptr<EeyoreValue> SysYBinary::genEeyore(EeyoreGenerator *gen) const { return gen->generateOn(this); }

std::shared_ptr<EeyoreValue> SysYUnary::genEeyore(EeyoreGenerator *gen) const { return gen->generateOn(this); }

std::shared_ptr<EeyoreValue> SysYNumber::genEeyore(EeyoreGenerator *gen) const { return gen->generateOn(this); }

std::shared_ptr<EeyoreValue> SysYFunCall::genEeyore(EeyoreGenerator *gen) const { return gen->generateOn(this); }

std::shared_ptr<EeyoreValue> SysYLVal::genEeyore(EeyoreGenerator *gen) const { return gen->generateOn(this); }

void SysYBlockStmt::genEeyore(EeyoreGenerator *gen) const { return gen->generateOn(this); }

void SysYExpStmt::genEeyore(EeyoreGenerator *gen) const { return gen->generateOn(this); }

void SysYAssignStmt::genEeyore(EeyoreGenerator *gen) const { return gen->generateOn(this); }

void SysYIfStmt::genEeyore(EeyoreGenerator *gen) const { return gen->generateOn(this); }

void SysYWhileStmt::genEeyore(EeyoreGenerator *gen) const { return gen->generateOn(this); }

void SysYControlStmt::genEeyore(EeyoreGenerator *gen) const { return gen->generateOn(this); }

void SysYVarDef::genEeyore(EeyoreGenerator *gen) const { return gen->generateOn(this); }

void SysYFuncDef::genEeyore(EeyoreGenerator *gen) const { return gen->generateOn(this); }

std::shared_ptr<EeyoreProgram> SysYCompUnit::genEeyore(EeyoreGenerator *gen) const { return gen->generateOn(this); }