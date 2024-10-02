#ifndef STRIDE_LANGUAGE_NFUNCTIONDECLARATION_H
#define STRIDE_LANGUAGE_NFUNCTIONDECLARATION_H

#include "../../ASTNodes.h"

#include "../../../tokens/TokenSet.h"
#include "../../../tokens/token.h"

#include "NIdentifier.h"
#include "NBlock.h"
#include "NVariableDeclaration.h"

/**
 * Function declaration.
 * Functions are defined using the following format: <br />
 * <code>
 * def <functionName>(&lt;argument&gt;, ...) -&gt; &lt;returnType&gt; { <br />
 *    &nbsp;&lt;statement&gt; ... <br />
 * }
 * </code>
 */
class NFunctionDeclaration : public stride::ast::Node
{
public:
    NIdentifier *functionName;
    NIdentifier *returnType;
    NBlock *body;
    std::vector<NVariableDeclaration *> arguments;
    bool isPublic;
    bool external;
    bool async;

    /**
     * Add a parameter to the function declaration.
     * @param argument The argument to add.
     */
    void addParameter(NVariableDeclaration *argument)
    {
        arguments.push_back(argument);
    }

    void setReturnType(NIdentifier &return_type)
    {
        this->returnType = &return_type;
    }

    void setFunctionName(std::string function_name)
    {
        this->functionName = new NIdentifier(function_name);
    }

    enum stride::ast::ENodeType getType() override
    { return stride::ast::FUNCTION_DECLARATION; }

    static void parse(TokenSet &tokenSetet, Node &parent);
};

#endif