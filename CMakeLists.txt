cmake_minimum_required(VERSION 3.19)
project(SysYCompiler)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-register")

find_package(BISON)
find_package(FLEX)
BISON_TARGET(parser src/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp)
FLEX_TARGET(scanner src/scanner.l ${CMAKE_CURRENT_BINARY_DIR}/scanner.cpp)
ADD_FLEX_BISON_DEPENDENCY(scanner parser)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(src src/define src/front src/back)

add_executable(SysYCompiler ${BISON_parser_OUTPUTS} ${FLEX_scanner_OUTPUTS}
        src/main.cpp src/sysy.h src/nested_map.h src/eeyore.h src/gen_eeyore.h src/gen_eeyore.cpp src/tigger.h src/operator.cpp src/operator.h)