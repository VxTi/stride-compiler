//
// Created by Luca Warmenhoven on 11/09/2024.
//

#include "../ASTNodes.h"
#include "definitions/NExpression.h"

NExpression *NExpression::parse(TokenSet &tokenSet)
{
    auto nstExpression = new NExpression();


    return nstExpression;
}

void NExpression::parse(TokenSet &tokenSet, Node &parent)
{
    parent.addChild(NExpression::parse(tokenSet));
}