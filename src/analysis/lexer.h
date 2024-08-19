//
// Created by Luca Warmenhoven on 16/08/2024.
//

#ifndef STRIDE_LANGUAGE_LEXER_H
#define STRIDE_LANGUAGE_LEXER_H

#include "token.h"

void lex_tokenize(const char *source, size_t source_size, token_t **dst, size_t *dst_size);
#endif //STRIDE_LANGUAGE_LEXER_H
