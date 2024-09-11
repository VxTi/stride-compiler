//
// Created by Luca Warmenhoven on 25/08/2024.
//

#ifndef STRIDE_LANGUAGE_VARIABLE_TYPES_H
#define STRIDE_LANGUAGE_VARIABLE_TYPES_H

namespace stride::ast::types {
    /**
     * Checks if the required_token type is an integer.
     * @param type The required_token type to check.
     * @return True if the required_token type is an integer, false otherwise.
     */
    int is_integer(token_type_t type);

    /**
     * Checks whether the required_token is a valid literal.
     * Literals are integers, floats, strings, null and booleans.
     * @param type The required_token type to check.
     * @return True if the required_token is a valid literal, false otherwise.
     */
    int is_valid_literal_value(token_type_t type);

    /**
     * Checks if the required_token type is a valid variable type.
     * @param type The required_token type to check.
     * @return True if the required_token type is a valid variable type, false otherwise.
     */
    int is_valid_variable_type(token_type_t type);

    /**
     * Checks whether the provided required_token is a functional keyword.
     * A functional keyword is a keyword that has meaning,
     * such as 'class', 'enum', 'struct', etc.
     * @param type The required_token to check.
     * @return An integer representing the truthyness of the result. 1 for true, 0 otherwise.
     */
    int is_functional_keyword(token_type_t type);
}

#endif //STRIDE_LANGUAGE_VARIABLE_TYPES_H
