#ifndef STRIDE_LANGUAGE_NUNARYOPERATOR_H
#define STRIDE_LANGUAGE_NUNARYOPERATOR_H

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
    std::unique_ptr<NExpression> expression;

    NUnaryOperator(enum EUnaryOperator operation, std::unique_ptr<NExpression> expression) :
            operation(operation),
            expression(std::move(expression))
    {}

    enum stride::ast::ENodeType getType() override
    { return stride::ast::UNARY_OPERATOR; }
};

#endif