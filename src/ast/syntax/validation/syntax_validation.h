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

    void validate(stride::ast::Node &root);

}

#endif //STRIDE_LANGUAGE_SYNTAX_VALIDATION_H
