//
// Created by Luca Warmenhoven on 10/09/2024.
//

#include "TokenSet.h"

#include "../abstractions/AST.h"

bool TokenSet::canConsume(token_type_t type)
{
    if (!this->hasNext())
    {
        return false;
    }

    return this->tokens[index].type == type;
}

bool TokenSet::consume(token_type_t type)
{
    if ( this->canConsume(type) )
    {
        this->index++;
        return true;
    }
    return false;
}

token_t TokenSet::consumeRequired(token_type_t type, const char *message)
{
    if (!this->canConsume(type))
    {
        fprintf(stderr, "Error: %s\n", message);
        exit(1);
    }
    return this->next();
}

token_t TokenSet::current()
{
    return this->tokens[index];
}

bool TokenSet::peekEq(token_type_t type, unsigned int offset)
{
    if (index + offset >= this->tokens.size())
    {
        return false;
    }

    return this->tokens[index + offset].type == type;
}

bool TokenSet::hasNext()
{
    return index + 1 < this->tokens.size();
}

token_t TokenSet::next()
{
    return this->tokens[index++];
}

unsigned long int TokenSet::size()
{
    return this->length;
}

int TokenSet::getIndex()
{
    return this->index;
}

TokenSet *TokenSet::subset(int start, int end)
{
    auto *subset = new TokenSet(this->tokens);
    subset->index = start;
    subset->length = end - start;
    return subset;
}