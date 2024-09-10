//
// Created by Luca Warmenhoven on 10/09/2024.
//

#include "TokenPatterns.h"

using namespace stride::ast::pattern;

int Pattern::patternLength()
{
    int length = 0;
    for ( PatternFragment *fragment: fragments )
    {
        length += fragment->patternLength;
    }
    return length;
}

bool PRefPattern::accepts(TokenSet &tokens)
{
    return pattern.accepts(tokens);
}

bool PExplicit::accepts(TokenSet &stream)
{
    if ( stream.consume(matcher))
    {
        if ( match_dst != nullptr )
        {
            *(void **) match_dst = stream.next().value;
        }
        return true;
    }
    return false;
}

bool POptional::accepts(TokenSet &tokens)
{
    if ( tokens.canConsume(matcher) && match_dst != nullptr )
    {
        *(void **) match_dst = tokens.next().value;
    }
    return true;
}

bool PVarying::accepts(TokenSet &tokens)
{
    for ( auto &matcher: matchers )
    {
        if ( tokens.consume(matcher))
        {
            return true;
        }
    }
    return false;
}