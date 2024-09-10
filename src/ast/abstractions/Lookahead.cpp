//
// Created by Luca Warmenhoven on 24/08/2024.
//

#include "Lookahead.h"

using namespace stride::ast;

TokenSet *captureBlock(TokenSet &tokens, token_type_t start_token, token_type_t end_token)
{
    tokens.consumeRequired(start_token,
                           "Expected opening required_token after statement. This could be due to missing tokens.");

    int indexFrom = 0, indexTo = 0,
            branchDepth = 0;

    for ( ; tokens.hasNext(); )
    {
        if ( tokens.consume(start_token) && branchDepth++ == 0 )
        {
            indexFrom = tokens.getIndex();
        }
        else if ( tokens.consume(end_token) && --branchDepth == 0 )
        {
            indexTo = tokens.getIndex();
            break;
        }

        if ( branchDepth < 0 )
        {
            fprintf(stderr, "Imbalanced closure at line %d column %d",
                  tokens.current().line, tokens.current().column);
        }
    }

    // If there aren't any tokens skipped, there's no closure to append.
    if ( indexFrom > 0 && indexTo > 0 )
    {
        return tokens.subset(indexFrom, indexTo);
    }

    return nullptr;
}

int stride::ast::distanceNextToken(TokenSet &stream, token_type_t token)
{
    int startingIndex = stream.getIndex();
    for (; stream.hasNext(); stream.next())
    {
        if (stream.current().type == token)
        {
            return stream.getIndex() - startingIndex;
        }
    }
    return -1;
}

int stride::ast::distanceNextTokenExclude(TokenSet &stream, token_type_t token)
{
    int startingIndex = stream.getIndex();
    int braces = 0, brackets = 0, squareBrackets = 0;

    for (; stream.hasNext(); stream.next())
    {
        if ( stream.consume(token) && braces == 0 && brackets == 0 && squareBrackets == 0 )
        {
            return stream.getIndex() - startingIndex;
        }
        if ( stream.consume(TOKEN_LBRACE) )
        {

        }
    }
    return -1;
}


int stride::ast::distance_next_token_outside_block(ast_token_set_t &token_set, int starting_index, token_type_t token)
{
    int
            distance = -1,
            brace_depth = 0,
            bracket_depth = 0,
            square_bracket_depth = 0,
            index = starting_index;

    for ( ;
            index < token_set.token_count;
            index++
            )
    {
        switch ( token_set.tokens[ index ].type )
        {
            case TOKEN_LBRACE:
                brace_depth++;
                break;
            case TOKEN_RBRACE:
                brace_depth--;
                break;
            case TOKEN_LPAREN:
                bracket_depth++;
                break;
            case TOKEN_RPAREN:
                bracket_depth--;
                break;
            case TOKEN_LSQUARE_BRACKET:
                square_bracket_depth++;
                break;
            case TOKEN_RSQUARE_BRACKET:
                square_bracket_depth--;
                break;
            default:
                break;
        }
        if ( token_set.tokens[ index ].type == token &&
             brace_depth == 0 &&
             bracket_depth == 0 &&
             square_bracket_depth == 0 )
        {
            distance = index - starting_index;
            break;
        }
        if ( brace_depth < 0 || bracket_depth < 0 || square_bracket_depth < 0 )
        {
            break;
        }
    }

    return distance;
}