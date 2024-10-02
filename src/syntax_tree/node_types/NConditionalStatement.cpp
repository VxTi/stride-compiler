//
// Created by Luca Warmenhoven on 11/09/2024.
//

#include "definitions/NConditionalStatement.h"

void NConditionalStatement::parse(TokenSet &tokenSet, stride::ast::Node &parent)
{
    tokenSet.consumeRequired(TOKEN_KEYWORD_IF, "If statement requires 'if' keyword.");

    // Initial 'if' statement
    auto *conditionalStatement = new NConditionalStatement();
    conditionalStatement->setCondition(NExpression::parse(tokenSet));
    conditionalStatement->setThen(NBlock::capture(tokenSet));

    // Whether the chained 'if' statement has a final 'else' that was consumed.
    bool consumedElse;
    auto *lastConditional = conditionalStatement;

    // Chained if, else if/elif statements
    while (( consumedElse = tokenSet.consume(TOKEN_KEYWORD_ELSE)) && tokenSet.canConsume(TOKEN_KEYWORD_IF))
    {
        auto chainedConditionalStatement = new NConditionalStatement();

        auto otherwiseBlock = new NBlock();
        otherwiseBlock->addChild(chainedConditionalStatement);
        lastConditional->setElse(otherwiseBlock);

        chainedConditionalStatement->setCondition(NExpression::parse(tokenSet));
        chainedConditionalStatement->setThen(NBlock::capture(tokenSet));

        lastConditional = chainedConditionalStatement;
    }

    if ( consumedElse )
    {
        lastConditional->setElse(NBlock::capture(tokenSet));
    }

    parent.addChild(conditionalStatement);
}