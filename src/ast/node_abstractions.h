//
// Created by Luca Warmenhoven on 09/09/2024.
//

#ifndef STRIDE_LANGUAGE_NODE_ABSTRACTIONS_H
#define STRIDE_LANGUAGE_NODE_ABSTRACTIONS_H

#include <vector>

class Node
{
private:
    std::vector<Node *> children;

public:

    void addChild(Node *child)
    {
        children.push_back(child);
    }

    virtual ~Node() = default;
};

/**
 * Represents a literal.
 * Literals are used to represent fixed values.
 * For example, in the expression "1 + 2", the literals are "1" and "2", respectively.
 */
class NLiteral : public Node
{
public:
    std::string value;

    explicit NLiteral(std::string value) :
            value(std::move(value))
    {}
};

/**
 * Represents an identifier.
 * Identifiers are used to represent names.
 * For example, in the expression "let x = 1", the identifier is "x".
 */
class NIdentifier : public Node
{
public:
    std::string name;

    explicit NIdentifier(std::string name) :
            name(std::move(name))
    {}
};

/**
 * A statement is a list of expressions.
 */
class NStatement : public Node
{
};

/**
 * Represents an expression.
 * Expressions are used to evaluate values.
 */
class NExpression : public Node
{
public:
    explicit NExpression(std::string value)
    {
        addChild(new NLiteral(std::move(value)));
    }

    NExpression() = default;
};

enum EBinaryOperator
{
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    MODULO,
    EQUALS,
    NOT_EQUALS,
    LESS_THAN,
    GREATER_THAN,
    LESS_THAN_EQUALS,
    GREATER_THAN_EQUALS,
    AND,
    OR,
    XOR,
    SHIFT_LEFT,
    SHIFT_RIGHT,
    ASSIGN,
    ADD_ASSIGN,
    SUBTRACT_ASSIGN,
    MULTIPLY_ASSIGN,
    DIVIDE_ASSIGN,
    MODULO_ASSIGN,
    AND_ASSIGN,
    OR_ASSIGN,
    XOR_ASSIGN,
    SHIFT_LEFT_ASSIGN,
    SHIFT_RIGHT_ASSIGN
};

/**
 * Represents a binary operation.
 * Binary operations are used to evaluate two expressions.
 * For example, in the expression "1 + 2", the binary operation is the addition operation.
 */
class NBinaryOperator : public NExpression
{
public:
    enum EBinaryOperator operation;
    NExpression &left;
    NExpression &right;

    NBinaryOperator(NExpression &lhs, enum EBinaryOperator operation, NExpression &rhs) :
            left(lhs),
            right(rhs),
            operation(operation)
    {}
};

enum EUnaryOperator
{
    NEGATE,         // Logical NOT (!)
    BITWISE_NOT,    // Bitwise NOT (~)
    INCREMENT_LHS,  // Increment left-hand side (++symbol)
    INCREMENT_RHS,  // Increment right-hand side (symbol++)
    DECREMENT_LHS,  // Decrement left-hand side (--symbol)
    DECREMENT_RHS   // Decrement right-hand side (symbol--)
};

/**
 * Represents a unary operation.
 * Unary operations are used to evaluate a single expression.
 * For example, in the expression "!true", the unary operation is the logical NOT operation.
 */
class NUnaryOperator : public NExpression
{
public:
    enum EBinaryOperator operation;
    NExpression &expression;

    NUnaryOperator(enum EBinaryOperator operation, NExpression &expression) :
            operation(operation),
            expression(expression)
    {}
};

class NModuleDeclaration : public Node
{
public:
    std::string module_name;

    explicit NModuleDeclaration(std::string module_name) :
            module_name(std::move(module_name))
    {}
};

/**
 * Variable declaration.
 * Variables are defined using the following format: <br />
 * <code>
 * let &lt;variable_name&gt;: &lt;variable_type&gt;; <br />
 * ... or <br />
 * const &lt;variable_name&gt;: &lt;variable_type&gt; = &lt;expression&gt;;
 * </code>
 */
class NVariableDeclaration : public Node
{
public:
    NIdentifier *variable_name{};
    NIdentifier *variable_type{};
    bool immutable;
    bool is_array;

    NVariableDeclaration() : immutable(false), is_array(false)
    {}

    void setVariableName(NIdentifier *variable_name)
    {
        this->variable_name = variable_name;
    }

    void setVariableType(NIdentifier *variable_type)
    {
        this->variable_type = variable_type;
    }

    void setIsArray(bool is_array)
    {
        this->is_array = is_array;
    }

    void setImmutable(bool immutable)
    {
        this->immutable = immutable;
    }
};

/**
 * Try catch statement.
 * Try catch statements are used to catch exceptions.
 * They are defined using the following format: <br />
 * <code>
 * try { <br />
 *   &nbsp;&lt;statement&gt; ... <br />
 *  } catch (err: Type) { <br />
 *   &nbsp;&lt;statement&gt; ... <br />
 *  }
 */
class NTryCatchStatement : public Node
{
public:
    NStatement &try_block;
    NStatement &catch_block;
    NVariableDeclaration *exception;

    NTryCatchStatement(NStatement &try_block, NStatement &catch_block) :
            try_block(try_block),
            catch_block(catch_block),
            exception(nullptr)
    {}

    void setException(NVariableDeclaration *exception)
    {
        this->exception = exception;
    }
};

