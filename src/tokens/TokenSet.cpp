//
// Created by Luca Warmenhoven on 10/09/2024.
//

#include "TokenSet.h"
#include "../error/ast_error_handling.h"

bool TokenSet::canConsume(token_type_t token, int fromIndex)
{
    if ( fromIndex >= this->length || this->startOffset + fromIndex >= this->tokens.size())
    {
        return false;
    }

    return this->tokens[ this->startOffset + fromIndex ].type == token;
}

bool TokenSet::canConsume(token_type_t type)
{
    return this->canConsume(type, this->index);
}

bool TokenSet::consume(token_type_t token, int *fromIndex)
{
    if ( this->canConsume(token, *fromIndex))
    {
        ( *fromIndex )++;
        return true;
    }
    return false;
}

bool TokenSet::consume(token_type_t type)
{
    return this->consume(type, &this->index);
}

token_t TokenSet::consumeRequired(token_type_t type, const char *message)
{
    if ( !this->canConsume(type))
    {
        this->error(message);
    }
    return this->next();
}

token_t TokenSet::current()
{
    return this->tokens[ this->startOffset + this->index ];
}

bool TokenSet::peekEq(token_type_t type, int offset)
{
    int absoluteIndex = this->startOffset + this->index + offset;
    // If the relative index or absolute index exceeds the boundaries, stop.
    if ( this->index + offset >= this->length || absoluteIndex >= this->tokens.size())
    {
        return false;
    }

    return this->tokens[ absoluteIndex ].type == type;
}

bool TokenSet::hasNext()
{
    return this->hasNext(this->index);
}

bool TokenSet::hasNext(int fromIndex)
{
    return this->startOffset + fromIndex + 1 < this->tokens.size() &&
           this->startOffset + fromIndex + 1 < this->length;
}

token_t TokenSet::next(int *fromIndex)
{
    return this->tokens[ *fromIndex++ ];
}

token_t TokenSet::next()
{
    if ( !this->hasNext())
    {
        this->error("Unexpected end of file.");
    }
    return this->tokens[ index++ ];
}

unsigned long int TokenSet::size() const
{
    return this->length;
}

int TokenSet::getIndex() const
{
    return this->index;
}

TokenSet *TokenSet::subset(int start, int subLength)
{
    auto *subset = new TokenSet(this->tokens, this->source);
    subset->startOffset = start;
    subset->length = subLength;
    return subset;
}

stride::StrideFile &TokenSet::getSource() const
{
    return this->source;
}

void TokenSet::error(const char *message)
{
    stride::error::error(this->source, this->index, message);
}
