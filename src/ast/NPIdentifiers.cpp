//
// Created by Luca Warmenhoven on 11/09/2024.
//

#include "NodeProperties.h"
#include "../error/ast_error_handling.h"

stride::ast::NIdentifier *stride::ast::parseIdentifier(TokenSet &tokenSet)
{
    if ( !tokenSet.canConsume(TOKEN_IDENTIFIER))
    {
        stride::error::error(tokenSet.getSource(), tokenSet.getIndex(), "Expected identifier");
    }
    std::string identifierName = "";
    token_t next_token;
    do
    {
        next_token = tokenSet.consumeRequired(TOKEN_IDENTIFIER, "Expected identifier after double colon.");
        identifierName.append("__").append(next_token.value);
    } while ( tokenSet.consume(TOKEN_DOUBLE_COLON));
    return new NIdentifier(identifierName);
}