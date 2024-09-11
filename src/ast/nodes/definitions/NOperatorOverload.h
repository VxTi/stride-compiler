#include "../../ASTNodes.h"

#include "../../../tokens/TokenSet.h"
#include "../../../tokens/token.h"
#include "NBinaryOperation.h"
#include "NFunctionDeclaration.h"

class NOperatorOverload : public Node
{
public:
    enum EBinaryOperator operation;
    NFunctionDeclaration *overloaded_function;

    /**
     * Create a new operator overload.
     * @param operation The operation to overload.
     * @param overloaded_function The overloaded function.
     */
    NOperatorOverload(enum EBinaryOperator operation, NFunctionDeclaration *overloaded_function) :
            operation(operation),
            overloaded_function(overloaded_function)
    {}

    /**
     * Set the overloaded function.
     * @param overloaded_function The overloaded function.
     */
    void setOverloadedFunction(NFunctionDeclaration *overloaded_function)
    {
        if ( overloaded_function->arguments.size() != 2 )
        {
            throw std::runtime_error("Overloaded function must have 2 arguments, found " +
                                     std::to_string(overloaded_function->arguments.size()));
        }
        this->overloaded_function = overloaded_function;
    }

    enum ENodeType getType() override
    { return OPERATOR_OVERLOAD; }

    static void parse(TokenSet &tokenSet, Node &parent);

};