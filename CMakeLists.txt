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
        src/ast/nodes/definitions/NLiteral.h
        src/ast/nodes/definitions/NIdentifier.h
        src/ast/nodes/definitions/NBlock.h
        src/ast/nodes/definitions/NExpression.h
        src/ast/nodes/definitions/NArray.h
        src/ast/nodes/definitions/NReturnStatement.h
        src/ast/nodes/definitions/NThrowStatement.h
        src/ast/nodes/definitions/NBinaryOperation.h
        src/ast/nodes/definitions/NUnaryOperator.h
        src/ast/nodes/definitions/NModuleDeclaration.h
        src/ast/nodes/definitions/NVariableDeclaration.h
        src/ast/nodes/definitions/NTryCatchStatement.h
        src/ast/nodes/definitions/NFunctionDeclaration.h
        src/ast/nodes/definitions/NFunctionCall.h
        src/ast/nodes/definitions/NOperatorOverload.h
        src/ast/nodes/definitions/NEnumerableDeclaration.h
        src/ast/nodes/definitions/NStructureDeclaration.h
        src/ast/nodes/definitions/NConditionalStatement.h
        src/ast/nodes/definitions/NSwitchStatement.h
        src/ast/nodes/definitions/NClassDeclaration.h
        src/ast/nodes/definitions/NImportStatement.h
        src/ast/nodes/definitions/NWhileLoop.h
        src/ast/nodes/definitions/NDoWhileLoop.h
        src/ast/nodes/definitions/NForLoop.h

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
        src/ast/nodes/NStructureDeclaration.cpp
        src/ast/nodes/NEnumerableDeclaration.cpp
        src/ast/nodes/NImportStatement.cpp
        src/ast/nodes/NModuleDeclaration.cpp
        src/ast/nodes/NForLoop.cpp
        src/ast/nodes/NTryCatchStatement.cpp
        src/ast/nodes/NWhileLoop.cpp
        src/ast/nodes/NDoWhileLoop.cpp
        src/ast/nodes/NThrowStatement.cpp
        src/ast/nodes/NBlock.cpp
        src/ast/nodes/NReturnStatement.cpp
        src/ast/nodes/NSwitchStatement.cpp
        src/ast/nodes/NConditionalStatement.cpp
        src/ast/nodes/NBinaryOperator.cpp
        src/ast/Reducible.h
        src/ast/nodes/NLiteral.cpp
)
