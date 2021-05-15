#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include "parser.tab.hpp"
#include "sysy.h"
#include "eeyore.h"
#include "tigger.h"
#include "sysy2eeyore.h"
#include "eeyore2tigger.h"

extern SysYCompUnit *root;

extern FILE *yyin;

extern int yylex_destroy();

using namespace std;

int main(int argc, const char *argv[]) {
    ofstream output{};
    FILE *input{};
    int arg_cnt;
    int src = 0, target = 3;
    bool lex = false;
    for (argc--, argv++; argc; argc -= arg_cnt, argv += arg_cnt) {
        if (strcmp(argv[0], "-S") == 0) {
            arg_cnt = 1;
        } else if (strcmp(argv[0], "-e") == 0) {
            target = 1;
            arg_cnt = 1;
        } else if (strcmp(argv[0], "-t") == 0) {
            target = 2;
            arg_cnt = 1;
        } else if (strcmp(argv[0], "-o") == 0) {
            arg_cnt = 2;
            output.open(argv[1]);
        } else if (strcmp(argv[0], "-l") == 0) {
            arg_cnt = 1;
            lex = true;
        } else {
            arg_cnt = 1;
            string filename = argv[0];
            input = fopen(argv[0], "r");
            if (filename.substr(filename.size() - 2, 2) == ".c" || filename.substr(filename.size() - 3, 3) == ".sy")
                src = 0;
            else if (filename.substr(filename.size() - 7, 7) == ".eeyore")src = 1;
            else if (filename.substr(filename.size() - 7, 7) == ".tigger")src = 2;
            else {
                cerr << "[error] unknown input file type_.\n";
                return 2;
            }
        }
    }
    if (src == 0) {
        if (input)yyin = input;
        yyparse();
        yylex_destroy();
        auto eg = new S2ETransformer();
        auto eeyore = root->genEeyore(eg);
        if (target == 1) {
            if (output.is_open())eeyore->dump(output);
            else eeyore->dump(std::cout);
            return 0;
        }
        auto tg = new E2TTransformer();
        auto tigger = eeyore->genTigger(tg);
        if (target == 2) {
            if (output.is_open())tigger->dump(output);
            else tigger->dump(std::cout);
            return 0;
        }
    }
    return 0;
}