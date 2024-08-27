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
        src/ast/syntax/expressions.cpp
        src/ast/syntax/code_blocks.cpp
        src/ast/syntax/shared_statement.cpp
        src/ast/syntax/import_statement.cpp
        src/ast/syntax/function_declaration.cpp
        src/ast/syntax/variable_declaration.cpp
        src/ast/syntax/identifiers.cpp
        src/ast/syntax/enumerables.cpp
        src/ast/syntax/variable_types.h
        src/ast/syntax/for_loop.cpp
        src/ast/syntax/function_call.cpp
        src/ast/syntax/while_loop.cpp
        src/ast/syntax/try_catch.cpp
        src/ast/syntax/variable_types.cpp
        src/ast/syntax/do_while.cpp
        src/ast/syntax/if_else.cpp
        src/ast/syntax/throw.cpp
        src/ast/syntax/validation/syntax_validation.cpp
        src/ast/syntax/validation/syntax_validation.h
)
