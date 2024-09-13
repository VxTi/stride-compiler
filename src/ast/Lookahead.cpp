//
// Created by Luca Warmenhoven on 24/08/2024.
//

#include "Lookahead.h"

using namespace stride::ast;

TokenSet *stride::ast::captureBlock(TokenSet &set, token_type_t startToken, token_type_t endToken)
{
    set.consumeRequired(startToken, "Expected opening token after statement. This could be due to missing closing token.");

    int blockLength = 0,
            startingIndex = set.getIndex(), // starts after the opening token
    branchDepth = 1;

    for ( ; !set.end(); )
    {
        token_type_t currentType = set.next().type;

        if ( currentType == startToken )
        {
            branchDepth++;
        }

        if ( currentType == endToken )
        {
            if ( --branchDepth == 0 )
            {
                break;
            }

            if ( branchDepth < 0 )
            {
                set.error("Imbalanced closure");
            }
        }

        blockLength++;
    }

    // If there aren't any set skipped, there's no closure to append.
    return set.subset(startingIndex, blockLength);
}