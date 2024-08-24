//
// Created by Luca Warmenhoven on 16/08/2024.
//

#include <regex.h>
#include "lexer.h"

/**
 * Checks whether the provided character is a word boundary character,
 * meaning, if there's a word surrounded by other letters, the function will return false.
 * The only moments the function returns true is for whitespaces.
 * @param character
 * @return
 */
bool is_word_boundary(char character)
{
    const char *word_boundary_characters = " \n\t(){}[]<>=+-*/%&|^~!?;:,.";
    for ( int i = 0; i < strlen(word_boundary_characters); i++ )
    {
        if ( character == word_boundary_characters[ i ] )
        {
            return true;
        }
    }
    return false;
}

/* Returns the cursor position relative to the cursor index */
void get_cursor_position(const char *source, size_t index, int *line, int *column)
{
    int i;
    for ( i = 0, *line = 1, *column = 0; i < index; i++ )
    {
        if ( source[ i ] == '\n' )
        {
            ( *line )++;
            *column = 0;
        }
        else
        {
            ( *column )++;
        }
    }
}

/**
 * Tokenize the source code.
 * This function will tokenize the source code and store the tokens in the destination token set.
 * Tokenization is based on the grammar defined in token.h. This uses regular expressions
 * to match for phrases in source code, and generate tokens accordingly.
 * @param source The source code.
 * @param source_size The size of the source code.
 * @param dst The destination token set.
 */
void stride::lexer::tokenize(const char *source, size_t source_size, ast_token_set_t &dst)
{
    int matched, i, j, line, col;

    dst.token_count = 0;
    dst.tokens = (token_t *) malloc(sizeof(token_t) * source_size);

    for ( i = 0; i < source_size; )
    {
        // Skip whitespaces
        if ( source[ i ] == ' ' || source[ i ] == '\n' || source[ i ] == '\t' )
        {
            i++;
            continue;
        }
        // Prevent illegal characters
        if ( source[ i ] < 32 || source[ i ] > 126 )
        {
            get_cursor_position(source, i, &line, &col);
            fprintf(stderr, "Invalid character at line %d, column %d\n", line, col);
            exit(1);
        }

        for ( j = 0, matched = 0; j < token_definitions.size(); j++ )
        {
            regmatch_t match;

            // Check if we have a match, and if the match is at the beginning of the string
            if ( !regexec(&token_definitions[ j ].regex, source + i, 1, &match, 0) && !match.rm_so )
            {
                if ((
                            !is_word_boundary(source[ i + match.rm_eo ]) ||
                            (i + match.rm_so - 1 >= 0 && !is_word_boundary(source[ i + match.rm_so - 1 ]))
                    ) && token_definitions[ j ].keyword )
                {
                    continue;
                }
                // Append the token to the buffer
                token_t token;
                token.type = token_definitions[ j ].token;
                token.value = (char *) malloc(match.rm_eo - match.rm_so + 1);
                get_cursor_position(source, i, &line, &col);
                token.line = line;
                token.column = col;

                memcpy((void *) token.value, source + i + match.rm_so, match.rm_eo - match.rm_so);
                ((char *) token.value )[ match.rm_eo - match.rm_so ] = '\0';
                dst.tokens[ dst.token_count++ ] = token;
                i += match.rm_eo - match.rm_so;
                matched = 1;
                break;
            }
        }
        if ( !matched )
        {
            get_cursor_position(source, i, &line, &col);
            fprintf(stderr, "Illegal character found at line %d column %d: %s\n", line, col, source + i);
            exit(1);
        }
    }
}