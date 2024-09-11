//
// Created by Luca Warmenhoven on 11/09/2024.
//


#include "NodeProperties.h"


void stride::ast::parseGenerics(TokenSet &tokenSet, std::vector<NIdentifier *> &genericsDst)
{
    if (tokenSet.consume(TOKEN_LARROW))
    {
        genericsDst.push_back(new NIdentifier(tokenSet.consumeRequired(TOKEN_IDENTIFIER, "Expected generic identifier after '<'.").value));

        while (tokenSet.consume(TOKEN_COMMA))
        {
            genericsDst.push_back(new NIdentifier(tokenSet.consumeRequired(TOKEN_IDENTIFIER, "Expected generic identifier after ','.").value));
        }
        tokenSet.consumeRequired(TOKEN_RARROW, "Expected '>' after generic declaration.");
    }
}