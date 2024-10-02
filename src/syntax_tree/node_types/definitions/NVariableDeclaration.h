#ifndef STRIDE_LANGUAGE_NVARIABLEDECLARATION_H
#define STRIDE_LANGUAGE_NVARIABLEDECLARATION_H

#include <utility>

#include "../../ASTNodes.h"
#include "../../../tokens/TokenSet.h"
#include "../../../tokens/token.h"
#include "NExpression.h"
#include "NIdentifier.h"

#define VARIABLE_TYPE_PRIMITIVE 0
#define VARIABLE_TYPE_

/**
 * Variable declaration.
 * Variables are defined using the following format: <br />
 * <code>
 * let &lt;varName&gt;: &lt;varType&gt;; <br />
 * ... or <br />
 * const &lt;varName&gt;: &lt;varType&gt; = &lt;expression&gt;;
 * </code>
 */
class NVariableDeclaration : public stride::ast::Node
{
private:

    /**
     * A variant that can either be a string pointer or a token definition.
     * This makes it so that the type can be either a primitive type or a custom type.
     */
    std::variant<std::string *, token_type_t> varType;
    std::string *varName;

    NExpression *value;
    bool isPrimitiveType;
    bool isConst;
    bool isArray;

public:

    NVariableDeclaration() : isConst(false), isArray(false),
                             isPrimitiveType(false), varName(nullptr),
                             varType(nullptr), value(nullptr)
    {}

    /**
     * Updates the name of the variable.
     * @param variableName
     */
    void setVariableName(std::string variableName)
    {
        this->varName = new std::string(std::move(variableName));
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
    void setVariableType(std::variant<std::string *, token_type_t> type)
    {
        this->varType = type;
        this->isPrimitiveType = std::holds_alternative<std::string *>(type);
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

    enum stride::ast::ENodeType getType() override
    {
        return stride::ast::VARIABLE_DECLARATION;
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
    static NVariableDeclaration *
    parseSingular(TokenSet &tokenSet, bool allowAssignment, bool implicitDeclaration, bool allowVariadic);

};

#endif