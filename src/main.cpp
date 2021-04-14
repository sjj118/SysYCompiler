#include<iostream>
#include<cstdio>
#include<cstring>

using namespace std;

int main(int argc, const char *argv[]) {
    string input, output;
    int arg_cnt;
    int src, target = 3;
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
            output = argv[1];
        } else {
            arg_cnt = 1;
            input = argv[0];
            if (input.substr(input.size() - 2, 2) == ".c")src = 0;
            else if (input.substr(input.size() - 7, 7) == ".eeyore")src = 1;
            else if (input.substr(input.size() - 7, 7) == ".tigger")src = 2;
            else {
                cerr << "[error] unknown input file type.\n";
                return 1;
            }
        }
    }
    printf("compile from stage %d to %d.\n", src, target);
    printf("input: %s  output: %s\n", input.c_str(), output.c_str());
    return 0;
}