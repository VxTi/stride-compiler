//
// Created by Luca Warmenhoven on 16/08/2024.
//

#ifndef STRIDE_LANGUAGE_LEXER_H
#define STRIDE_LANGUAGE_LEXER_H

#include "token.h"
#include "../ast/ast.h"

using namespace stride::ast;

namespace stride::lexer {
    void tokenize(const char *source, size_t source_size, ast_token_set_t &token_set);
}
#endif //STRIDE_LANGUAGE_LEXER_H
