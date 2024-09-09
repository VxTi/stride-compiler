//
// Created by Luca Warmenhoven on 27/08/2024.
//

#ifndef STRIDE_LANGUAGE_SYNTAX_VALIDATION_H
#define STRIDE_LANGUAGE_SYNTAX_VALIDATION_H

/**
 * This file contains the functions that are used to validate the syntax of the Stride language.
 * This is done by checking if the syntax is correct, and if not, throwing an error.
 */
#include "../ast.h"

namespace stride::validation {

    void validate(stride::ast::Node &root);

    /**
     * Checks whether the symbol exists upon access.
     * This can include variable access before declaration,
     * duplicate declaration of variables / functions / modules,
     * and more.
     * @param root The root node to start the search from.
     */
    void check_symbol_existence_at_access(stride::ast::Node &root);

    void function_exists_before_call(stride::ast::Node &root);

}

#endif //STRIDE_LANGUAGE_SYNTAX_VALIDATION_H
