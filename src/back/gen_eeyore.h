//
// Created by 史记 on 2021/5/1.
//

#ifndef SYSYCOMPILER_GEN_EEYORE_H
#define SYSYCOMPILER_GEN_EEYORE_H

#include "environment.h"
#include "sysy.h"
#include "eeyore.h"
#include "parser.hpp"

class EeyoreGenerator {
private:
    int T_cnt{}, t_cnt{}, l_cnt{};
    Environment<std::string, EeyoreValue> vars;
    std::vector<EeyoreAssignStmt *> inits;
    EeyoreFunc *func{};
    std::map<std::string, EeyoreFunc *> funcs;
    int err_cnt{};

    EeyoreValue *logError(const char *msg) {
        printf("Error(eeyore): %s\n", msg);
        err_cnt++;
        return nullptr;
    }

public:
    EeyoreGenerator() = default;

    ~EeyoreGenerator() {
        for (auto *init:inits)delete init;
        for (const auto &it:funcs)delete it.second;
    }

    EeyoreValue *generateOn(const SysYCompUnit *ast);

    EeyoreValue *generateOn(const SysYVarDef *ast);

    EeyoreValue *generateOn(const SysYFuncDef *ast);

    EeyoreValue *generateOn(const SysYBinary *ast);

    EeyoreValue *generateOn(const SysYUnary *ast);

    EeyoreValue *generateOn(const SysYNumber *ast);
};

#endif //SYSYCOMPILER_GEN_EEYORE_H
