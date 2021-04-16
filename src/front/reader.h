//
// Created by 史记 on 2021/4/16.
//

#ifndef SYSYCOMPILER_READER_H
#define SYSYCOMPILER_READER_H

#include <istream>
#include <fstream>
#include <iostream>

class Reader {
private:
    std::istream *in;
    std::string name_;
    int line_ = 1, col_ = 0;
public:
    Reader() : name_("stdin"), in(&std::cin) {}

    explicit Reader(const std::string name) : name_(name), in(new std::ifstream(name)) {}

    char get() {
        char c = in->get();
        if (c == '\n' || c == '\r') {
            line_++;
            col_ = 0;
        } else col_++;
        return c;
    }

    char peek() const { return in->peek(); }

    bool eof() const { return in->eof(); }

    bool eol() const { return eof() || peek() == '\r' || peek() == '\n'; }

    std::string name() const { return name_; }

    int line() const { return line_; }

    int col() const { return col_; }
};

#endif //SYSYCOMPILER_READER_H
