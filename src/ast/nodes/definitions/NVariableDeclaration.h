#include "../../ASTNodes.h"

#include "../../../tokens/TokenSet.h"
#include "../../../tokens/token.h"

using namespace stride::ast;


/**
 * Variable declaration.
 * Variables are defined using the following format: <br />
 * <code>
 * let &lt;varName&gt;: &lt;varType&gt;; <br />
 * ... or <br />
 * const &lt;varName&gt;: &lt;varType&gt; = &lt;expression&gt;;
 * </code>
 */
class NVariableDeclaration : public Node
{
private:
    NIdentifier *varName;
    NIdentifier *varType;
    NExpression *value;
    bool isConst;
    bool isArray;

public:

    NVariableDeclaration() : isConst(false), isArray(false), varName(nullptr), varType(nullptr), value(nullptr)
    {}

    /**
     * Updates the name of the variable.
     * @param variable_name
     */
    void setVariableName(NIdentifier *variable_name)
    {
        this->varName = variable_name;
    }

    /**
     * Updates the name of the variable.
     * @param variable_name
     */
    void setVariableName(std::string variable_name)
    {
        this->varName = new NIdentifier(variable_name);
    }

    /**
     * Updates the value of the variable.
     * @param expression The expression to set.
     */
    void setValue(NExpression *expression)
    {
        this->value = expression;
    }

    /**
     * Updates the type node of this variable.
     * The type node of this variable ust be of type 'IDENTIFIER',
     * and can be validated after class instantiation.
     * @param variable_type The type node of the variable.
     */
    void setVariableType(NIdentifier *variable_type)
    {
        this->varType = variable_type;
    }

    /**
     * Updates whether this variable is an isVariableArray or not.
     * @param isVariableArray Whether it's an isVariableArray.
     */
    void setIsArray(bool isVariableArray)
    {
        this->isArray = isVariableArray;
    }

    /**
     * Changes whether this variable is mutable or not (constant)
     * @param isConst Whether the variable is mutable or not.
     */
    void setConst(bool isConstant)
    {
        this->isConst = isConstant;
    }

    enum ENodeType getType() override
    {
        return VARIABLE_DECLARATION;
    }

    static void parse(TokenSet &tokenSet, Node &parent);

    /**
     * Parses a singular variable declaration.
     * In Stride, multiple variables can be declared in a row with comma separation.
     * This function is to parse a single part of it. This function also allows assignment
     * and variadic parsing, if necessary. This will likely only be used in lambda and function declarations.
     * @param tokenSet The token set to parse the variable declaration with
     * @param allowAssignment Whether the variable declaration allows assignment.
     * @param implicitDeclaration Whether the variable is implicitly declared,
     * e.g. as a function parameter, or structure. Implicit declaration does not allow 'const' or 'let' as starting symbols
     * of the sequence. This will throw an error if found.
     * @param allowVariadic Whether or not the assignment allows variadic declaration.
     * @return An pointer to a variable declaration node.
     */
    static NVariableDeclaration *parseSingular(TokenSet &tokenSet, bool allowAssignment, bool implicitDeclaration, bool allowVariadic);

};