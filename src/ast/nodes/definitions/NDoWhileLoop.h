#include "../../ASTNodes.h"

#include "../../../tokens/TokenSet.h"
#include "../../../tokens/token.h"

#include "NWhileLoop.h"

using namespace stride::ast;

/**
     * Represents a do-while loop.
     * Do-while loops are used to repeat a block of code while a condition is true.
     * The condition is checked after the loop body is executed.
     * This means that the loop body is always executed at least once.
     * The syntax for a do-while loop is as follows:
     * <code>
     * do {  ... } while (condition);
     *   </code>
     */
class NDoWhileLoop : public NWhileLoop
{
public:
    NDoWhileLoop(NExpression *condition, NBlock *body) :
            NWhileLoop(condition, body)
    {}

    explicit NDoWhileLoop() : NDoWhileLoop(nullptr, nullptr)
    {}

    enum ENodeType getType() override
    { return DO_WHILE_LOOP; }

    static void parse(TokenSet &tokenSet, Node &parent);

};