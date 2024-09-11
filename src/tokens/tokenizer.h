//
// Created by Luca Warmenhoven on 16/08/2024.
//

#ifndef STRIDE_LANGUAGE_TOKENIZER_H
#define STRIDE_LANGUAGE_TOKENIZER_H

#include "token.h"
#include "../StrideFile.h"
#include "../tokens/TokenSet.h"

namespace stride {
    TokenSet * tokenize(stride::StrideFile *source);
}
#endif //STRIDE_LANGUAGE_TOKENIZER_H
