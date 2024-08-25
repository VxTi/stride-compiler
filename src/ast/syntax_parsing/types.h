//
// Created by Luca Warmenhoven on 25/08/2024.
//

#ifndef STRIDE_LANGUAGE_TYPES_H
#define STRIDE_LANGUAGE_TYPES_H

#include "../ast.h"

using namespace stride::ast;

/**
 * Returns whether the provided token type is an integer,
 * can be from 8 to 64 bit.
 * @param type The type to check
 * @return An integer (bool) representing whether the type is an integer.
 */
inline int type_is_integer(token_type_t type)
{
    switch (type)
    {
        case TOKEN_PRIMITIVE_INT64:
        case TOKEN_PRIMITIVE_INT32:
        case TOKEN_PRIMITIVE_INT16:
        case TOKEN_PRIMITIVE_INT8:
        case TOKEN_NUMBER_INTEGER:
            return true;
        default:
            return false;
    }
}

#endif //STRIDE_LANGUAGE_TYPES_H
