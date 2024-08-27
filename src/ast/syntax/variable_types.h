//
// Created by Luca Warmenhoven on 25/08/2024.
//

#ifndef STRIDE_LANGUAGE_VARIABLE_TYPES_H
#define STRIDE_LANGUAGE_VARIABLE_TYPES_H

#include "../ast.h"

namespace stride::ast::types {
    /**
     * Checks if the token type is an integer.
     * @param type The token type to check.
     * @return True if the token type is an integer, false otherwise.
     */
    int is_integer(token_type_t type);

    /**
     * Checks whether the token is a valid literal.
     * Literals are integers, floats, strings, null and booleans.
     * @param type The token type to check.
     * @return True if the token is a valid literal, false otherwise.
     */
    int is_valid_literal_value(token_type_t type);

    /**
     * Checks if the token type is a valid variable type.
     * @param type The token type to check.
     * @return True if the token type is a valid variable type, false otherwise.
     */
    int is_valid_variable_type(token_type_t type);
}

#endif //STRIDE_LANGUAGE_VARIABLE_TYPES_H
