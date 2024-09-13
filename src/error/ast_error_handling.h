//
// Created by Luca Warmenhoven on 29/08/2024.
//

#ifndef STRIDE_LANGUAGE_AST_ERROR_HANDLING_H
#define STRIDE_LANGUAGE_AST_ERROR_HANDLING_H

#include <vector>
#include "../StrideFile.h"

namespace stride::error
{
    void error(StrideFile &file, int index, int tokenLength, const char *message, ...);
}

#endif //STRIDE_LANGUAGE_AST_ERROR_HANDLING_H
