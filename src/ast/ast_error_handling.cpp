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

/**
 * Exits the program if the next token is not of the provided properties.
 * @param type The properties to check for.
 * @param offset The offset to check.p
 * @param errorMessage The error message to display.
 */
void stride::ast::requiresToken(token_type_t type, ast_token_set_t &token_set, cursor_t index, const char *error_message, ...)
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

        fprintf(stderr, "\n\nError at line %d column %d\n", ref.line, ref.column);
        vfprintf(stderr, error_message, args);
        va_end(args);
        exit(1);
    }
}