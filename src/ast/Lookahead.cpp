//
// Created by Luca Warmenhoven on 24/08/2024.
//

#include "Lookahead.h"

using namespace stride::ast;

TokenSet *stride::ast::captureBlock(TokenSet &tokens, token_type_t start_token, token_type_t end_token)
{
    tokens.consumeRequired(start_token,
                           "Expected opening required_token after statement. This could be due to missing tokens.");

    int
            indexFrom = tokens.getIndex(),
            indexTo = tokens.getIndex(),
            branchDepth = 0;

    for ( ; tokens.hasNext(indexTo); tokens.next(&indexTo))
    {
        if ( tokens.consume(start_token, &indexTo) && branchDepth++ == 0 )
        {
            indexFrom = tokens.getIndex();
        }
        else if ( tokens.consume(end_token, &indexTo) && --branchDepth == 0 )
        {
            break;
        }

        if ( branchDepth < 0 )
        {
            tokens.error("Imbalanced closure");
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
    for ( ; stream.hasNext(); stream.next())
    {
        if ( stream.current().type == token )
        {
            return stream.getIndex() - startingIndex;
        }
    }
    return -1;
}

int stride::ast::distanceNextTokenInScope(TokenSet &stream, token_type_t token)
{
    int startingIndex = stream.getIndex();
    int braces = 0, brackets = 0, squareBrackets = 0;

    for ( ; stream.hasNext(); stream.next())
    {
        if ( stream.consume(token) && braces == 0 && brackets == 0 && squareBrackets == 0 )
        {
            return stream.getIndex() - startingIndex;
        }
        switch ( stream.current().type )
        {
            case TOKEN_LBRACE:
                braces++;
                break;
            case TOKEN_RBRACE:
                braces--;
                break;
            case TOKEN_LPAREN:
                brackets++;
                break;
            case TOKEN_RPAREN:
                brackets--;
                break;
            case TOKEN_LSQUARE_BRACKET:
                squareBrackets++;
                break;
            case TOKEN_RSQUARE_BRACKET:
                squareBrackets--;
                break;
            default:
                break;
        }
        if ( braces < 0 || brackets < 0 || squareBrackets < 0 )
        {
            break;
        }
    }
    return -1;
}