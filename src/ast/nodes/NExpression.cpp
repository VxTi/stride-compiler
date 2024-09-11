//
// Created by Luca Warmenhoven on 11/09/2024.
//

#include "../ASTNodes.h"
#include "definitions/NExpression.h"
#include "../Lookahead.h"

NExpression *NExpression::parse(TokenSet &tokenSet)
{
    auto nstExpression = new NExpression();

    do {
        // Do something.
        tokenSet.next();
    } while (tokenSet.hasNext() && !tokenSet.canConsume(TOKEN_SEMICOLON));

    return nstExpression;
}

void NExpression::parse(TokenSet &tokenSet, Node &parent)
{
    parent.addChild(NExpression::parse(tokenSet));
}

NExpression *NExpression::captureParenthesis(TokenSet &tokenSet)
{
    auto expressionSubset = stride::ast::captureBlock(tokenSet, TOKEN_LPAREN, TOKEN_RPAREN);

    if (expressionSubset == nullptr)
    {
        tokenSet.error("Expected opening and closing brackets after 'while' keyword.");
        return nullptr;
    }

    return NExpression::parse(*expressionSubset);
}