//
// Created by Luca Warmenhoven on 21/08/2024.
//


#include "ast.h"

/**
 * Exits the program with an error message.
 * This function uses variadic arguments,
 * so one can use string formatting and provide variables as
 * formatting arguments.
 * @param errorMessage The error message to display.
 */
void stride::ast::error(const char *errorMessage, ...)
{
    va_list args;
    va_start(args, errorMessage);
    vfprintf(stderr, errorMessage, args);
    va_end(args);
    exit(1);
}

std::string capture_line(stride::ast::ast_token_set_t &token_set, int index) {
    std::string line;
    // Find first token in line, up until provided token index
    int line_number = token_set.tokens[index].line;

    for (int i = index; i >= 0; i--) {
        if (token_set.tokens[i].line != line_number) {
            break;
        }
        line = std::string(token_set.tokens[i].value) + " " + line;
    }
    for (int i = index + 1; i < token_set.token_count; i++) {
        if (token_set.tokens[i].line != line_number) {
            break;
        }
        line = line + " " + token_set.tokens[i].value;
    }
    return line;
}

/**
 * Exits the program if the next token is not of the provided properties.
 * @param type The properties to check for.
 * @param offset The offset to check.p
 * @param errorMessage The error message to display.
 */
void
stride::ast::requires_token(token_type_t type, ast_token_set_t &token_set, cursor_t index, const char *error_message,
                           ...)
{
    if ( index >= token_set.token_count || token_set.tokens[ index ].type != type )
    {

        va_list args;
        va_start(args, error_message);

        // Prevent empty token sets from causing a crash
        if ( token_set.token_count == 0 )
        {
            fprintf(stderr, "\n\nError whilst seeking token - No tokens found.");
            exit(1);
        }

        token_t ref = token_set.tokens[
                ( index >= token_set.token_count ? token_set.token_count - 1 : index == 0 ? 0 : index - 1 )
        ];
        std::string line = capture_line(token_set, index);
        fprintf(stderr, "\nError at line %d column %d\n", ref.line, ref.column);
        fprintf(stderr, " %d | %s\n", ref.line, line.c_str());
        fprintf(stderr, " %*s^\n", ref.column, "");
        vfprintf(stderr, error_message, args);
        va_end(args);
        exit(1);
    }
}