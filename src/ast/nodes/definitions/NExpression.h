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

    static NExpression *parse(TokenSet &tokenSet);

    static void parse(TokenSet &tokenSet, Node &parent);

    enum stride::ast::ENodeType getType() override
    { return stride::ast::EXPRESSION; }
};