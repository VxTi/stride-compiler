cmake_minimum_required(VERSION 3.28)
project(stride_language)

set(CMAKE_CXX_STANDARD 20)

add_executable(stride_language src/main.cpp
        src/analysis/token.h
        src/analysis/lexer.cpp
        src/analysis/lexer.h
        src/analysis/token.cpp
        src/ast/ast.cpp
        src/ast/ast.h
        src/ast/ast_token_seeking.cpp
        src/ast/ast_error_handling.cpp
        src/ast/syntax_parsing/expressions.cpp
        src/ast/syntax_parsing/code_blocks.cpp
        src/ast/syntax_parsing/shared_statement.cpp
        src/ast/syntax_parsing/import_statement.cpp
        src/ast/syntax_parsing/function_declaration.cpp
        src/ast/syntax_parsing/variable_declaration.cpp
)
