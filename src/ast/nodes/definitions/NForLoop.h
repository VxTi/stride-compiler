#ifndef STRIDE_LANGUAGE_NFORLOOP_H
#define STRIDE_LANGUAGE_NFORLOOP_H

#include "../../ASTNodes.h"
#include "../../../tokens/TokenSet.h"
#include "../../../tokens/token.h"
#include "NWhileLoop.h"
#include "NVariableDeclaration.h"
#include "NExpression.h"

/**
 * Represents a for loop.
 * For loops are used to repeat a block of code a fixed number of times.
 * The syntax for a for loop is as follows: <br />
 * <code>
 * for (initializer; conditionalValue; increment) { ... }
 * </code>
 */
class NForLoop : public NWhileLoop
{
private:
    std::vector<NVariableDeclaration *> initializers;
    std::vector<NVariableDeclaration *> increments;

public:
    /**
     * For loop constructor with initializer, conditionalValue and increment.
     * This is the classical form of a for-loop
     */
    NForLoop() : NWhileLoop() {}

    /**
     * Updates the conditionalValue of this for-loop.
     * @param condition The conditionalValue that has to hold true for the next iteration to happen.
     */
    void setCondition(NExpression *condition)
    {
        this->condition = condition;
    }

    /**
     * Adds an initializer to this for-loop node.
     * Initializers are variables that are declared when the for loop is initially called.
     * @param declaration The declaration of the initializer node.
     */
    void addInitializer(NVariableDeclaration *declaration)
    {
        initializers.push_back(declaration);
    }

    /**
     * Adds an incrementor node.
     * Incrementor abstractions are the ones that are called when the for loop was successfully executed,
     * and a next iteration is commencing.
     * @param incrementor The incrementor node.
     */
    void addIncrementor(NVariableDeclaration *incrementor)
    {
        increments.push_back(incrementor);
    }

    enum stride::ast::ENodeType getType() override
    { return stride::ast::FOR_LOOP; }

    static void parse(TokenSet &tokenSet, Node &parent);

};

#endif