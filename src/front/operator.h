//
// Created by 史记 on 2021/5/7.
//

#ifndef SYSYCOMPILER_TOKEN_H
#define SYSYCOMPILER_TOKEN_H

#endif //SYSYCOMPILER_TOKEN_H

#include <string>

std::string op2str(int op);

int calc_bin(int op, int lhs, int rhs);

int calc_un(int op, int rhs);

int cmp_not(int op);