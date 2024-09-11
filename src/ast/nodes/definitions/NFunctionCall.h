#include "../../ASTNodes.h"

#include "../../../tokens/TokenSet.h"
#include "../../../tokens/token.h"
#include "NExpression.h"
#include "NLiteral.h"

using namespace stride::ast;

/**
 * Function call.
 * Function calls are defined using the following format:
 * <code>
 * &lt;functionName&gt;(&lt;argument&gt;, ...)
 * </code>
 */
class NFunctionCall : public Node
{
public:
    std::string function_name;
    std::vector<NExpression *> arguments;

    /**
     * Create a new function call with the given function name.
     * @param function_name The name of the function.
     */
    explicit NFunctionCall(std::string function_name) :
            function_name(std::move(function_name))
    {}

    /**
     * Add an argument to the function call.
     * @param argument The argument to add.
     */
    void addArgument(NExpression *argument)
    {
        arguments.push_back(argument);
    }

    /**
     * Add a literal argument to the function call.
     * @param argument The argument to add.
     */
    void addArgument(NLiteral *argument)
    {
        this->addArgument(new NExpression((char *) argument->value));
    }

    enum ENodeType getType() override
    { return FUNCTION_CALL; }

    static void parse(TokenSet &tokenSet, Node &parent);

};