//
// Created by Luca Warmenhoven on 21/08/2024.
//


#include <iostream>
#include <iomanip>
#include "ast_error_handling.h"

#define ANSI_BOLD_WHITE "\033[1;38m"
#define ANSI_DARK_RED "\033[31m"
#define ANSI_RESET "\033[0m"
#define ANSI_BG_RED_WHITE "\033[101;38m"

void getFaultyLineInfo(const char *source, int index, int &line, int &column, std::string &lineContent)
{
    line = 1;
    column = 1;
    int start = index;
    int end = index;
    while ( start > 0 && source[ start ] != '\n' )
    {
        start--;
    }
    while ( end < strlen(source) && source[ end ] != '\n' )
    {
        end++;
    }
    if ( source[ start ] == '\n' )
    {
        start++;
    }
    lineContent = std::string(source + start, end - start);
    for ( int i = 0; i < index; i++ )
    {
        if ( source[ i ] == '\n' )
        {
            line++;
            column = 1;
        }
        else
        {
            column++;
        }
    }
}

/**
 * Exits the program with an error message.
 * This function uses variadic arguments,
 * so one can use string formatting and provide variables as
 * formatting arguments.
 * @param errorMessage The error message to display.
 */
void stride::error::error(StrideFile &file, int index, int tokenLength, const char *message, ...)
{
    va_list args;
    va_start(args, message);

    int line, column;
    std::string lineContent;
    getFaultyLineInfo(file.getContent().c_str(), index, line, column, lineContent);
    int lineNumberDigitsLength = std::to_string(line).length();
    int padding = 5 - lineNumberDigitsLength;
    // Insert ansi code at beginning (background red, foreground white) and reset at the end

    lineContent.insert(column - 1, ANSI_BG_RED_WHITE);
    lineContent.insert(column + tokenLength + strlen(ANSI_BG_RED_WHITE) - 1, ANSI_RESET);

    std::cout << std::endl << std::setw(padding) << "" << "╭─[ " << file.path() << ":" << line << ":" << column << " ] " << std::endl;
    std::cout << std::setw(padding) << "" << "·" << std::endl;
    std::cout << line << std::setw(padding - lineNumberDigitsLength) << "" << "│ " << lineContent << std::endl;
    std::cout << std::setw(padding) << "" << "·" << std::endl;
    std::cout << std::setw(padding) << "" << "·   " << ANSI_DARK_RED;
    vprintf(message, args);
    va_end(args);

    exit(1);
}