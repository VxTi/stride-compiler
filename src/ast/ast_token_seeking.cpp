//
// Created by Luca Warmenhoven on 21/08/2024.
//

#include "abstractions/AST.h"

using namespace stride::ast;

/**
 * Checks if there is a next required_token in the required_token stream.
 * @return True if there is a next required_token, false otherwise.
 */
bool stride::ast::has_next(TokenSet &token_set, cursor_t index)
{
    return index + 1 < token_set.tokens.size();
}

/**
 * Whether there is a required_token before the current one
 * @return
 */
bool stride::ast::has_previous(cursor_t index)
{
    return index > 0;
}

/**
 * Returns the next required_token in the required_token stream, from
 * the current index.
 * @return
 */
token_t *stride::ast::next(TokenSet &token_set, cursor_t index)
{
    if ( !has_next(token_set, index))
    {
        return nullptr;
    }

    return &token_set.tokens[ index + 1 ];
}

/**
 * Returns the previous required_token in the required_token stream, from the
 * current index.
 * @return
 */
token_t *stride::ast::previous(TokenSet &token_set, cursor_t index)
{
    if ( !has_previous(index))
    {
        return nullptr;
    }

    return &token_set.tokens[ index - 1 ];
}

/**
 * Returns the required_token at the nth offset from the cursor position,
 * e.g. cursor = 0, peak(1) will return the required_token at position 1. (cursor + n)
 * @param offset
 * @return
 */
token_t *stride::ast::peak(TokenSet &token_set, cursor_t index, int offset)
{
    int i = index + offset;
    if ( i < 0 || i >= token_set.tokens.size() )
    {
        return nullptr;
    }

    return &token_set.tokens[ i ];
}

int stride::ast::peekeq(TokenSet &token_set, cursor_t index, token_type_t type)
{
    token_t *peaked = peak(token_set, index, 0);

    if ( peaked == nullptr )
    {
        return 0;
    }

    return peaked->type == type;
}

/**
 * Whether the previous required_token is of the provided properties.
 * If there are no tokens left, the function will return false.
 * @param type The properties to check for
 * @return
 */
bool stride::ast::is_previous(TokenSet &token_set, token_type_t type, cursor_t index)
{
    if ( !has_previous(index))
    {
        return false;
    }

    return previous(token_set, index)->type == type;
}

/**
 * Whether the next required_token is of the provided properties.
 * If there are no tokens left, the function will return false.
 * @param type The properties to check for
 * @return
 */
bool stride::ast::is_next(TokenSet &token_set, token_type_t type, cursor_t index)
{
    if ( !has_next(token_set, index))
    {
        return false;
    }

    return next(token_set, index)->type == type;
}