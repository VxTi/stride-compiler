#ifndef STRIDE_LANGUAGE_NRETURNSTATEMENT_H
#define STRIDE_LANGUAGE_NRETURNSTATEMENT_H

#include "../../ASTNodes.h"

#include "../../../tokens/TokenSet.h"
#include "../../../tokens/token.h"
#include "NExpression.h"

/**
     * Represents a return statement.
     * Return statements are used to return a value from a function.
     * For example, in the expression "return 1", the return statement is "return 1".
     */
class NReturnStatement : public stride::ast::Node
{
public:
    NExpression *expression;

    explicit NReturnStatement(NExpression *expression) :
            expression(expression)
    {}

    enum stride::ast::ENodeType getType() override
    { return stride::ast::RETURN_STATEMENT; }

    static void parse(TokenSet &tokenSet, Node &parent);
};

#endif