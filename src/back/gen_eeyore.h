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
#include "parser.hpp"

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
    std::vector<SysYSymbolEntry> params_;
public:
    void push_param(const SysYSymbolEntry &param) { params_.push_back(param); }

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

    void generateInit(const std::vector<SysYInitVal *> &inits, int dim, const std::vector<int> &stride,
                      std::vector<std::shared_ptr<EeyoreValue>> &dst, int begin, int end);

public:
    EeyoreGenerator() {
        root = std::make_shared<EeyoreProgram>();
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
