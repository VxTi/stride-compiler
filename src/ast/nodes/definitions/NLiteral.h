#ifndef STRIDE_LANGUAGE_NLITERAL_H
#define STRIDE_LANGUAGE_NLITERAL_H

#include <utility>

#include "../../ASTNodes.h"

#include "../../../tokens/TokenSet.h"
#include "../../../tokens/token.h"

#define INT8_MASK 0x7F
#define INT16_MASK 0x7FFF
#define INT32_MASK 0x7FFFFFFF

typedef std::variant<
        int64_t, // 64 bit int,
        double_t, // 64 bit float,
        const char *
> LiteralValue;

/**
 * Represents a literal.
 * Literals are used to represent fixed values.
 * For example, in the expression "1 + 2", the literals are "1" and "2", respectively.
 */
class NLiteral : public stride::ast::Node
{
private:
    int byteCount;

public:

    LiteralValue value;

    /**
     * Create a new literal node.
     * Literals created with this constructor are used to represent fixed values.
     * @param token The token representing the literal.
     */
    explicit NLiteral(token_t token);

    explicit NLiteral(int64_t value) : value(value), byteCount(8)
    {}

    explicit NLiteral(float value) : value(value), byteCount(4)
    {}

    explicit NLiteral(double_t value) : value(value), byteCount(8)
    {}

    explicit NLiteral(int8_t value) : value(value), byteCount(1)
    {}

    explicit NLiteral(int16_t value) : value(value), byteCount(2)
    {}

    explicit NLiteral(int32_t value) : value(value), byteCount(4)
    {}


    explicit NLiteral(const char *value) : value(value)
    {
        if ( value[ 0 ] == 0 )
        {
            this->value = (int8_t) 0;
            this->byteCount = 1;
        }
        byteCount = strlen(value);
    }

    enum stride::ast::ENodeType getType() override
    { return stride::ast::LITERAL; }

    /**
     * Returns the size of the literal in bytes.
     * This is used for the generation of the assembly code.
     */
    int bytes();

    /**
     * Whether the literal is a numeric value.
     * This can be both integers and floating point numbers.
     */
    bool isNumeric();

    /**
     * Whether the literal is an integer.
     */
    bool isInteger();
};

#endif