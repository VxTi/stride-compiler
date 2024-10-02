//
// Created by Luca Warmenhoven on 11/09/2024.
//

#include "definitions/NDoWhileLoop.h"

void NDoWhileLoop::parse(TokenSet &tokenSet, stride::ast::Node &parent)
{
    tokenSet.consumeRequired(TOKEN_KEYWORD_DO, "Expected 'do' keyword in do-while loop.");

    auto nstDoWhileDecl = new NDoWhileLoop();
    nstDoWhileDecl->body = NBlock::capture(tokenSet); // Body after 'do'

    // Required 'while' keyword
    tokenSet.consumeRequired(TOKEN_KEYWORD_WHILE, "Expected 'while' keyword in do-while loop.");

    // Required '(' <expression> ')' after 'while'
    nstDoWhileDecl->condition = NExpression::captureParenthesis(tokenSet);
    tokenSet.consumeRequired(TOKEN_SEMICOLON, "Expected semicolon after do-while loop.");

    parent.addChild(nstDoWhileDecl);
}