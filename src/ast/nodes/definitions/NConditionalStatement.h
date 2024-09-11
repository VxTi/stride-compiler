#include "../../ASTNodes.h"

#include "../../../tokens/TokenSet.h"
#include "../../../tokens/token.h"

using namespace stride::ast;

/**
     * If statement.
     * If statements are defined using the following format:
     * <code>
     * if (&lt;expression&gt;) { <br />
     *   &lt;statement&gt; ... <br />
     * } elif { <br />
     *   &lt;statement&gt; ... <br />
     * } else { ... } <br />
     * </code>
     */
class NConditionalStatement : public Node
{
private:
    NExpression *condition;
    NBlock *truthyBlock;
    NBlock *otherwise;


public:
    explicit NConditionalStatement() : condition(nullptr), truthyBlock(nullptr), otherwise(nullptr)
    {}

    /**
     * Set the condition of the if statement.
     * @param condition The condition to set.
     */
    void setCondition(NExpression *condition)
    {
        NConditionalStatement::condition = condition;
    }

    /**
     * Set the truthy block of the if statement.
     * @param truthyBlock The truthy block to set.
     */
    void setTruthyBlock(NBlock *truthyBlock)
    {
        NConditionalStatement::truthyBlock = truthyBlock;
    }

    /**
     * Set the otherwise block of the if statement.
     * @param otherwise The otherwise block to set.
     */
    void setOtherwise(NBlock *otherwise)
    {
        NConditionalStatement::otherwise = otherwise;
    }

    enum ENodeType getType() override
    { return CONDITIONAL_STATEMENT; }
};
