//
// Created by Luca Warmenhoven on 27/08/2024.
//

#ifndef STRIDE_LANGUAGE_SYNTAX_VALIDATION_H
#define STRIDE_LANGUAGE_SYNTAX_VALIDATION_H

/**
 * This file contains the functions that are used to validate the syntax of the Stride language.
 * This is done by checking if the syntax is correct, and if not, throwing an error.
 */
#include "../../ast.h"

namespace stride::ast::validation {

    void validate(Node &root);

    /**
     * Checks whether the provided node is being accessed before declaration.
     */
    void check_variable_scope_order_accessibility(Node &node);

    /**
     * Checks whether the provided node is attempting to reassign an already
     * existing variable.
     * @param node
     */
    void check_variable_redeclaration(Node &original);

}

#endif //STRIDE_LANGUAGE_SYNTAX_VALIDATION_H
