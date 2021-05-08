#include <iostream>
#include <fstream>
#include <cstdio>
#include "sysy.h"
#include "gen_eeyore.h"

extern SysYCompUnit *root;

extern int yylex_destroy();

using namespace std;

int main(int argc, const char *argv[]) {
    yyparse();
    yylex_destroy();
    auto *eg = new EeyoreGenerator();
    root->genEeyore(eg)->dump(std::cout);
    /*
    ofstream output;
    Reader reader;
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
            reader = Reader(filename);
            if (filename.substr(filename.size() - 2, 2) == ".c")src = 0;
            else if (filename.substr(filename.size() - 7, 7) == ".eeyore")src = 1;
            else if (filename.substr(filename.size() - 7, 7) == ".tigger")src = 2;
            else {
                cerr << "[error] unknown input file type_.\n";
                return 1;
            }
        }
    }
    if (lex) {
        printf("start lexing from stdin\n");
        auto lexer = Lexer(reader);
        while (lexer.token() != Token::End) {
            std::cout << lexer.toString(lexer.token());
            if (lexer.token() == Token::Error)return 1;
            lexer.next();
        }
        puts("");
    } else {
        printf("compile from stage %d to %d.\n", src, target);
        // todo
    }*/
    return 0;
}