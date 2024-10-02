//
// Created by Luca Warmenhoven on 10/09/2024.
//

#include "TokenSet.h"
#include "../error/ast_error_handling.h"

TokenSet::TokenSet(std::vector<token_t> *tokens, stride::StrideFile *source)
{
    this->source = source;
    this->tokens = tokens;
    this->index = 0;
    this->startOffset = 0;

    if ( this->tokens == nullptr )
    {
        this->error("Token stream is null.");
        exit(1);
    }

    this->length = tokens->size();

    if ( this->length == 0 )
    {
        this->error("Token stream is empty.");
    }
}

bool TokenSet::end(int fromIndex) const
{
    return this->startOffset + fromIndex >= this->tokens->size()
              || fromIndex >= this->length;
}

bool TokenSet::end() const
{
    return this->end(this->index);
}

bool TokenSet::canConsume(token_type_t token, int fromIndex)
{
    return !end(fromIndex) && ( *this->tokens )[ this->startOffset + fromIndex ].type == token;
}

bool TokenSet::canConsume(token_type_t type)
{
    return this->canConsume(type, this->index);
}

bool TokenSet::consume(token_type_t token, int *fromIndex)
{
    if ( !this->canConsume(token, *fromIndex))
    {
        return false;
    }

    ( *fromIndex )++;
    return true;
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
    return ( *this->tokens )[ this->startOffset + this->index ];
}

bool TokenSet::peekEq(token_type_t type, int offset)
{
    int absoluteIndex = this->startOffset + this->index + offset;

    // If the relative index or absolute index exceeds the boundaries, stop.
    if ( this->index + offset >= this->length || absoluteIndex >= this->tokens->size())
    {
        return false;
    }

    return ( *this->tokens )[ absoluteIndex ].type == type;
}

bool TokenSet::hasNext()
{
    return this->hasNext(this->index);
}

bool TokenSet::hasNext(int fromIndex)
{
    return this->startOffset + fromIndex + 1 < this->tokens->size() &&
           fromIndex + 1 < this->length && this->tokens != nullptr;
}

token_t TokenSet::next(int *fromIndex)
{
    if (end(*fromIndex))
    {
        this->error("No more tokens in stream.");
    }

    token_t entry = ( *this->tokens )[ this->startOffset + *fromIndex ];
    ( *fromIndex )++;
    return entry;
}

token_t TokenSet::next()
{
    return this->next(&this->index);
}

// relative size, not absolute.
unsigned long int TokenSet::size() const
{
    return this->length;
}

// Relative index, not absolute.
int TokenSet::getIndex() const
{
    return this->index;
}

TokenSet *TokenSet::subset(int start, int subLength)
{
    auto *subset = new TokenSet(this->tokens, this->source);
    subset->startOffset = this->startOffset + start;
    subset->length = subLength;
    return subset;
}

stride::StrideFile &TokenSet::getSource() const
{
    return *this->source;
}

void TokenSet::error(const char *message)
{
    stride::error::error(*this->source, this->current().index, strlen(this->current().value), message);
}

token_t TokenSet::peek(int offset)
{
    return ( *this->tokens )[ this->startOffset + this->index + offset ];
}