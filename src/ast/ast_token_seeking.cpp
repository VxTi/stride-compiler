//
// Created by Luca Warmenhoven on 21/08/2024.
//

#include "ast.h"

using namespace stride::ast;

/**
 * Checks if there is a next token in the token stream.
 * @return True if there is a next token, false otherwise.
 */
bool stride::ast::hasNext(ast_token_set_t &token_set, cursor_t index)
{
    return index + 1 < token_set.token_count;
}

/**
 * Whether there is a token before the current one
 * @return
 */
bool stride::ast::hasPrevious(cursor_t index)
{
    return index > 0;
}

/**
 * Returns the next token in the token stream, from
 * the current index.
 * @return
 */
token_t *stride::ast::next(ast_token_set_t &token_set, cursor_t index)
{
    if ( !hasNext(token_set, index))
    {
        return nullptr;
    }

    return &token_set.tokens[ index + 1 ];
}

/**
 * Returns the index of the first token of the provided properties.
 * @param token_set The token set to search in.
 * @param type The properties to search for.
 * @param token_count The amount of tokens in the subset.
 * @param from_index_incl The index to start searching from.
 * @return The index of the token, or -1 if not found.
 */
int stride::ast::indexOfToken(ast_token_set_t &token_set, token_type_t type, int token_count, int from_index_incl)
{
    for ( int i = from_index_incl; i < token_count; i++ )
    {
        if ( token_set.tokens[ i ].type == type )
        {
            return i;
        }
    }
    return -1;
}

/**
 * Returns the previous token in the token stream, from the
 * current index.
 * @return
 */
token_t *stride::ast::previous(ast_token_set_t &token_set, cursor_t index)
{
    if ( !hasPrevious(index))
    {
        return nullptr;
    }

    return &token_set.tokens[ index - 1 ];
}

/**
 * Returns the token at the nth offset from the cursor position,
 * e.g. cursor = 0, peak(1) will return the token at position 1. (cursor + n)
 * @param offset
 * @return
 */
token_t *stride::ast::peak(ast_token_set_t &token_set, cursor_t index, int offset)
{
    int i = index + offset;
    if ( i < 0 || i >= token_set.token_count )
    {
        return nullptr;
    }

    return &token_set.tokens[ i ];
}

/**
 * Whether the previous token is of the provided properties.
 * If there are no tokens left, the function will return false.
 * @param type The properties to check for
 * @return
 */
bool stride::ast::isPrevious(ast_token_set_t &token_set, token_type_t type, cursor_t index)
{
    if ( !hasPrevious(index))
    {
        return false;
    }

    return previous(token_set, index)->type == type;
}

/**
 * Whether the next token is of the provided properties.
 * If there are no tokens left, the function will return false.
 * @param type The properties to check for
 * @return
 */
bool stride::ast::isNext(ast_token_set_t &token_set, token_type_t type, cursor_t index)
{
    if ( !hasNext(token_set, index))
    {
        return false;
    }

    return next(token_set, index)->type == type;
}

/**
 * Checks whether the provided properties is within the provided range,
 * from the current index. If one wants to check if there's a token of a specific properties
 * within the next -3n || +3n of the cursor, this function is yours.
 * If range = 0, the function will check whether the current token is of the provided properties.
 * @param type The properties to check
 * @param range The offset to provide
 * @return
 */
bool stride::ast::isInRange(ast_token_set_t &token_set, token_type_t type, cursor_t index, int range)
{
    if ( range == 0 )
    {
        return token_set.tokens[ index ].type == type;
    }

    long start = index;
    long end = index + range;


    // Swap range if range is negative
    if ( start > end )
    {
        std::swap(start, end);
    }

    // Check whether token properties is in range
    for ( cursor_t i = start; i < end; i++ )
    {
        if ( token_set.tokens[ i ].type == type )
        {
            return true;
        }
    }
    return false;
}