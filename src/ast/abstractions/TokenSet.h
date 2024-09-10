//
// Created by Luca Warmenhoven on 10/09/2024.
//

#ifndef STRIDE_LANGUAGE_TOKENSET_H
#define STRIDE_LANGUAGE_TOKENSET_H

#include <vector>
#include "../../tokens/token.h"

/**
 * Represents a token stream.
 * Token streams are used to store a sequence of tokens.
 */
class TokenSet
{
private:
    // Fields for the token stream.
    std::vector<token_t> &tokens;
    int index;
    unsigned long int length;

public:

    /**
     * Gets the length of the token stream.
     * @return The length of the token stream.
     */
    unsigned long int size();

    /**
     * Gets the index of the token stream.
     * @return The index of the token stream.
     */
    int getIndex();


    /**
     * Checks if there are more tokens in the stream.
     * @return True if there are more tokens, false otherwise.
     */
    bool hasNext();

    /**
     * Gets the next token in the stream.
     * @return The next token in the stream.
     */
    token_t next();

    /**
     * Peeks at the next token in the stream.
     * @return The next token in the stream.
     */
    bool peekEq(token_type_t token, unsigned int offset);

    /**
     * Consumes a token from the stream.
     * @param token The token to consume.
     * @return True if the token was consumed, false otherwise.
     */
    bool consume(token_type_t token);

    /**
     * Consumes a token from the stream.
     * If the token is not found, an error message is printed.
     * @param type The token to consume.
     */
    token_t consumeRequired(token_type_t type, const char *message);

    /**
     * Checks if the next token in the stream is of a certain type.
     * @param token The token to check for.
     * @return True if the next token is of the specified type, false otherwise.
     */
    bool canConsume(token_type_t token);

    /**
     * Returns the next token in the stream without consuming it.
     */
    token_t current();

    /**
     * Creates a new token stream.
     * @param tokens The tokens to store in the stream.
     */
    explicit TokenSet(std::vector<token_t> &tokens) :
            tokens(tokens),
            index(0),
            length(tokens.size())
    {}

    explicit TokenSet(std::vector<token_t> &tokens, int startOffset) :
            tokens(tokens),
            index(startOffset),
            length(tokens.size() - startOffset)
    {}

    /**
     * Creates a new token stream from a subset of tokens.
     * This does not copy the content of the tokens vector,
     * but instead references the original vector and
     * updates the range of tokens to consider.
     */
    TokenSet *subset(int startOffset, int endOffset);

};

#endif //STRIDE_LANGUAGE_TOKENSET_H
