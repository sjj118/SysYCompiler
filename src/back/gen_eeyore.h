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
    EeyoreProgram *root;
    int gT_cnt{}, T_cnt{}, t_cnt{}, l_cnt{};
    Environment<std::string, EeyoreValue> vars;
    EeyoreFunc *func{};
    std::map<std::string, EeyoreFunc *> funcs;
    int err_cnt{};

    std::shared_ptr<EeyoreValue> logError(const char *msg) {
        printf("Error(eeyore): %s\n", msg);
        err_cnt++;
        return nullptr;
    }

public:
    EeyoreGenerator() {
        root = new EeyoreProgram();
    }

    ~EeyoreGenerator() {
        for (const auto &it:funcs)delete it.second;
    }

    [[nodiscard]] const EeyoreProgram *program() const { return root; }

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

    std::shared_ptr<EeyoreValue> generateOn(const SysYInitVal *ast);

    std::shared_ptr<EeyoreValue> generateOn(const SysYVarDef *ast);

    std::shared_ptr<EeyoreValue> generateOn(const SysYFuncDef *ast);

    std::shared_ptr<EeyoreValue> generateOn(const SysYCompUnit *ast);
};

#endif //SYSYCOMPILER_GEN_EEYORE_H
