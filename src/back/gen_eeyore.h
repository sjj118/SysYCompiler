//
// Created by 史记 on 2021/5/1.
//

#ifndef SYSYCOMPILER_GEN_EEYORE_H
#define SYSYCOMPILER_GEN_EEYORE_H

#include "nested_map.h"
#include "sysy.h"
#include "eeyore.h"

#include <stack>
#include <utility>
#include "parser.tab.hpp"

class SysYSymbolEntry {
private:
    int dim_, size_;
    std::vector<int> stride_;
    bool is_const_;
    std::vector<int> value_;
public:
    SysYSymbolEntry(int dim, int size, std::vector<int> *stride, bool is_const, std::vector<int> *value) :
            dim_(dim), size_(size), is_const_(is_const) {
        if (stride)stride_ = *stride;
        if (value)value_ = *value;
    }

    [[nodiscard]] int dim() const { return dim_; }

    [[nodiscard]] int size() const { return size_; }

    [[nodiscard]] int stride(int x) const { return stride_[x]; }

    [[nodiscard]] bool is_const() const { return is_const_; }

    [[nodiscard]] int value(int x) const { return value_[x]; }
};

class SysYFuncEntry {
private:
    bool is_void_;
    std::vector<SysYSymbolEntry> params_;
public:
    explicit SysYFuncEntry(bool is_void) : is_void_(is_void) {}

    void push_param(const SysYSymbolEntry &param) { params_.push_back(param); }

    [[nodiscard]] bool is_void() const { return is_void_; }

    [[nodiscard]] const std::vector<SysYSymbolEntry> &params() const { return params_; }
};

class EeyoreGenerator {
private:
    std::shared_ptr<EeyoreProgram> root;
    int gT_cnt{}, T_cnt{}, t_cnt{}, p_cnt{}, l_cnt{};
    NestedMap<std::string, std::pair<SysYSymbolEntry, std::shared_ptr<EeyoreValue>>> vars;
    std::shared_ptr<EeyoreFunc> func;
    SysYFuncEntry *func_entry{};
    NestedMap<std::string, SysYFuncEntry> funcs;
    std::stack<std::shared_ptr<EeyoreLabel>> continue_stack, break_stack;

    static std::shared_ptr<EeyoreValue> logError(const char *msg) {
        printf("Error(eeyore): %s\n", msg);
        exit(1);
        return nullptr;
    }

    std::shared_ptr<EeyoreTempSymbol> newTempSymbol() {
        auto dst = std::make_shared<EeyoreTempSymbol>(t_cnt++);
        func->push_decl(std::make_shared<EeyoreDeclaration>(dst));
        return dst;
    }

    std::shared_ptr<EeyoreArgSymbol> newArgSymbol() {
        return std::make_shared<EeyoreArgSymbol>(p_cnt++);
    }

    std::shared_ptr<EeyorePrimSymbol> newPrimSymbol(int size = 0) {
        auto dst = std::make_shared<EeyorePrimSymbol>(func ? T_cnt++ : gT_cnt++);
        auto decl = std::make_shared<EeyoreDeclaration>(dst, size);
        if (func)func->push_decl(decl);
        else root->push_decl(decl);
        return dst;
    }

    void generateInit(const std::vector<SysYInitVal *> &inits, int dim, const std::vector<int> &stride,
                      std::vector<std::shared_ptr<EeyoreValue>> &dst, int begin, int end);

public:
    EeyoreGenerator() {
        root = std::make_shared<EeyoreProgram>();
        funcs.insert("getint", SysYFuncEntry(false));
        funcs.insert("getch", SysYFuncEntry(false));
        SysYFuncEntry getarray(false);
        getarray.push_param(SysYSymbolEntry(1, 0, new std::vector<int>(1, 4), false, nullptr));
        funcs.insert("getarray", getarray);
        SysYFuncEntry putint(true);
        putint.push_param(SysYSymbolEntry(0, 0, nullptr, false, nullptr));
        funcs.insert("putint", putint);
        SysYFuncEntry putch(true);
        putch.push_param(SysYSymbolEntry(0, 0, nullptr, false, nullptr));
        funcs.insert("putch", putch);
        SysYFuncEntry putarray(true);
        putarray.push_param(SysYSymbolEntry(0, 0, nullptr, false, nullptr));
        getarray.push_param(SysYSymbolEntry(1, 0, new std::vector<int>(1, 4), false, nullptr));
        funcs.insert("putarray", putarray);
    }

    std::shared_ptr<EeyoreValue> generateOn(const SysYBinary *ast);

    std::shared_ptr<EeyoreValue> generateOn(const SysYUnary *ast);

    static std::shared_ptr<EeyoreValue> generateOn(const SysYNumber *ast);

    std::shared_ptr<EeyoreValue> generateOn(const SysYFunCall *ast);

    std::shared_ptr<EeyoreValue> generateOn(const SysYLVal *ast);

    void generateOn(const SysYBlockStmt *ast);

    void generateOn(const SysYExpStmt *ast);

    void generateOn(const SysYAssignStmt *ast);

    void generateOn(const SysYIfStmt *ast);

    void generateOn(const SysYWhileStmt *ast);

    void generateOn(const SysYControlStmt *ast);

    void generateOn(const SysYVarDef *ast);

    void generateOn(const SysYFuncDef *ast);

    std::shared_ptr<EeyoreProgram> generateOn(const SysYCompUnit *ast);
};

#endif //SYSYCOMPILER_GEN_EEYORE_H
