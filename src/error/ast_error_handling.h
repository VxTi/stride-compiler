//
// Created by Luca Warmenhoven on 29/08/2024.
//

#ifndef STRIDE_LANGUAGE_AST_ERROR_HANDLING_H
#define STRIDE_LANGUAGE_AST_ERROR_HANDLING_H

#include <vector>
#include "../StrideFile.h"

#define ERROR_LINE_BUFFER_SIZE 256

namespace stride::error
{

    void error(StrideFile &file, int index, const char *message, ...);

}

#endif //STRIDE_LANGUAGE_AST_ERROR_HANDLING_H
