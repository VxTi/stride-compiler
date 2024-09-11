#ifndef STRIDE_LANGUAGE_NSWITCHSTATEMENT_H
#define STRIDE_LANGUAGE_NSWITCHSTATEMENT_H

#include "../../ASTNodes.h"
#include "../../../tokens/TokenSet.h"
#include "../../../tokens/token.h"
#include "NLiteral.h"
#include "NBlock.h"

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
    NLiteral &condition;
    NBlock &body;

    /**
     * Create a new switch case.
     * @param condition The condition of the case.
     * @param body The body of the case.
     */
    NSwitchCase(NLiteral &condition, NBlock &body) :
            condition(condition),
            body(body)
    {}

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
public:
    std::vector<NSwitchCase *> cases;
    NSwitchCase *default_case;

    /**
     * Create a new switch statement.
     * @param cases The cases of the switch statement.
     * @param default_case The default case.
     */
    NSwitchStatement(std::vector<NSwitchCase *> cases, NSwitchCase *default_case) :
            cases(std::move(cases)),
            default_case(default_case)
    {}

    /**
     * Add a case to the switch statement.
     * @param switch_case The case to add.
     */
    void addCase(NSwitchCase *switch_case)
    {
        cases.push_back(switch_case);
    }

    /**
     * Set the default case.
     * @param switch_case The default case.
     */
    void setDefaultCase(NSwitchCase *switch_case)
    {
        default_case = switch_case;
    }

    enum stride::ast::ENodeType getType() override
    {
        return stride::ast::SWITCH_STATEMENT;
    }

    static void parse(TokenSet &tokenSet, Node &parent);

};

#endif