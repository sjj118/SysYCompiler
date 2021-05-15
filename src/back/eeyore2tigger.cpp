//
// Created by 史记 on 2021/5/11.
//

#include "eeyore2tigger.h"
#include "eeyore.h"
#include "operator.h"
#include "parser.tab.hpp"

void E2TTransformer::load(const std::shared_ptr<EeyoreValue> &val, const std::shared_ptr<TiggerReg> &reg) {
    auto num = std::dynamic_pointer_cast<EeyoreNumber>(val);
    if (num) {
        new_stmt<TiggerAssignStmt>(reg, std::make_shared<TiggerNum>(num->num()));
        return;
    }
    auto symbol = std::dynamic_pointer_cast<EeyoreSymbol>(val);
    auto entry = vars.find(symbol->name());
    std::shared_ptr<TiggerAddr> from;
    if (entry->var()) from = entry->var();
    else from = std::make_shared<TiggerNum>(entry->offset());
    if (entry->is_array())new_stmt<TiggerLoadaddrStmt>(from, reg);
    else new_stmt<TiggerLoadStmt>(from, reg);
}


void E2TTransformer::store(const std::shared_ptr<TiggerReg> &reg, const std::shared_ptr<EeyoreSymbol> &symbol) {
    auto entry = vars.find(symbol->name());
    if (entry->var()) {
        new_stmt<TiggerLoadaddrStmt>(entry->var(), REG.t6);
        new_stmt<TiggerAssignStmt>(REG.t6, reg, std::make_shared<TiggerNum>(0));
    } else new_stmt<TiggerStoreStmt>(reg, entry->offset());
}

void E2TTransformer::generateOn(const EeyoreBinaryStmt *ast) {
    load(ast->lhs(), REG.t1);
    auto rhs_num = std::dynamic_pointer_cast<EeyoreNumber>(ast->rhs());
    if (rhs_num) {
        auto t2 = std::make_shared<TiggerNum>(rhs_num->num());
        new_stmt<TiggerBinaryStmt>(ast->op(), REG.t0, REG.t1, t2);
    } else {
        load(ast->rhs(), REG.t2);
        new_stmt<TiggerBinaryStmt>(ast->op(), REG.t0, REG.t1, REG.t2);
    }
    store(REG.t0, ast->dst());
}

void E2TTransformer::generateOn(const EeyoreUnaryStmt *ast) {
    load(ast->rhs(), REG.t1);
    new_stmt<TiggerUnaryStmt>(ast->op(), REG.t0, REG.t1);
    store(REG.t0, ast->dst());
}

void E2TTransformer::generateOn(const EeyoreAssignStmt *ast) {
    if (ast->dst_offset()) {
        auto offset_num = std::dynamic_pointer_cast<EeyoreNumber>(ast->dst_offset());
        load(ast->dst(), REG.t0);
        load(ast->val(), REG.t1);
        if (offset_num) {
            auto offset = std::make_shared<TiggerNum>(offset_num->num());
            new_stmt<TiggerAssignStmt>(REG.t0, REG.t1, offset);
        } else {
            load(ast->dst_offset(), REG.t2);
            new_stmt<TiggerBinaryStmt>(ADD, REG.t0, REG.t0, REG.t2);
            new_stmt<TiggerAssignStmt>(REG.t0, REG.t1, std::make_shared<TiggerNum>(0));
        }
    } else if (ast->val_offset()) {
        load(ast->val(), REG.t1);
        auto offset_num = std::dynamic_pointer_cast<EeyoreNumber>(ast->val_offset());
        if (offset_num) {
            auto offset = std::make_shared<TiggerNum>(offset_num->num());
            new_stmt<TiggerAssignStmt>(REG.t0, REG.t1, nullptr, offset);
        } else {
            load(ast->val_offset(), REG.t2);
            new_stmt<TiggerBinaryStmt>(ADD, REG.t1, REG.t1, REG.t2);
            new_stmt<TiggerAssignStmt>(REG.t0, REG.t1, nullptr, std::make_shared<TiggerNum>(0));
        }
        store(REG.t0, ast->dst());
    } else {
        load(ast->val(), REG.t0);
        store(REG.t0, ast->dst());
    }
}

void E2TTransformer::generateOn(const EeyoreIfStmt *ast) {
    load(ast->lhs(), REG.t0);
    load(ast->rhs(), REG.t1);
    new_stmt<TiggerIfStmt>(ast->op(), REG.t0, REG.t1, std::make_shared<TiggerLabel>(ast->label()->id()));
}

void E2TTransformer::generateOn(const EeyoreGotoStmt *ast) {
    new_stmt<TiggerGotoStmt>(std::make_shared<TiggerLabel>(ast->label()->id()));
}

