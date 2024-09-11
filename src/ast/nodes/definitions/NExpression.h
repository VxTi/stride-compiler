#ifndef STRIDE_LANGUAGE_NEXPRESSION_H
#define STRIDE_LANGUAGE_NEXPRESSION_H

#include "../../ASTNodes.h"
#include "../../../tokens/TokenSet.h"
#include "../../../tokens/token.h"
#include "NLiteral.h"

/**
 * Represents an expression.
 * Expressions are used to evaluate values.
 */
class NExpression : public stride::ast::Node
{
public:
    explicit NExpression(const std::string &value)
    {
        addChild(new NLiteral((void *) value.c_str()));
    }

    NExpression() = default;

    /**
     * Parses an expression that is surrounded by parenthesis.
     * This is useful for capturing expressions after, for example, an if statement,
     * or while statement.
     * @param tokenSet The set of tokens to parse.
     * @return The parsed expression.
     */
    static NExpression *captureParenthesis(TokenSet &tokenSet);

    /**
     * Parses an expression.
     * This function automatically stops parsing when it encounters a semicolon,
     * and does not consume it. This allows the parser to check for the end of the expression.
     * @param tokenSet The set of tokens to parse.
     * @return The parsed expression.
     */
    static NExpression *parse(TokenSet &tokenSet);

    static void parse(TokenSet &tokenSet, Node &parent);

    enum stride::ast::ENodeType getType() override
    { return stride::ast::EXPRESSION; }
};

#endif