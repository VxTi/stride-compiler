#ifndef STRIDE_LANGUAGE_NCONDITIONALSTATEMENT_H
#define STRIDE_LANGUAGE_NCONDITIONALSTATEMENT_H

#include "../../ASTNodes.h"

#include "../../../tokens/TokenSet.h"
#include "../../../tokens/token.h"
#include "NBlock.h"
#include "NExpression.h"

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
class NConditionalStatement : public stride::ast::Node
{
private:
    NExpression *condition;
    NBlock *truthyBlock;
    NBlock *otherwise;


public:
    explicit NConditionalStatement() : condition(nullptr), truthyBlock(nullptr), otherwise(nullptr)
    {}

    /**
     * Set the conditionalValue of the if statement.
     * @param condition The conditionalValue to set.
     */
    void setCondition(NExpression *condition)
    {
        NConditionalStatement::condition = condition;
    }

    /**
     * Set the truthy block of the if statement.
     * @param truthyBlock The truthy block to set.
     */
    void setThen(NBlock *truthyBlock)
    {
        NConditionalStatement::truthyBlock = truthyBlock;
    }

    /**
     * Set the otherwise block of the if statement.
     * @param otherwise The otherwise block to set.
     */
    void setElse(NBlock *otherwise)
    {
        NConditionalStatement::otherwise = otherwise;
    }

    static void parse(TokenSet &tokenSet, Node &parent);/**/

    enum stride::ast::ENodeType getType() override
    { return stride::ast::CONDITIONAL_STATEMENT; }
};

#endif