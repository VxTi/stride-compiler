//
// Created by Luca Warmenhoven on 21/08/2024.
//

#include "ast.h"

using namespace stride::ast;

/**
 * Checks if there is a next token in the token stream.
 * @return True if there is a next token, false otherwise.
 */
bool stride::ast::has_next(ast_token_set_t &token_set, cursor_t index)
{
    return index + 1 < token_set.token_count;
}

/**
 * Whether there is a token before the current one
 * @return
 */
bool stride::ast::has_previous(cursor_t index)
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
    if ( !has_next(token_set, index))
    {
        return nullptr;
    }

    return &token_set.tokens[ index + 1 ];
}

/**
 * Returns the previous token in the token stream, from the
 * current index.
 * @return
 */
token_t *stride::ast::previous(ast_token_set_t &token_set, cursor_t index)
{
    if ( !has_previous(index))
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

int stride::ast::peakeq(ast_token_set_t &token_set, cursor_t index, int offset, token_type_t type)
{
    token_t *peaked = peak(token_set, index, offset);

    if ( peaked == nullptr )
    {
        return 0;
    }

    return peaked->type == type;
}

/**
 * Whether the previous token is of the provided properties.
 * If there are no tokens left, the function will return false.
 * @param type The properties to check for
 * @return
 */
bool stride::ast::is_previous(ast_token_set_t &token_set, token_type_t type, cursor_t index)
{
    if ( !has_previous(index))
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
bool stride::ast::is_next(ast_token_set_t &token_set, token_type_t type, cursor_t index)
{
    if ( !has_next(token_set, index))
    {
        return false;
    }

    return next(token_set, index)->type == type;
}