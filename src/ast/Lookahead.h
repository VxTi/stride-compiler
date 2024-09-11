//
// Created by Luca Warmenhoven on 11/09/2024.
//

#ifndef STRIDE_LANGUAGE_LOOKAHEAD_H
#define STRIDE_LANGUAGE_LOOKAHEAD_H

#include "../tokens/TokenSet.h"

namespace stride::ast
{

    /**
     * Captures a block of tokens that is enclosed by the provided start and end tokens.
     * @param stream The stream to capture the block from.
     * @param start The token that starts the block.
     * @param end The token that ends the block.
     * @return A TokenSet containing the block of tokens.
     */
    TokenSet *captureBlock(TokenSet &stream, token_type_t start, token_type_t end);

    /**
     * Returns the distance to the provided token.
     * @param stream The stream to search in.
     * @param token The token to search for.
     * @return An integer representing the distance to the token.
     */
    int distanceNextToken(TokenSet &stream, token_type_t token);

    /**
     * Returns the distance to the provided token, excluding scope tokens.
     * @param stream
     * @param token
     * @return
     */
    int distanceNextTokenInScope(TokenSet &stream, token_type_t token);

}

#endif //STRIDE_LANGUAGE_LOOKAHEAD_H
