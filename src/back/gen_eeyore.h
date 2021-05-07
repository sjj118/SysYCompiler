//
// Created by 史记 on 2021/5/1.
//

#ifndef SYSYCOMPILER_GEN_EEYORE_H
#define SYSYCOMPILER_GEN_EEYORE_H

#include "nested_map.h"
#include "sysy.h"
#include "eeyore.h"

#include <utility>
#include "parser.hpp"

class SysY2Eeyore_SymbolEntry {
private:
    int dim_, size_;
    int *stride_{};
    bool is_const_;
    int *value_{};
    std::shared_ptr<EeyoreValue> symbol_;
public:
    ~SysY2Eeyore_SymbolEntry() {
        delete[] stride_;
        delete[] value_;
    }

    SysY2Eeyore_SymbolEntry(int dim, int size, int *stride, bool is_const, int *value,
                            std::shared_ptr<EeyoreValue> symbol) :
            dim_(dim), size_(size), stride_(stride), is_const_(is_const), value_(value), symbol_(std::move(symbol)) {}

    SysY2Eeyore_SymbolEntry(const SysY2Eeyore_SymbolEntry &other) :
            dim_(other.dim_), size_(other.size_),
            is_const_(other.is_const_), symbol_(other.symbol_) {
        if (other.stride_) {
            stride_ = new int[other.dim_];
            memcpy(stride_, other.stride_, other.dim_ * 4);
        }
        if (other.value_) {
            value_ = new int[other.size_ / 4];
            memcpy(value_, other.value_, other.size_);
        }
    }

    [[nodiscard]] int dim() const { return dim_; }

    [[nodiscard]] int size() const { return size_; }

    [[nodiscard]] int stride(int x) const { return stride_[x]; }

    [[nodiscard]] bool is_const() const { return is_const_; }

    [[nodiscard]] int value(int x) const { return value_[x]; }

    [[nodiscard]] const std::shared_ptr<EeyoreValue> &symbol() const { return symbol_; }
};

class EeyoreGenerator {
private:
    std::shared_ptr<EeyoreProgram> root;
    int gT_cnt{}, T_cnt{}, t_cnt{}, p_cnt{}, l_cnt{};
    NestedMap<std::string, SysY2Eeyore_SymbolEntry> vars;
    bool flag_param{};
    std::shared_ptr<EeyoreFunc> func;
    std::map<std::string, std::shared_ptr<EeyoreFunc>> funcs;

    static std::shared_ptr<EeyoreValue> logError(const char *msg) {
        printf("Error(eeyore): %s\n", msg);
        exit(1);
        return nullptr;
    }

    void generateInit(const std::vector<SysYInitVal *> &inits, int dim, const int *stride,
                      std::shared_ptr<EeyoreValue> *dst, int begin, int end);

public:
    EeyoreGenerator() {
        root = std::make_shared<EeyoreProgram>();
    }

    std::shared_ptr<EeyoreValue> generateOn(const SysYBinary *ast);

    std::shared_ptr<EeyoreValue> generateOn(const SysYUnary *ast);

    static std::shared_ptr<EeyoreValue> generateOn(const SysYNumber *ast);

    std::shared_ptr<EeyoreValue> generateOn(const SysYFunCall *ast);

    std::shared_ptr<EeyoreValue> generateOn(const SysYLVal *ast);

    std::shared_ptr<EeyoreValue> generateOn(const SysYBlockStmt *ast);

    std::shared_ptr<EeyoreValue> generateOn(const SysYExpStmt *ast);

    std::shared_ptr<EeyoreValue> generateOn(const SysYAssignStmt *ast);

    std::shared_ptr<EeyoreValue> generateOn(const SysYIfStmt *ast);

    std::shared_ptr<EeyoreValue> generateOn(const SysYWhileStmt *ast);

    std::shared_ptr<EeyoreValue> generateOn(const SysYControlStmt *ast);

    std::shared_ptr<EeyoreValue> generateOn(const SysYVarDef *ast);

    std::shared_ptr<EeyoreValue> generateOn(const SysYFuncDef *ast);

    std::shared_ptr<EeyoreProgram> generateOn(const SysYCompUnit *ast);
};

#endif //SYSYCOMPILER_GEN_EEYORE_H
