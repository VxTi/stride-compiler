#ifndef STRIDE_LANGUAGE_NSWITCHSTATEMENT_H
#define STRIDE_LANGUAGE_NSWITCHSTATEMENT_H

#include "../../ASTNodes.h"
#include "../../../tokens/TokenSet.h"
#include "../../../tokens/token.h"
#include "NLiteral.h"
#include "NBlock.h"
#include "NExpression.h"

/**
    * Switch case.
    * Switch cases are defined using the following format: <br />
    * <code>
    * case &lt;literal&gt; -&gt; { ... }
    * </code>
    */
class NSwitchCase : public stride::ast::Node
{
public:
    NLiteral *conditionalValue;
    NBlock *body;

    /**
     * Create a new switch case.
     * @param condition The conditionalValue of the case.
     * @param body The body of the case.
     */
    NSwitchCase() : conditionalValue(nullptr), body(nullptr){}

    enum stride::ast::ENodeType getType() override
    {
        return stride::ast::SWITCH_CASE;
    }

    static void parse(TokenSet &tokenSet, Node &parent);
};

/**
 * Switch statement.
 * Switch statements are defined using the following format:
 * <code>
 * switch (&lt;expression&gt;) { <br />
 *   &lt;case&gt; ... <br />
 * }
 * </code>
 */
class NSwitchStatement : public stride::ast::Node
{
private:

    NExpression *expression;
    std::vector<NSwitchCase *> cases;
    NSwitchCase *defaultCase;

public:

    NSwitchStatement() : cases(), defaultCase(nullptr), expression(nullptr)
    {}

    /**
     * Add a case to the switch statement.
     * @param switchCase The case to add.
     */
    void addCase(NSwitchCase *switchCase)
    {
        cases.push_back(switchCase);
    }

    /**
     * Set the default case.
     * @param switchCase The default case.
     */
    void setDefaultCase(NSwitchCase *switchCase)
    {
        defaultCase = switchCase;
    }

    enum stride::ast::ENodeType getType() override
    {
        return stride::ast::SWITCH_STATEMENT;
    }

    static void parse(TokenSet &tokenSet, Node &parent);

};

#endif