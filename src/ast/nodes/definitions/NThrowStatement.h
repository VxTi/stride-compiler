#include "../../ASTNodes.h"

#include "../../../tokens/TokenSet.h"
#include "../../../tokens/token.h"
#include "NExpression.h"

using namespace stride::ast;

/**
     * Represents a throw statement.
     * Throw statements are used to throw an exception.
     * For example, in the expression "throw new Error()", the throw statement is "throw new Error()".
     */
class NThrowStatement : public Node
{
public:
    NExpression *expression;

    explicit NThrowStatement(NExpression *expression) :
            expression(expression)
    {}

    enum ENodeType getType() override
    { return THROW_STATEMENT; }

    static void parse(TokenSet &tokenSet, Node &parent);
};