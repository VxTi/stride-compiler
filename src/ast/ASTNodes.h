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

    enum ENodeType
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
        virtual enum ENodeType getType()
        { return GENERIC; }

        /**
         * Generates the IR code for this node.
         * @return The IR code for this node.
         */
        virtual IRValue *codegen()
        { return nullptr; }

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
            if ( child != nullptr )
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

    namespace parser
    {

        /**
         * Parses a token set and appends the generated nodes
         * to a root node. This root node will then be returned.
         * @param tokenSet The token set to parse
         */
        Node *parse(TokenSet &tokenSet);

        /**
         * Parses a token set and appends the generated nodes
         * to the provided root node.
         * @param tokenSet The token set to parse
         * @param root The root node to append the generated nodes to.
         */
        void parse(TokenSet &tokenSet, Node &root);
    }
}

#endif //STRIDE_LANGUAGE_ASTNODES_H
