//
// Created by Luca Warmenhoven on 09/09/2024.
//

#ifndef STRIDE_LANGUAGE_ASTNODES_H
#define STRIDE_LANGUAGE_ASTNODES_H

#include <vector>
#include "../tokens/TokenSet.h"
#include "../tokens/token.h"

namespace stride::ast
{

    enum NType
    {
        GENERIC,
        LITERAL,
        IDENTIFIER,
        BLOCK,
        EXPRESSION,
        ARRAY,
        BINARY_OPERATOR,
        UNARY_OPERATOR,
        TRY_CATCH_CLAUSE,
        OPERATOR_OVERLOAD,
        FUNCTION_CALL,

        VARIABLE_DECLARATION,
        FUNCTION_DECLARATION,
        MODULE_DECLARATION,     // module <name> { ... }
        STRUCTURE_DECLARATION,  // struct <name> { ... }
        ENUMERABLE_DECLARATION, // enum <name> { ... }
        CLASS_DECLARATION,      // class <name><<generics>> like ..., ..., { ... }

        THROW_STATEMENT,
        RETURN_STATEMENT,
        CONDITIONAL_STATEMENT,
        SWITCH_STATEMENT,       // switch (... )
        SWITCH_CASE,            // case ... -> { ... }
        IMPORT_STATEMENT,       // import ...
        FOR_LOOP, WHILE_LOOP, DO_WHILE_LOOP
    };

    class IRValue
    {
    public:
        void *value;

        explicit IRValue(void *value) : value(value)
        {}

    };

    /**
     * Represents a node in the abstract syntax tree.
     * Nodes are used to represent the structure of the source code.
     */
    class Node
    {
    private:
        std::vector<Node *> children;

    public:

        /**
         * Returns the type of the node.
         * This is useful for future casting.
         * @return The type of this node.
         */
        virtual enum NType getType()
        { return GENERIC; }

        /**
         * Generates the IR code for this node.
         * @return The IR code for this node.
         */
        virtual IRValue *codegen()
        { return nullptr; }

        /**
         * Parses the token stream and appends the children to the parent node.
         * @param tokens The token stream to parse.
         * @param parent The parent node to append the children to.
         */
        static void parse(TokenSet &tokenSet, Node &parent)
        {}

        /**
         * Converts the node to a string.
         * @return The string representation of the node.
         */
        virtual std::string toString()
        { return ""; }

        /**
         * Appends a child to this node tree.
         * @param child
         */
        void addChild(Node *child)
        {
            children.push_back(child);
        }

        /**
         * Returns the children of this node.
         * @return The children of this node.
         */
        std::vector<Node *> getChildren()
        {
            return children;
        }

        /**
         * Destructor for the node.s
         */
        ~Node()
        {
            for ( auto &child: children )
            {
                delete child;
            }
        }
    };

    /*
     * Represents a literal.
     * Literals are used to represent fixed values.
     * For example, in the expression "1 + 2", the literals are "1" and "2", respectively.
     */
    class NLiteral : public Node
    {
    public:

        void *value;

        /**
         * Create a new literal node.
         * @param val The current of the literal.
         */
        explicit NLiteral(void *val) : value(val)
        {}

        enum NType getType() override
        { return LITERAL; }

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

        enum NType getType() override
        { return IDENTIFIER; }
    };

    /**
     * Represents a block.
     * A block is a sequence of statements and expressions.
     */
    class NBlock : public Node
    {
        enum NType getType() override
        { return BLOCK; }
    };

    /**
     * Represents an expression.
     * Expressions are used to evaluate values.
     */
    class NExpression : public Node
    {
    public:
        explicit NExpression(const std::string &value)
        {
            addChild(new NLiteral((void *) value.c_str()));
        }

        NExpression() = default;

        static NExpression *parse(TokenSet &tokenSet);

        static void parse(TokenSet &tokenSet, Node &parent);

        enum NType getType() override
        { return EXPRESSION; }
    };

    /**
     * Represents a return statement.
     * Return statements are used to return a value from a function.
     * For example, in the expression "return 1", the return statement is "return 1".
     */
    class NReturnStatement : public Node
    {
    public:
        NExpression *expression;

        explicit NReturnStatement(NExpression *expression) :
                expression(expression)
        {}

        enum NType getType() override
        { return RETURN_STATEMENT; }

        static void parse(TokenSet &tokenSet, Node &parent);
    };

    /**
     * Represents a throw statement.
     * Throw statements are used to throw an exception.
     * For example, in the expression "throw new Error()", the throw statement is "throw new Error()".
     */
    class NThrowStatement : public Node
    {
    public:
        NExpression *expression;

        explicit NThrowStatement(NExpression *expression) :
                expression(expression)
        {}

        enum NType getType() override
        { return THROW_STATEMENT; }

        static void parse(TokenSet &tokenSet, Node &parent);
    };

