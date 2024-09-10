//
// Created by Luca Warmenhoven on 11/09/2024.
//

#include "ASTNodeAbstractions.h"
#include <cstdio>

using namespace stride::ast;

Node *parse(TokenSet &tokenSet)
{
    auto *root = new Node();
    parse(tokenSet, *root);
    return root;
}

void parse(TokenSet &tokenSet, stride::ast::Node &root)
{
    for ( token_t token = tokenSet.current(); tokenSet.hasNext(); )
    {
        if ( tokenSet.consume(static_cast<token_type_t>(0))) // skip comments (0)
        {
            continue;
        }

        switch (token.type)
        {
            case TOKEN_KEYWORD_DEFINE: NFunctionDeclaration::parse(tokenSet, root);   break;
            case TOKEN_KEYWORD_IF:     NConditionalStatement::parse(tokenSet, root);  break;
            case TOKEN_KEYWORD_LET:
            case TOKEN_KEYWORD_CONST:  NVariableDeclaration::parse(tokenSet, root);   break;
            case TOKEN_KEYWORD_IMPORT: NImportStatement::parse(tokenSet, root);       break;
            case TOKEN_KEYWORD_CLASS:  NClassDeclaration::parse(tokenSet, root);      break;
            case TOKEN_KEYWORD_STRUCT: NStructureDeclaration::parse(tokenSet, root);  break;
            case TOKEN_KEYWORD_SWITCH: NSwitchStatement::parse(tokenSet, root);       break;
            case TOKEN_KEYWORD_TRY:    NTryCatchStatement::parse(tokenSet, root);     break;
            case TOKEN_KEYWORD_ENUM:   NEnumerableDeclaration::parse(tokenSet, root); break;
            case TOKEN_KEYWORD_DO:     NDoWhileLoop::parse(tokenSet, root);           break;
            case TOKEN_KEYWORD_WHILE:  NWhileLoop::parse(tokenSet, root);             break;
            case TOKEN_KEYWORD_FOR:    NForLoop::parse(tokenSet, root);               break;
            case TOKEN_KEYWORD_RETURN: NReturnStatement::parse(tokenSet, root);       break;
            case TOKEN_KEYWORD_THROW:  NThrowStatement::parse(tokenSet, root);        break;
            case TOKEN_KEYWORD_MODULE: NModuleDeclaration::parse(tokenSet, root);     break;
            case TOKEN_IDENTIFIER:     NIdentifier::parse(tokenSet, root);            break;
            default:
                fprintf(stderr, "Unexpected token %s\n", tokenSet.current().value);
                exit(1);
        }
    }
}