#ifndef STRIDE_LANGUAGE_NTHROWSTATEMENT_H
#define STRIDE_LANGUAGE_NTHROWSTATEMENT_H

#include "../../ASTNodes.h"

#include "../../../tokens/TokenSet.h"
#include "../../../tokens/token.h"
#include "NExpression.h"

/**
     * Represents a throw statement.
     * Throw statements are used to throw an exception.
     * For example, in the expression "throw new Error()", the throw statement is "throw new Error()".
     */
class NThrowStatement : public stride::ast::Node
{
public:
    NExpression *expression;

    explicit NThrowStatement(NExpression *expression) :
            expression(expression)
    {}

    enum stride::ast::ENodeType getType() override
    { return stride::ast::THROW_STATEMENT; }

    static void parse(TokenSet &tokenSet, Node &parent);
};

#endif