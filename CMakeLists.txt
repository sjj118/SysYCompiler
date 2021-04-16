cmake_minimum_required(VERSION 3.19)
project(SysYCompiler)

set(CMAKE_CXX_STANDARD 17)

add_executable(SysYCompiler src/main.cpp src/front/lexer.cpp src/front/lexer.h src/front/token.h)