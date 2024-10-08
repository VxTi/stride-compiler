#ifndef STRIDE_LANGUAGE_NWHILELOOP_H
#define STRIDE_LANGUAGE_NWHILELOOP_H

#include "../../ASTNodes.h"
#include "../../../tokens/TokenSet.h"
#include "../../../tokens/token.h"
#include "NExpression.h"
#include "NBlock.h"

/**
     * Represents a while loop.
     * While loops are used to repeat a block of code while a conditionalValue is true.
     */
class NWhileLoop : public stride::ast::Node
{
public:
    NExpression *condition;
    NBlock *body;


    /**
     * Create a new while loop.
     * @param condition The conditionalValue of the while loop.
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

    enum stride::ast::ENodeType getType() override
    { return stride::ast::WHILE_LOOP; }

    static void parse(TokenSet &tokenSet, Node &parent);
};

#endif