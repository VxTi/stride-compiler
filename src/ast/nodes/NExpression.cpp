//
// Created by Luca Warmenhoven on 11/09/2024.
//

#include "../ASTNodes.h"

using namespace stride::ast;

NExpression *NExpression::parse(TokenSet &tokenSet)
{
    auto nstExpression = new NExpression();


    return nstExpression;
}

void stride::ast::NExpression::parse(TokenSet &tokenSet, Node &parent)
{
    parent.addChild(NExpression::parse(tokenSet));
}