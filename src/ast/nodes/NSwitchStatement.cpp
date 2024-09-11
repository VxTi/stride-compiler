//
// Created by Luca Warmenhoven on 11/09/2024.
//

#include "definitions/NSwitchStatement.h"
#include "../NodeProperties.h"

void NSwitchStatement::parse(TokenSet &tokenSet, stride::ast::Node &parent)
{
    tokenSet.consumeRequired(TOKEN_KEYWORD_SWITCH, "Expected 'switch' keyword in switch statement.");

    auto nstSwitch = new NSwitchStatement();

    nstSwitch->expression = NExpression::captureParenthesis(tokenSet);

    auto switchBodySet = NBlock::captureRaw(tokenSet);
    do {
        if (switchBodySet->consume(TOKEN_KEYWORD_CASE))
        {
            if (!stride::ast::validateLiteralValue(*switchBodySet))
            {
                switchBodySet->error("Expected valid literal value after 'case' keyword.");
                return;
            }

            auto nstCase = new NSwitchCase();
            nstCase->conditionalValue = new NLiteral(switchBodySet->next().value);
            switchBodySet->consumeRequired(TOKEN_DASH_RARROW, "Expected '->' after case value.");
            nstCase->body = NBlock::capture(*switchBodySet);
        }

    } while (switchBodySet->hasNext());

    parent.addChild(nstSwitch);
}