void E2TTransformer::generateOn(const EeyoreLabelStmt *ast) {
    new_stmt<TiggerLabelStmt>(std::make_shared<TiggerLabel>(ast->label()->id()));
}

void E2TTransformer::generateOn(const EeyoreParamStmt *ast) {
    load(ast->param(), REG.a(p_cnt++));
}

void E2TTransformer::generateOn(const EeyoreFunCall *ast) {
    new_stmt<TiggerFunCall>(ast->ident());
    if (ast->dst())store(REG.a0, ast->dst());
    p_cnt = 0;
}

void E2TTransformer::generateOn(const EeyoreReturnStmt *ast) {
    if (ast->val())load(ast->val(), REG.a0);
    new_stmt<TiggerReturnStmt>();
}

void E2TTransformer::generateOn(const EeyoreFunc *ast) {
    func = std::make_shared<TiggerFunc>(ast->ident(), ast->arg_num());
    if (ast->ident() == "f_main") for (const auto &init:inits)func->push_stmt(init);
    root->push_func(func);
    vars.nest();
    for (int i = 0; i < ast->arg_num(); i++) {
        vars.insert("p" + std::to_string(i), E2TSymbolEntry(func->slot_num, false));
        new_stmt<TiggerStoreStmt>(REG.a(i), func->slot_num);
        func->slot_num++;
    }
    for (const auto &decl:ast->decls()) {
        vars.insert(decl->symbol()->name(), E2TSymbolEntry(func->slot_num, decl->size()));
        func->slot_num += std::max(1, decl->size());
    }
    for (const auto &stmt:ast->stmts())stmt->genTigger(this);
    vars.unnest();
    func = nullptr;
}

std::shared_ptr<TiggerProgram> E2TTransformer::generateOn(const EeyoreProgram *ast) {
    std::map<std::string, std::shared_ptr<TiggerDeclaration>> decls;
    for (const auto &decl:ast->decls()) {
        auto var = std::make_shared<TiggerVariable>(v_cnt++);
        decls[decl->symbol()->name()] = std::make_shared<TiggerDeclaration>(var, decl->size(), 0);
        vars.insert(decl->symbol()->name(), E2TSymbolEntry(var, decl->size()));
    }
    std::shared_ptr<TiggerVariable> last = nullptr;
    for (const auto &init:ast->inits()) {
        auto t0 = std::make_shared<TiggerReg>("t0");
        int num = std::dynamic_pointer_cast<EeyoreNumber>(init->val())->num();
        if (!init->dst_offset()) {
            decls[init->dst()->name()]->set_init(num);
        } else {
            auto var = decls[init->dst()->name()]->var();
            if (var != last) {
                inits.push_back(std::make_shared<TiggerLoadaddrStmt>(var, REG.t0));
                last = var;
            }
            int offset = std::dynamic_pointer_cast<EeyoreNumber>(init->dst_offset())->num();
            inits.push_back(std::make_shared<TiggerAssignStmt>(REG.t1, std::make_shared<TiggerNum>(num)));
            inits.push_back(std::make_shared<TiggerAssignStmt>(REG.t0, REG.t1, std::make_shared<TiggerNum>(offset)));
        }
    }
    for (const auto &it:decls)root->push_decl(it.second);
    for (const auto &fun:ast->funcs()) fun->genTigger(this);
    return root;
}

void EeyoreBinaryStmt::genTigger(E2TTransformer *gen) const { return gen->generateOn(this); }

void EeyoreUnaryStmt::genTigger(E2TTransformer *gen) const { return gen->generateOn(this); }

void EeyoreAssignStmt::genTigger(E2TTransformer *gen) const { return gen->generateOn(this); }

void EeyoreIfStmt::genTigger(E2TTransformer *gen) const { return gen->generateOn(this); }

void EeyoreGotoStmt::genTigger(E2TTransformer *gen) const { return gen->generateOn(this); }

void EeyoreLabelStmt::genTigger(E2TTransformer *gen) const { return gen->generateOn(this); }

void EeyoreParamStmt::genTigger(E2TTransformer *gen) const { return gen->generateOn(this); }

void EeyoreFunCall::genTigger(E2TTransformer *gen) const { return gen->generateOn(this); }

void EeyoreReturnStmt::genTigger(E2TTransformer *gen) const { return gen->generateOn(this); }

void EeyoreFunc::genTigger(E2TTransformer *gen) const { return gen->generateOn(this); }

std::shared_ptr<TiggerProgram> EeyoreProgram::genTigger(E2TTransformer *gen) const { return gen->generateOn(this); }