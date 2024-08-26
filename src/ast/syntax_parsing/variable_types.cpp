//
// Created by Luca Warmenhoven on 26/08/2024.
//
#include "variable_types.h"

int stride::ast::types::is_integer(token_type_t type)
{
    switch ( type )
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

int stride::ast::types::is_valid_literal_value(token_type_t type)
{
    switch ( type )
    {
        case TOKEN_KEYWORD_NULL:
        case TOKEN_NUMBER_INTEGER:
        case TOKEN_NUMBER_FLOAT:
        case TOKEN_STRING_LITERAL:
        case TOKEN_BOOLEAN_LITERAL:
        case TOKEN_IDENTIFIER:
            return true;
        default:
            return false;
    }
}

int stride::ast::types::is_valid_variable_type(token_type_t type)
{
    switch ( type )
    {
        case TOKEN_IDENTIFIER:
        case TOKEN_PRIMITIVE_INT8:
        case TOKEN_PRIMITIVE_INT16:
        case TOKEN_PRIMITIVE_INT32:
        case TOKEN_PRIMITIVE_INT64:
        case TOKEN_PRIMITIVE_FLOAT32:
        case TOKEN_PRIMITIVE_FLOAT64:
        case TOKEN_PRIMITIVE_BOOL:
        case TOKEN_PRIMITIVE_STRING:
        case TOKEN_PRIMITIVE_CHAR:
        case TOKEN_PRIMITIVE_UNKNOWN:
            return true;
        default:
            return false;
    }
}
