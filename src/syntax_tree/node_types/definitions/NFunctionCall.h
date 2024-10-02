#ifndef STRIDE_LANGUAGE_NFUNCTIONCALL_H
#define STRIDE_LANGUAGE_NFUNCTIONCALL_H

#include "../../ASTNodes.h"
#include "../../../tokens/TokenSet.h"
#include "../../../tokens/token.h"
#include "NExpression.h"
#include "NIdentifier.h"

/**
 * Function call.
 * Function calls are defined using the following format:
 * <code>
 * &lt;functionName&gt;(&lt;argument&gt;, ...)
 * </code>
 */
class NFunctionCall : public NExpression
{
public:

    std::vector<NExpression *> arguments;
    std::basic_string<char> *functionName;

    /**
     * Create a new function call with the given function name.
     * @param function_name The name of the function.
     */
    explicit NFunctionCall() : functionName(nullptr), arguments()
    {}

    /**
     * Add an argument to the function call.
     * @param argument The argument to add.
     */
    void addArgument(NExpression *argument)
    {
        arguments.push_back(argument);
    }

    enum stride::ast::ENodeType getType() override
    {
        return stride::ast::FUNCTION_CALL;
    }
};

#endif