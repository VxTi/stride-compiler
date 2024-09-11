//
// Created by Luca Warmenhoven on 11/09/2024.
//

#include "definitions/NThrowStatement.h"

void NThrowStatement::parse(TokenSet &tokenSet, stride::ast::Node &parent)
{
    tokenSet.consumeRequired(TOKEN_KEYWORD_THROW, "Throw statement requires 'throw' keyword.");

    auto nstExpression = new NExpression();
    auto nstThrow = new NThrowStatement(nstExpression);

    parent.addChild(nstThrow);
}