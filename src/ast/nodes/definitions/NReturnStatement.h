#include "../../ASTNodes.h"

#include "../../../tokens/TokenSet.h"
#include "../../../tokens/token.h"
#include "NExpression.h"

using namespace stride::ast;

/**
     * Represents a return statement.
     * Return statements are used to return a value from a function.
     * For example, in the expression "return 1", the return statement is "return 1".
     */
class NReturnStatement : public Node
{
public:
    NExpression *expression;

    explicit NReturnStatement(NExpression *expression) :
            expression(expression)
    {}

    enum ENodeType getType() override
    { return RETURN_STATEMENT; }

    static void parse(TokenSet &tokenSet, Node &parent);
};
