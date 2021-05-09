#include <iostream>
#include <fstream>
#include <cstdio>
#include <cstring>
#include "sysy.h"
#include "gen_eeyore.h"

extern SysYCompUnit *root;

extern FILE *yyin;

extern int yylex_destroy();

using namespace std;

int main(int argc, const char *argv[]) {
    ofstream output;
    FILE *input;
    int arg_cnt;
    int src, target = 3;
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
            if (filename.substr(filename.size() - 2, 2) == ".c")src = 0;
            if (filename.substr(filename.size() - 3, 3) == ".sy")src = 0;
            else if (filename.substr(filename.size() - 7, 7) == ".eeyore")src = 1;
            else if (filename.substr(filename.size() - 7, 7) == ".tigger")src = 2;
            else {
                cerr << "[error] unknown input file type_.\n";
                return 1;
            }
        }
    }
    if (src == 0 && target == 1) {
        yyin = input;
        return 0;
        yyparse();
        yylex_destroy();
        auto *eg = new EeyoreGenerator();
        root->genEeyore(eg)->dump(output);
    }
    return 0;
}