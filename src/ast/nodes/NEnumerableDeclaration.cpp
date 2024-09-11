//
// Created by Luca Warmenhoven on 11/09/2024.
//

#include "../ASTNodes.h"
#include "../Lookahead.h"

using namespace stride::ast;

void NEnumerableDeclaration::parse(TokenSet &tokenSet, Node &parent)
{
    tokenSet.consumeRequired(TOKEN_KEYWORD_ENUM, "Enumerable declaration requires 'enum' keyword.");

    auto enumSubSet = captureBlock(tokenSet, TOKEN_LBRACE, TOKEN_RBRACE);

    if ( enumSubSet == nullptr )
    {
        tokenSet.error("Enumerable requires opening and closing brackets after declaration.");
        return;
    }

    auto nstEnumDecl = new NEnumerableDeclaration();

    int enumMemberId = 0;

    do
    {
        token_t identifier = tokenSet.consumeRequired(TOKEN_IDENTIFIER,
                                                      "Enumerable member declaration requires identifier.");
        // If enum member has assignment, the value must be of integer type.
        if ( tokenSet.consume(TOKEN_EQUALS))
        {
            token_t value = tokenSet.consumeRequired(TOKEN_NUMBER_INTEGER,
                                                     "Enumerable member value declaration must be of integer type.");
            enumMemberId = atoi(value.value);
        }

        nstEnumDecl->addValue(identifier.value, enumMemberId++);

        tokenSet.consumeRequired(TOKEN_SEMICOLON, "Enumerable members must be ");

    } while ( enumSubSet->consume(TOKEN_COMMA));

    // Since all members must have been processed, there's not supposed
    // to be any tokens remaining in the subset.
    if (enumSubSet->hasNext())
    {
        tokenSet.error("Illegal trailing token.");
        return;
    }

    parent.addChild(nstEnumDecl);
}