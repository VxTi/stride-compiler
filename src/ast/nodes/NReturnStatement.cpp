//
// Created by Luca Warmenhoven on 11/09/2024.
//

#include "definitions/NReturnStatement.h"

void NReturnStatement::parse(TokenSet &tokenSet, Node &parent)
{
    tokenSet.consumeRequired(TOKEN_KEYWORD_RETURN, "Return statement requires 'return' keyword.");

    auto nstReturn = new NReturnStatement();

    if ( tokenSet.consume(TOKEN_SEMICOLON))
    {
        parent.addChild(nstReturn);
        return;
    }

    nstReturn->expression = NExpression::parse(tokenSet);
    parent.addChild(nstReturn);
}