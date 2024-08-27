//
// Created by Luca Warmenhoven on 21/08/2024.
//


#include <iostream>
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

std::string capture_line(token_t token, int index)
{
    std::string line;

    const char *file_content = stride::ast::current_file_content.c_str();
    for ( int i = index; i >= 0; i-- )
    {
        if ( file_content[ i ] == '\n' || i == 0 )
        {
            line = std::string(file_content + i + 1, file_content + index);
            break;
        }
    }
            line += std::string("\e[0;33m") + std::string(token.value) + std::string("\e[0m");
    for (int i = index + strlen(token.value); i < stride::ast::current_file_content.size(); i++)
    {
        if ( file_content[ i ] == '\n' || i == stride::ast::current_file_content.size() - 1 )
        {
            line += std::string(file_content + index + strlen(token.value), file_content + i);
            break;
        }
    }


    return line;
}

void stride::ast::blame_token(token_t token, const char *error_message, ...)
{
    std::string line = capture_line(token, token.index);
    std::string line_number = std::to_string(token.line);
    va_list args;
    va_start(args, error_message);
    int half_token_len = strlen(token.value) / 2;

    // Print line_number's width in space  characters
    printf("\n%*s╭─[ \e[1;38m%s\e[0m ] \e[31mError at line %d column %d.\e[0m\n %d │ %s\n",
           (int) line_number.length() + 2, "", stride::ast::current_file_name.c_str(),
           token.line, token.column,
           token.line, line.c_str());
    printf("%*s·%*s\e[31m┬ \e[0;33m│\n",
           (int) line_number.length() + 2, " ", token.column - 1 + half_token_len, " ");
    printf("%*s·%*s\e[31m│ \e[0;33m╰─ Faulty token: %s\e[0m\n",
           (int) line_number.length() + 2, " ", token.column - 1 + half_token_len, " ",
           token.value);

    printf("%*s·%*s\e[31m╰── ", (int) line_number.length() + 2, " ", token.column - 1 + half_token_len, " ");
    vprintf(error_message, args);
    printf("\e[0m\n");
    for (int i = 0; i < line_number.length() + 1; i++) printf("─");
    printf("─╯\n");
    va_end(args);
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

        blame_token(ref, error_message, args);

        va_end(args);
        exit(1);
    }
}