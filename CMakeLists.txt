cmake_minimum_required(VERSION 3.29.6)
project(stride_language)

# Include standard library path
include_directories(/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -isystem /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include/c++/v1")

add_executable(stride_language src/main.cpp
        src/tokens/token.h
        src/tokens/tokenizer.cpp
        src/tokens/tokenizer.h
        src/tokens/token.cpp
        src/ast/ast_token_seeking.cpp
        src/ast/ast_error_handling.cpp
        src/ast/syntax/expressions.cpp
        src/ast/abstractions/Lookahead.cpp
        src/ast/syntax/module_statement.cpp
        src/ast/syntax/import_statement.cpp
        src/ast/abstractions/NFunctionDeclaration.cpp
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
        src/ast/abstractions/NClassDeclaration.cpp
        src/ast/syntax/switch_case.cpp
        src/ast/ast_error_handling.h
        src/ast/syntax/generics.cpp
        src/ast/syntax/structures.cpp
        src/ast/syntax/returning.cpp
        src/ast/abstractions/ASTNodeAbstractions.h
        src/ast/abstractions/TokenSet.cpp
        src/ast/abstractions/TokenSet.h
        src/ast/TokenPatterns.cpp
        src/ast/TokenPatterns.h
        src/ast/TokenPatterns.h
        src/ast/abstractions/StrideFile.cpp
        src/ast/abstractions/StrideFile.h
        src/ast/abstractions/ASTParser.cpp
        src/ast/abstractions/Lookahead.h
)
