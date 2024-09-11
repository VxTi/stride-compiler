#include "../../ASTNodes.h"

#include "../../../tokens/TokenSet.h"
#include "../../../tokens/token.h"
#include "NExpression.h"

enum EUnaryOperator
{
    NEGATE,         // Logical NOT (!)
    BITWISE_NOT,    // Bitwise NOT (~)
    INCREMENT_LHS,  // Increment left-hand side (++symbol)
    INCREMENT_RHS,  // Increment right-hand side (symbol++)
    DECREMENT_LHS,  // Decrement left-hand side (--symbol)
    DECREMENT_RHS   // Decrement right-hand side (symbol--)
};

/**
     * Represents a unary operation.
     * Unary operations are used to evaluate a single expression.
     * For example, in the expression "!true", the unary operation is the logical NOT operation.
     */
class NUnaryOperator : public NExpression
{
public:
    enum EUnaryOperator operation;
    NExpression &expression;

    NUnaryOperator(enum EUnaryOperator operation, NExpression &expression) :
            operation(operation),
            expression(expression)
    {}

    enum stride::ast::ENodeType getType() override
    { return stride::ast::UNARY_OPERATOR; }

    static void parse(TokenSet &tokenSet, Node &parent);
};