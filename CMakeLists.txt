cmake_minimum_required(VERSION 3.19)
project(SysYCompiler)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-register")

find_package(BISON)
find_package(FLEX)
BISON_TARGET(parser src/front/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.tab.cpp)
FLEX_TARGET(scanner src/front/scanner.l ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.cpp)
ADD_FLEX_BISON_DEPENDENCY(scanner parser)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(src src/define src/front src/back)

add_executable(SysYCompiler ${BISON_parser_OUTPUTS} ${FLEX_scanner_OUTPUTS}
        src/main.cpp src/define/sysy.h src/back/nested_map.h src/define/eeyore.h src/back/sysy2eeyore.h src/back/sysy2eeyore.cpp src/define/tigger.h src/front/operator.cpp src/front/operator.h src/back/eeyore2tigger.cpp src/back/eeyore2tigger.h)