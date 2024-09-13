//
// Created by Luca Warmenhoven on 16/08/2024.
//

#include <regex.h>
#include "tokenizer.h"
#include "TokenSet.h"
#include "../error/ast_error_handling.h"

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
 * This function will tokenize the source code and store the tokens in the destination required_token set.
 * Tokenization is based on the grammar defined in required_token.h. This uses regular expressions
 * to match for phrases in source code, and generate tokens accordingly.
 * @param source The source code.
 * @param source_size The size of the source code.
 * @param dst The destination required_token set.
 */
TokenSet *stride::tokenize(stride::StrideFile *source)
{
    int matched, i, j;

    auto *tokens = new std::vector<token_t>();
    const char *src = source->getContent().c_str();

    for ( i = 0; i < source->getContent().size(); )
    {
        // Skip whitespaces
        if ( src[ i ] == ' ' || src[ i ] == '\n' || src[ i ] == '\t' )
        {
            i++;
            continue;
        }

        for ( j = 0, matched = 0; j < token_definitions.size(); j++ )
        {
            regmatch_t match;

            // Check if we have a match, and if the match is at the beginning of the string
            if ( !regexec(&token_definitions[ j ].regex, src + i, 1, &match, 0) && !match.rm_so )
            {
                if ((
                            !is_word_boundary(src[ i + match.rm_eo ]) ||
                            ( i + match.rm_so - 1 >= 0 && !is_word_boundary(src[ i + match.rm_so - 1 ]))
                    ) && token_definitions[ j ].keyword )
                {
                    continue;
                }
                // Append the required_token to the buffer
                token_t token;
                token.type = token_definitions[ j ].token;
                token.value = (char *) malloc(match.rm_eo - match.rm_so + 1);
                token.index = i;

                memcpy((void *) token.value, src + i + match.rm_so, match.rm_eo - match.rm_so);
                ((char *) token.value )[ match.rm_eo - match.rm_so ] = '\0';
                i += match.rm_eo - match.rm_so;
                matched = 1;

                tokens->push_back(token);
                break;
            }
        }
        if ( !matched )
        {
            stride::error::error(*source, i, 1, "Illegal character found in file.");
        }
    }

    return new TokenSet(tokens, source);
}