/**
 * Function declaration.
 * Functions are defined using the following format: <br />
 * <code>
 * def <function_name>(&lt;argument&gt;, ...) -&gt; &lt;return_type&gt; { <br />
 *    &nbsp;&lt;statement&gt; ... <br />
 * }
 * </code>
 */
class NFunctionDeclaration : public Node
{
public:
    NModuleDeclaration *module;
    std::string function_name;
    std::string return_type;
    std::vector<NVariableDeclaration *> arguments;

    NFunctionDeclaration(std::string function_name, std::string return_type) :
            function_name(std::move(function_name)),
            return_type(std::move(return_type)),
            module(nullptr)
    {}

    NFunctionDeclaration(std::string function_name, std::string return_type,
                         NModuleDeclaration *module) :
            function_name(std::move(function_name)),
            return_type(std::move(return_type)),
            module(module)
    {}

    void add_parameter(NVariableDeclaration *argument)
    {
        arguments.push_back(argument);
    }
};

/**
 * Function call.
 * Function calls are defined using the following format:
 * <code>
 * &lt;function_name&gt;(&lt;argument&gt;, ...)
 * </code>
 */
class NFunctionCall : public Node
{
public:
    std::string function_name;
    std::vector<NExpression *> arguments;

    NFunctionCall(std::string function_name) :
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
        this->addArgument(new NExpression(argument->value));
    }
};

class NOperatorOverload : public Node
{
public:
    enum EBinaryOperator operation;
    NFunctionDeclaration *overloaded_function;

    NOperatorOverload(enum EBinaryOperator operation, NFunctionDeclaration *overloaded_function) :
            operation(operation),
            overloaded_function(overloaded_function)
    {}

    void setOverloadedFunction(NFunctionDeclaration *overloaded_function)
    {
        if ( overloaded_function->arguments.size() != 2 )
        {
            throw std::runtime_error("Overloaded function must have 2 arguments, found " +
                                     std::to_string(overloaded_function->arguments.size()));
        }
        this->overloaded_function = overloaded_function;
    }
};

/**
 * Structure declaration.
 * Structures are defined using the following format: <br />
 * <code>
 * struct &lt;structure_name&gt; {
 *  &nbsp;&lt;field&gt; ...
 *  }
 */
class NStructureDeclaration : public Node
{
public:
    std::string structure_name;
    std::vector<NVariableDeclaration *> fields;

    NStructureDeclaration(std::string structure_name) :
            structure_name(std::move(structure_name))
    {}

    NStructureDeclaration(std::string structure_name, std::vector<NVariableDeclaration *> fields) :
            structure_name(std::move(structure_name)),
            fields(std::move(fields))
    {}

    void addField(NVariableDeclaration *field)
    {
        fields.push_back(field);
    }
};

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
public:
    NExpression &condition;
    NStatement if_true;
    NStatement *otherwise;

    /**
     * Create a new conditional statement.
     * @param condition The condition of the statement.
     * @param if_true The body of the statement.
     * @param otherwise The body of the else statement.
     */
    NConditionalStatement(NExpression &condition, NStatement &if_true, NStatement *otherwise) :
            condition(condition),
            if_true(if_true),
            otherwise(otherwise)
    {}
};

/**
 * Switch case.
 * Switch cases are defined using the following format: <br />
 * <code>
 * case &lt;literal&gt; -&gt; { ... }
 * </code>
 */
class NSwitchCase : public Node
{
public:
    NLiteral &condition;
    NStatement &body;

    /**
     * Create a new switch case.
     * @param condition The condition of the case.
     * @param body The body of the case.
     */
    NSwitchCase(NLiteral &condition, NStatement &body) :
            condition(condition),
            body(body)
    {}
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
class NSwitchStatement : public Node
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
};

/**
 * Class declaration.
 * Classes are defined using the following format:
 * <code>
 * class <class_name> {
 *   <field> ...
 *   <method> ...
 * }
 * </code>
 */
class NClassDeclaration : public Node
{
public:
    std::string class_name;
    std::vector<NClassDeclaration *> parents;
    std::vector<NVariableDeclaration *> fields;
    std::vector<NFunctionDeclaration *> methods;
    std::vector<NIdentifier *> generics;

    NClassDeclaration() = default;

    /**
     * Create a new class declaration
     * @param class_name The name of the class.
     */
    explicit NClassDeclaration(std::string class_name) :
            class_name(std::move(class_name))
    {}

    /**
     * Add a parent class to the class declaration.
     * @param parent The parent class.
     */
    void addParent(NClassDeclaration *parent)
    {
        parents.push_back(parent);
    }

    /**
     * Add a field to the class declaration
     * @param field The field to add.
     */
    void addField(NVariableDeclaration *field)
    {
        fields.push_back(field);
    }

    /**
     * Add a method to the class declaration.
     * @param method The method to add.
     */
    void addMethod(NFunctionDeclaration *method)
    {
        methods.push_back(method);
    }

    /**
     * Add a generic to the class declaration.
     * @param generic The generic to add.
     */
    void addGeneric(NIdentifier *generic)
    {
        generics.push_back(generic);
    }
};

/**
 * Import statement.
 * Import statements are used to import modules.
 * They are defined using the following format:
 * <code>
 * import &lt;module_name&gt;;
 * </code>
 */
class NImportStatement : public Node
{
public:
    std::string module_name;

    explicit NImportStatement(std::string module_name) :
            module_name(std::move(module_name))
    {}

    void validate();
};

#endif //STRIDE_LANGUAGE_NODE_ABSTRACTIONS_H
