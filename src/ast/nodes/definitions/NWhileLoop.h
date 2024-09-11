#include "../../ASTNodes.h"

#include "../../../tokens/TokenSet.h"
#include "../../../tokens/token.h"

/**
     * Represents a while loop.
     * While loops are used to repeat a block of code while a condition is true.
     */
class NWhileLoop : public Node
{
public:
    NExpression *condition;
    NBlock *body;


    /**
     * Create a new while loop.
     * @param condition The condition of the while loop.
     * @param body The body of the while loop.
     */
    NWhileLoop(NExpression *condition, NBlock *body) :
            condition(condition),
            body(body)
    {}

    explicit NWhileLoop() : NWhileLoop(nullptr, nullptr)
    {};

    void setCondition(NExpression *condition)
    {
        this->condition = condition;
    }

    enum NType getType() override
    { return WHILE_LOOP; }

    static void parse(TokenSet &tokenSet, Node &parent);
};