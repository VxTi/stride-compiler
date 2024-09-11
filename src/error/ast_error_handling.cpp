//
// Created by Luca Warmenhoven on 21/08/2024.
//


#include <iostream>
#include "ast_error_handling.h"

/**
 * Exits the program with an error message.
 * This function uses variadic arguments,
 * so one can use string formatting and provide variables as
 * formatting arguments.
 * @param errorMessage The error message to display.
 */
void stride::error::error(StrideFile &file, int index, const char *message, ...)
{
    va_list args;
    va_start(args, message);
    printf("\n%*s╭─[ \e[1;38m%s\e[0m ] \e[31mError at line %d column %d.\e[0m\n",
           0, "",
           file.path().c_str(), 0, 0);
    vprintf(message, args);
    va_end(args);
    exit(1);
}