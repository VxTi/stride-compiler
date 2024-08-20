cmake_minimum_required(VERSION 3.28)
project(stride_language)

set(CMAKE_CXX_STANDARD 20)

add_executable(stride_language src/main.cpp
        src/analysis/token.h
        src/analysis/lexer.cpp
        src/analysis/lexer.h
        src/analysis/token.cpp
        src/parsing/AST.cpp
        src/parsing/AST.h
        src/analysis/expression.h)
