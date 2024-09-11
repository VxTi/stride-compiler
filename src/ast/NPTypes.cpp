//
// Created by Luca Warmenhoven on 11/09/2024.
//

#include "NodeProperties.h"


bool stride::ast::validateVariableType(TokenSet &tokenSet)
{
    token_type_t type = tokenSet.current().type;
    switch (type)
    {
        case TOKEN_IDENTIFIER:
        case TOKEN_PRIMITIVE_INT8:
        case TOKEN_PRIMITIVE_INT16:
        case TOKEN_PRIMITIVE_INT32:
        case TOKEN_PRIMITIVE_INT64:
        case TOKEN_PRIMITIVE_FLOAT32:
        case TOKEN_PRIMITIVE_FLOAT64:
        case TOKEN_PRIMITIVE_UNKNOWN:
        case TOKEN_PRIMITIVE_VOID:
        case TOKEN_PRIMITIVE_BOOL:
        case TOKEN_PRIMITIVE_CHAR:
            return true;
        default:
            return false;
    }
}