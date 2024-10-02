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
        src/syntax_tree/Lookahead.cpp
        src/syntax_tree/node_types/definitions/NLiteral.h
        src/syntax_tree/node_types/definitions/NIdentifier.h
        src/syntax_tree/node_types/definitions/NBlock.h
        src/syntax_tree/node_types/definitions/NExpression.h
        src/syntax_tree/node_types/definitions/NArray.h
        src/syntax_tree/node_types/definitions/NReturnStatement.h
        src/syntax_tree/node_types/definitions/NThrowStatement.h
        src/syntax_tree/node_types/definitions/NBinaryOperation.h
        src/syntax_tree/node_types/definitions/NUnaryOperator.h
        src/syntax_tree/node_types/definitions/NModuleDeclaration.h
        src/syntax_tree/node_types/definitions/NVariableDeclaration.h
        src/syntax_tree/node_types/definitions/NTryCatchStatement.h
        src/syntax_tree/node_types/definitions/NFunctionDeclaration.h
        src/syntax_tree/node_types/definitions/NFunctionCall.h
        src/syntax_tree/node_types/definitions/NOperatorOverload.h
        src/syntax_tree/node_types/definitions/NEnumerableDeclaration.h
        src/syntax_tree/node_types/definitions/NStructureDeclaration.h
        src/syntax_tree/node_types/definitions/NConditionalStatement.h
        src/syntax_tree/node_types/definitions/NSwitchStatement.h
        src/syntax_tree/node_types/definitions/NClassDeclaration.h
        src/syntax_tree/node_types/definitions/NImportStatement.h
        src/syntax_tree/node_types/definitions/NWhileLoop.h
        src/syntax_tree/node_types/definitions/NDoWhileLoop.h
        src/syntax_tree/node_types/definitions/NForLoop.h

        src/syntax_tree/node_types/NFunctionDeclaration.cpp
        src/syntax_tree/node_types/NClassDeclaration.cpp
        src/syntax_tree/ASTNodes.h
        src/tokens/TokenSet.cpp
        src/tokens/TokenSet.h
        src/error/ast_error_handling.h
        src/StrideFile.cpp
        src/StrideFile.h
        src/syntax_tree/ASTParser.cpp
        src/syntax_tree/Lookahead.h
        src/syntax_tree/NodeProperties.h
        src/syntax_tree/NPGenerics.cpp
        src/syntax_tree/NPIdentifiers.cpp
        src/syntax_tree/NPTypes.cpp
        src/syntax_tree/node_types/NVariableDeclaration.cpp
        src/syntax_tree/node_types/NExpression.cpp
        src/syntax_tree/node_types/NStructureDeclaration.cpp
        src/syntax_tree/node_types/NEnumerableDeclaration.cpp
        src/syntax_tree/node_types/NImportStatement.cpp
        src/syntax_tree/node_types/NModuleDeclaration.cpp
        src/syntax_tree/node_types/NForLoop.cpp
        src/syntax_tree/node_types/NTryCatchStatement.cpp
        src/syntax_tree/node_types/NWhileLoop.cpp
        src/syntax_tree/node_types/NDoWhileLoop.cpp
        src/syntax_tree/node_types/NThrowStatement.cpp
        src/syntax_tree/node_types/NBlock.cpp
        src/syntax_tree/node_types/NReturnStatement.cpp
        src/syntax_tree/node_types/NSwitchStatement.cpp
        src/syntax_tree/node_types/NConditionalStatement.cpp
        src/syntax_tree/node_types/NBinaryOperator.cpp
        src/syntax_tree/Reducible.h
        src/syntax_tree/node_types/NLiteral.cpp
)
