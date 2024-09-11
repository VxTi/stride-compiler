//
// Created by Luca Warmenhoven on 11/09/2024.
//

#include "definitions/NWhileLoop.h"

void NWhileLoop::parse(TokenSet &tokenSet, stride::ast::Node &parent)
{
    tokenSet.consumeRequired(TOKEN_KEYWORD_WHILE, "Expected 'while' keyword in while loop.");

    auto nstWhileLoop = new NWhileLoop();
    nstWhileLoop->condition = NExpression::captureParenthesis(tokenSet);
    nstWhileLoop->body = NBlock::capture(tokenSet);
    tokenSet.consumeRequired(TOKEN_SEMICOLON, "Expected semicolon after while loop.");
}