    /**
     * Represents a block.
     * Blocks are used to group statements.
     * For example, in the expression "{ let x = 1; let y = 2; }", the block contains the statements "let x = 1" and "let y = 2".
     */
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
    class NBinaryOperation : public NExpression
    {
    public:
        enum EBinaryOperator operation;
        NExpression &left;
        NExpression &right;

        NBinaryOperation(NExpression &lhs, enum EBinaryOperator operation, NExpression &rhs) :
                left(lhs),
                right(rhs),
                operation(operation)
        {}

        enum NType getType() override
        { return BINARY_OPERATOR; }

        static void parse(TokenSet &tokenSet, Node &parent);
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
        enum EUnaryOperator operation;
        NExpression &expression;

        NUnaryOperator(enum EUnaryOperator operation, NExpression &expression) :
                operation(operation),
                expression(expression)
        {}

        enum NType getType() override
        { return UNARY_OPERATOR; }

        static void parse(TokenSet &tokenSet, Node &parent);
    };

    class NModuleDeclaration : public Node
    {
    public:
        std::string module_name;

        explicit NModuleDeclaration(std::string module_name) :
                module_name(std::move(module_name))
        {}

        enum NType getType() override
        { return MODULE_DECLARATION; }

        static void parse(TokenSet &tokenSet, Node &parent);

    };

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

        enum NType getType() override
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
    private:
        NBlock *tryBlock;
        NBlock *catchBlock;
        NVariableDeclaration *exception;

    public:
        NTryCatchStatement(NBlock *try_block, NBlock *catch_block) :
                tryBlock(try_block),
                catchBlock(catch_block),
                exception(nullptr)
        {}

        /**
         * Set the exception variable.
         * @param exception The exception variable.
         */
        void setException(NVariableDeclaration *exception)
        {
            this->exception = exception;
        }

        /**
         * Set the try block.
         * This is the block that will be executed first,
         * and will be the block that could throw an exception.
         * @param try_block The try block.
         */
        void setTryBlock(NBlock *try_block)
        {
            this->tryBlock = try_block;
        }

        /**
         * Set the catch block.
         * @param catch_block The catch block.
         */
        void setCatchBlock(NBlock *catch_block)
        {
            this->catchBlock = catch_block;
        }

        enum NType getType() override
        { return TRY_CATCH_CLAUSE; }

