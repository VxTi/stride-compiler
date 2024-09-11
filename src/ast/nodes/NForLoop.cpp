//
// Created by Luca Warmenhoven on 11/09/2024.
//

#include "definitions/NForLoop.h"

void NForLoop::parse(TokenSet &tokenSet, stride::ast::Node &parent)
{
    tokenSet.consumeRequired(TOKEN_KEYWORD_FOR, "Expected 'for' keyword in for loop.");

    auto nstForLoop = new NForLoop();

    // TODO: Add support for for loop initialization.
    auto conditionBlock = NExpression::captureParenthesis(tokenSet);

    nstForLoop->body = NBlock::capture(tokenSet);

    parent.addChild(nstForLoop);
}