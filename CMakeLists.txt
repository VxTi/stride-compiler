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
        src/error/ast_error_handling.cpp
        src/ast/Lookahead.cpp
        #[[src/ast/syntax/expressions.cpp
        src/ast/syntax/module_statement.cpp
        src/ast/syntax/import_statement.cpp
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
        src/ast/syntax/switch_case.cpp
        src/ast/syntax/generics.cpp
        src/ast/syntax/structures.cpp
        src/ast/syntax/returning.cpp]]
        src/ast/nodes/NFunctionDeclaration.cpp
        src/ast/nodes/NClassDeclaration.cpp
        src/ast/ASTNodes.h
        src/tokens/TokenSet.cpp
        src/tokens/TokenSet.h
        src/error/ast_error_handling.h
        src/StrideFile.cpp
        src/StrideFile.h
        src/ast/ASTParser.cpp
        src/ast/Lookahead.h
        src/ast/NodeProperties.h
        src/ast/NPGenerics.cpp
        src/ast/NPIdentifiers.cpp
        src/ast/NPTypes.cpp
        src/ast/nodes/NVariableDeclaration.cpp
        src/ast/nodes/NExpression.cpp
)
