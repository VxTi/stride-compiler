#ifndef STRIDE_LANGUAGE_NEXPRESSION_H
#define STRIDE_LANGUAGE_NEXPRESSION_H

#include "../../ASTNodes.h"
#include "../../../tokens/TokenSet.h"
#include "../../../tokens/token.h"
#include "NLiteral.h"
#include "../../Reducible.h"


/**
 * Represents an expression.
 * Expressions are used to evaluate values.
 */
class NExpression :
        public stride::ast::Node,
        public Reducible<NExpression *, NLiteral *>
{
public:

    typedef std::variant<NExpression *, NLiteral *> ExpressionReducible;

    NExpression() = default;

    virtual ~NExpression() = default;

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
     * @param explicitExpression Whether the expression is explicit or not. If set to true,
     * the expression must start with a keyword and have a left associative operator after it,
     * or be a function call.
     * @return The parsed expression.
     */
    static NExpression *parse(TokenSet &tokenSet, bool explicitExpression = false);

    static void parse(TokenSet &tokenSet, Node &parent);

    /**
     * Reduces the expression.
     * @return
     */
    ExpressionReducible reduce() override;

    /**
     * Whether the expression is reducible.
     * This is the case when the expression has reducible children,
     * or when the expression has a singular child that is reducible.
     */
    bool isReducible() override;

    enum stride::ast::ENodeType getType() override
    {
        return stride::ast::EXPRESSION;
    }
};

#endif