//
// Created by Luca Warmenhoven on 16/08/2024.
//

#include <regex.h>
#include "lexer.h"

int get_column(const char *source, size_t index)
{
    if ( index == 0 )
    {
        return 0;
    }

    int column = 0, i;

    for ( i = 0; index - i >= 0; i++ )
    {
        if ( source[index - i] == '\n' )
        {
            break;
        }
        column++;
    }
    return column;
}

int get_line(const char *source, size_t index)
{
    int line = 1, i;
    for ( i = 0; i < index; i++ )
    {
        if ( source[i] == '\n' )
        {
            line++;
        }
    }
}

void lex_tokenize(const char *source, size_t source_size, token_t **dst, size_t *dst_size)
{
    int matched, i, j;

    size_t size = 0;

    auto *buffer = (token_t *) malloc(sizeof(token_t) * source_size);

    for ( i = 0; i < source_size; )
    {
        for ( j = 0, matched = 0; j < token_definitions.size(); j++ )
        {
            regmatch_t match;

            // Check if we have a match, and if the match is at the beginning of the string
            if ( !regexec(&token_definitions[j].regex, source + i, 1, &match, 0) && !match.rm_so )
            {
                // Append the token to the buffer
                token_t token;
                token.type = token_definitions[j].token;
                token.value = (char *) malloc(match.rm_eo - match.rm_so + 1);
                token.column = get_column(source, i);
                token.line = get_line(source, i);

                memcpy((void *) token.value, source + i + match.rm_so, match.rm_eo - match.rm_so);
                ((char *) token.value)[match.rm_eo - match.rm_so] = '\0';
                buffer[size++] = token;
                i += match.rm_eo - match.rm_so;
                matched = 1;
                break;
            }
        }
        if ( !matched )
        {
            i++;
        }
    }

    // Move the buffer to the destination
    *dst_size = size;
    *dst = buffer;
}