//
// Created by Luca Warmenhoven on 10/09/2024.
//

#ifndef STRIDE_LANGUAGE_TOKENSET_H
#define STRIDE_LANGUAGE_TOKENSET_H

#include <vector>
#include "token.h"
#import "../StrideFile.h"

/**
 * Represents a token stream.
 * Token streams are used to store a sequence of tokens.
 */
class TokenSet
{
private:
    // Fields for the token stream.
    std::vector<token_t> *tokens;
    stride::StrideFile *source;
    int index;
    int startOffset;
    int length;

public:



    /**
     * Checks if the token stream has ended.
     * @return True if the token stream has ended, false otherwise.
     */
    [[nodiscard]] bool end() const;

    /**
     * Checks if the token stream has ended.
     * @param fromIndex The index to start from.
     * @return True if the token stream has ended, false otherwise.
     */
    [[nodiscard]] bool end(int fromIndex) const;

    /**
     * Gets the source file of the token stream.
     * @return The source file of the token stream.
     */
    [[nodiscard]] stride::StrideFile &getSource() const;

    /**
     * Gets the length of the token stream.
     * @return The length of the token stream.
     */
    [[nodiscard]] unsigned long int size() const;

    /**
     * Gets the index of the token stream.
     * @return The index of the token stream.
     */
    [[nodiscard]] int getIndex() const;

    void error(const char *message);

    /**
     * Checks if there are more tokens in the stream,
     * relative to the starting index.
     * @param fromIndex The index to start from.
     * @return True if there are more tokens, false otherwise.
     */
    bool hasNext(int fromIndex);

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
     * Gets the next token in the stream, relative to the starting index.
     * @param fromIndex The index to start from.
     * @return The next token in the stream.
     */
    token_t next(int *fromIndex);

    /**
     * Peeks at the next token in the stream.
     * @return The next token in the stream.
     */
    bool peekEq(token_type_t token, int offset);

    /**
     * Consumes a token from the stream.
     * @param token The token to consume.
     * @return True if the token was consumed, false otherwise.
     */
    bool consume(token_type_t token);

    /**
     * Consumes a token from the stream relative to the starting index.
     * After consuming the token, the index is updated.
     * @param token The token to consume.
     * @param fromIndex The index to start from.
     * @return True if the token was consumed, false otherwise.
     */
    bool consume(token_type_t token, int *fromIndex);

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
     *
     */
    bool canConsume(token_type_t token);

    /**
     * Checks if the next token in the stream is of a certain type.
     * @param token The token to check for.
     * @param fromIndex The index to start from.
     * @return True if the next token is of the specified type, false otherwise.
     */
    bool canConsume(token_type_t token, int fromIndex);

    /**
     * Returns the next token in the stream without consuming it.
     */
    token_t current();

    /**
     * Creates a new token stream.
     * @param tokens The tokens to store in the stream.
     */
    explicit TokenSet(std::vector<token_t> *tokens, stride::StrideFile *source);

    /**
     * Creates a new token stream from a subset of tokens.
     * This does not copy the content of the tokens vector,
     * but instead references the original vector and
     * updates the range of tokens to consider.
     */
    TokenSet *subset(int startOffset, int length);

    /*struct error_io
    {
        template<typename T>
        friend std::ostream &operator<<(std::ostream &os, T &message)
        {
            os << message;
            return os;
        }
    };*/

    token_t peek(int offset);
};

#endif //STRIDE_LANGUAGE_TOKENSET_H
