#include "../../ASTNodes.h"

#include "../../../tokens/TokenSet.h"
#include "../../../tokens/token.h"

using namespace stride::ast;


/**
 * Represents an expression.
 * Expressions are used to evaluate values.
 */
class NExpression : public Node
{
public:
    explicit NExpression(const std::string &value)
    {
        addChild(new NLiteral((void *) value.c_str()));
    }

    NExpression() = default;

    static NExpression *parse(TokenSet &tokenSet);

    static void parse(TokenSet &tokenSet, Node &parent);

    enum ENodeType getType() override
    { return EXPRESSION; }
};