        static void parse(TokenSet &tokenSet, Node &parent);

    };

    /**
     * Function declaration.
     * Functions are defined using the following format: <br />
     * <code>
     * def <functionName>(&lt;argument&gt;, ...) -&gt; &lt;returnType&gt; { <br />
     *    &nbsp;&lt;statement&gt; ... <br />
     * }
     * </code>
     */
    class NFunctionDeclaration : public Node
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

        enum NType getType() override
        { return FUNCTION_DECLARATION; }

        static void parse(TokenSet &tokenSetet, Node &parent);
    };

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

        enum NType getType() override
        { return FUNCTION_CALL; }

        static void parse(TokenSet &tokenSet, Node &parent);

    };

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

        enum NType getType() override
        { return OPERATOR_OVERLOAD; }

        static void parse(TokenSet &tokenSet, Node &parent);

    };

    class NEnumerableDeclaration : public Node
    {
    public:

        std::map<std::string, long int> values;

        NEnumerableDeclaration() : values()
        {};

        void addValue(const char *key, long int value)
        {
            values [ key ] = value;
        }

        void addValue(std::string &key, long int value)
        {
            values[ key ] = value;
        }

        /**
         * Parses an enumerable
         * @param tokenSet
         * @param parent
         */
        static void parse(TokenSet &tokenSet, Node &parent);

        enum NType getType() override
        { return ENUMERABLE_DECLARATION; }
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
    private:

        std::string structure_name;
        std::vector<NVariableDeclaration *> *fields;

    public:

        explicit NStructureDeclaration() : structure_name(""), fields(new std::vector<NVariableDeclaration *>())
        {}

        /**
         * Add a field to the structure declaration.
         * @param field The field to add.
         */
        void addField(NVariableDeclaration *field)
        {
            fields->push_back(field);
        }

        void setName(const char *name) {
            structure_name = name;
        }


        enum NType getType() override
        {
            return STRUCTURE_DECLARATION;
        }

        static void parse(TokenSet &tokenSet, Node &parent);

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
    private:
        NExpression *condition;
        NBlock *truthyBlock;
        NBlock *otherwise;


    public:
        explicit NConditionalStatement() : condition(nullptr), truthyBlock(nullptr), otherwise(nullptr)
        {}

        /**
         * Set the condition of the if statement.
         * @param condition The condition to set.
         */
        void setCondition(NExpression *condition)
        {
            NConditionalStatement::condition = condition;
        }

        /**
         * Set the truthy block of the if statement.
         * @param truthyBlock The truthy block to set.
         */
        void setTruthyBlock(NBlock *truthyBlock)
        {
            NConditionalStatement::truthyBlock = truthyBlock;
        }

        /**
         * Set the otherwise block of the if statement.
         * @param otherwise The otherwise block to set.
         */
        void setOtherwise(NBlock *otherwise)
        {
            NConditionalStatement::otherwise = otherwise;
        }

        enum NType getType() override
        { return CONDITIONAL_STATEMENT; }
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

        enum NType getType() override
        { return SWITCH_CASE; }

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

        enum NType getType() override
        { return SWITCH_STATEMENT; }

        static void parse(TokenSet &tokenSet, Node &parent);

    };

    /**
     * Class declaration.
     * Classes are defined using the following format: <br />
     * <code>
     * class &lt;class_name&gt; {
     *  ... <br />
     * } <br />
     * or <br />
     * class &lt;class_name&gt;&lt;&lt;GENERICS&gt;&gt; : &lt;parent_class&gt;, ... {
     * </code>
     */
    class NClassDeclaration : public Node
    {
    public:
        std::string class_name;
        std::vector<NIdentifier *> *parents;
        std::vector<NIdentifier *> *generics;
        NBlock *body;

        NClassDeclaration();

        ~NClassDeclaration();

        /**
         * Add a parent class to the class declaration.
         * @param parent The parent class.
         */
        void addParent(NIdentifier *parent)
        {
            parents->push_back(parent);
        }

        /**
         * Add a generic to the class declaration.
         * @param generic The generic to add.
         */
        void addClassGeneric(NIdentifier *generic)
        {
            generics->push_back(generic);
        }

        enum NType getType() override
        { return CLASS_DECLARATION; }

        static void parse(TokenSet &tokenSet, Node &parent);
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

        enum NType getType() override
        { return IMPORT_STATEMENT; }

        static void parse(TokenSet &tokenSet, Node &parent);
    };

    /**
     * Represents a do-while loop.
     * Do-while loops are used to repeat a block of code while a condition is true.
     * The condition is checked after the loop body is executed.
     * This means that the loop body is always executed at least once.
     * The syntax for a do-while loop is as follows:
     * <code>
     * do {  ... } while (condition);
     *   </code>
     */
    class NDoWhileLoop : public NWhileLoop
    {
    public:
        NDoWhileLoop(NExpression *condition, NBlock *body) :
                NWhileLoop(condition, body)
        {}

        explicit NDoWhileLoop() : NDoWhileLoop(nullptr, nullptr)
        {}

        enum NType getType() override
        { return DO_WHILE_LOOP; }

        static void parse(TokenSet &tokenSet, Node &parent);

    };

    /**
     * Represents a for loop.
     * For loops are used to repeat a block of code a fixed number of times.
     * The syntax for a for loop is as follows: <br />
     * <code>
     * for (initializer; condition; increment) { ... }
     * </code>
     */
    class NForLoop : public NWhileLoop
    {
    public:
        std::vector<NVariableDeclaration *> initializers;
        std::vector<NVariableDeclaration *> increments;

        /**
         * For loop constructor with initializer, condition and increment.
         * This is the classical form of a for-loop
         * @param initializer The initializer node, e.g. with a variable declaration.
         * @param condition The condition that has to hold true for the next iteration to commence
         * @param increment The incremental expression that
         * @param body
         */
        NForLoop(NExpression *condition, NExpression *increment, NBlock *body) :
                NWhileLoop(condition, body)
        {}

        /**
         * Updates the condition of this for-loop.
         * @param condition The condition that has to hold true for the next iteration to happen.
         */
        void setCondition(NExpression *condition)
        {
            this->condition = condition;
        }

        /**
         * Adds an initializer to this for-loop node.
         * Initializers are variables that are declared when the for loop is initially called.
         * @param declaration The declaration of the initializer node.
         */
        void addInitializer(NVariableDeclaration *declaration)
        {
            initializers.push_back(declaration);
        }

        /**
         * Adds an incrementor node.
         * Incrementor abstractions are the ones that are called when the for loop was successfully executed,
         * and a next iteration is commencing.
         * @param incrementor The incrementor node.
         */
        void addIncrementor(NVariableDeclaration *incrementor)
        {
            increments.push_back(incrementor);
        }

        enum NType getType() override
        { return FOR_LOOP; }

        static void parse(TokenSet &tokenSet, Node &parent);

    };

    namespace parser
    {

        /**
         * Parses a token set and appends the generated nodes
         * to a root node. This root node will then be returned.
         * @param tokenSet The token set to parse
         */
        Node *parse(TokenSet &tokenSetet);

        /**
         * Parses a token set and appends the generated nodes
         * to the provided root node.
         * @param tokenSet The token set to parse
         * @param root The root node to append the generated nodes to.
         */
        void parse(TokenSet &tokenSetet, Node &root);
    }
}

#endif //STRIDE_LANGUAGE_ASTNODES_H
