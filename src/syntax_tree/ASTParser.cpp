//
// Created by Luca Warmenhoven on 11/09/2024.
//

#include "ASTNodes.h"
#include "node_types/definitions/NFunctionDeclaration.h"
#include "node_types/definitions/NConditionalStatement.h"
#include "node_types/definitions/NImportStatement.h"
#include "node_types/definitions/NClassDeclaration.h"
#include "node_types/definitions/NSwitchStatement.h"
#include "node_types/definitions/NStructureDeclaration.h"
#include "node_types/definitions/NTryCatchStatement.h"
#include "node_types/definitions/NForLoop.h"
#include "node_types/definitions/NWhileLoop.h"
#include "node_types/definitions/NDoWhileLoop.h"
#include "node_types/definitions/NEnumerableDeclaration.h"
#include "node_types/definitions/NReturnStatement.h"
#include "node_types/definitions/NThrowStatement.h"
#include "node_types/definitions/NModuleDeclaration.h"

stride::ast::Node *stride::ast::parser::parse(TokenSet &tokenSet)
{
    auto *root = new Node();
    parser::parse(tokenSet, *root);
    return root;
}

void stride::ast::parser::parse(TokenSet &tokenSet, stride::ast::Node &root)
{
    for ( ; tokenSet.hasNext(); )
    {
        tokenSet.consume(static_cast<token_type_t>(0)); // skip whitespace and comments

        switch ( tokenSet.current().type )
        {
            case TOKEN_KEYWORD_DEFINE:
                NFunctionDeclaration::parse(tokenSet, root);
                break;
            case TOKEN_KEYWORD_IF:
                NConditionalStatement::parse(tokenSet, root);
                break;
            case TOKEN_KEYWORD_LET:
            case TOKEN_KEYWORD_CONST:
                NVariableDeclaration::parse(tokenSet, root);
                break;
            case TOKEN_KEYWORD_IMPORT:
                NImportStatement::parse(tokenSet, root);
                break;
            case TOKEN_KEYWORD_PUBLIC:
            case TOKEN_KEYWORD_CLASS:
                NClassDeclaration::parse(tokenSet, root);
                break;
            case TOKEN_KEYWORD_STRUCT:
                NStructureDeclaration::parse(tokenSet, root);
                break;
            case TOKEN_KEYWORD_SWITCH:
                NSwitchStatement::parse(tokenSet, root);
                break;
            case TOKEN_KEYWORD_TRY:
                NTryCatchStatement::parse(tokenSet, root);
                break;
            case TOKEN_KEYWORD_ENUM:
                NEnumerableDeclaration::parse(tokenSet, root);
                break;
            case TOKEN_KEYWORD_DO:
                NDoWhileLoop::parse(tokenSet, root);
                break;
            case TOKEN_KEYWORD_WHILE:
                NWhileLoop::parse(tokenSet, root);
                break;
            case TOKEN_KEYWORD_FOR:
                NForLoop::parse(tokenSet, root);
                break;
            case TOKEN_KEYWORD_RETURN:
                NReturnStatement::parse(tokenSet, root);
                break;
            case TOKEN_KEYWORD_THROW:
                NThrowStatement::parse(tokenSet, root);
                break;
            case TOKEN_KEYWORD_MODULE:
                NModuleDeclaration::parse(tokenSet, root);
                break;
            case TOKEN_SEMICOLON:
                tokenSet.next();
                break;
            case TOKEN_LBRACE:
                root.addChild(NBlock::capture(tokenSet));
                break;
            default:
                // Attempt to parse expression.
                NExpression::parse(tokenSet, root);
                break;
        }
    }
}