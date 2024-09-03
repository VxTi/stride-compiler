//
// Created by Luca Warmenhoven on 29/08/2024.
//

#ifndef STRIDE_LANGUAGE_AST_ERROR_HANDLING_H
#define STRIDE_LANGUAGE_AST_ERROR_HANDLING_H


#include <queue>

#define ERROR_LINE_BUFFER_SIZE 256

namespace stride::error
{

    extern std::queue<const char *> error_queue;

    void begin(const char *file_path);

    void blame_line(const char *referring_source_line, unsigned int source_line_number, int start_index, int length, const char *message, ...);

    void empty_line();

    void end();

}

#endif //STRIDE_LANGUAGE_AST_ERROR_HANDLING_H
