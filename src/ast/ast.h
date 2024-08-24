/**///
// Created by Luca Warmenhoven on 19/08/2024.
//

#ifndef STRIDE_LANGUAGE_AST_H
#define STRIDE_LANGUAGE_AST_H

#include <cstdlib>
#include "../analysis/token.h"

#define BRANCH_GROWTH_FACTOR 2

/* 
 * Definitions of Node types.
 * These types determine what kind of operation will be performed.
 */
#define AST_NODE_OP_OPERATION            (0x01)
#define AST_NODE_OP_CONDITIONAL          (0x02)
#define AST_NODE_OP_REPETITION           (0x03)

#define AST_NODE_OP_VARIABLE_DECLARATION (0x04)
#define AST_NODE_OP_VARIABLE_ASSIGNMENT  (0x05)
#define AST_NODE_OP_VARIABLE_REFERENCE   (0x06)

#define AST_NODE_OP_RETURN               (0x07)
#define AST_NODE_OP_IMPORT               (0x08)
#define AST_NODE_OP_DECLARE              (0x09)
#define AST_NODE_OP_THROW                (0x0A)
#define AST_NODE_OP_ARRAY                (0x0B)
#define AST_NODE_OP_SWITCH               (0x0C)
#define AST_NODE_OP_CASE                 (0x0D)
#define AST_NODE_OP_DEFAULT              (0x0E)
#define AST_NODE_OP_BREAK                (0x0F)
#define AST_NODE_OP_FUNCTION_DEFINITION  (0x10)
#define AST_NODE_OP_FUNCTION_CALL        (0x11)
#define AST_NODE_OP_FUNCTION_PARAMETERS  (0x12)
#define AST_NODE_OP_IDENTIFIER           (0x13)
#define AST_NODE_OP_STRUCTURE            (0x14)
#define AST_NODE_OP_ENUMERATION          (0x15)
#define AST_NODE_OP_ENUMERATION_MEMBER   (0x16)
#define AST_NODE_OP_CLASS                (0x17)
#define AST_NODE_OP_BLOCK                (0x18)

#define AST_NODE_OP_VARIABLE_TYPE        (0x19)
#define AST_NODE_OP_VARIABLE_VALUE       (0x1A)
#define AST_NODE_OP_EXPRESSION           (0x1B)
#define AST_NODE_OP_SHARED               (0x1C)

/* 
 * Definitions of function types.
 * These types determine what kind of function will be created.
 * These values can be stored in a single byte, since they're all boolean
 * properties.
 */
#define AST_FLAG_FUNCTION_SHARED   (0x1)
#define AST_FLAG_FUNCTION_EXTERNAL (0x2)
#define AST_FLAG_FUNCTION_SHARED_BP   (1)
#define AST_FLAG_FUNCTION_EXTERNAL_BP (2)

#define AST_VARIABLE_IMMUTABLE (0x1)
#define AST_VARIABLE_ARRAY     (0x2)

#define AST_FLAG_BLOCK_SCOPE_GLOBAL (0x1)
#define AST_FLAG_BLOCK_SCOPE_LOCAL  (0x2)

/**
 * Definitions of operations.
 * These can be used as value for `ast_node_props_t`.
 */
#define AST_OPERATION_ADD (0x1)
#define AST_OPERATION_SUB (0x2)
#define AST_OPERATION_MUL (0x3)
#define AST_OPERATION_DIV (0x4)
#define AST_OPERATION_MOD (0x5)
#define AST_OPERATION_AND (0x6)
#define AST_OPERATION_OR  (0x7)
#define AST_OPERATION_XOR (0x8)
#define AST_OPERATION_NOT (0x9)

/**
 * Definitions of conditionals.
 * Similar to operation types, these are also used as value in
 * structure `ast_node_props_t`.
 */
#define AST_CONDITIONAL_LESS_THAN          (0x1)
#define AST_CONDITIONAL_GREATER_THAN       (0x2)
#define AST_CONDITIONAL_LESS_THAN_EQUAL    (0x3)
#define AST_CONDITIONAL_GREATER_THAN_EQUAL (0x4)
#define AST_CONDITIONAL_EQUAL              (0x5)
#define AST_CONDITIONAL_NOT_EQUAL          (0x6)

typedef unsigned long cursor_t;

namespace stride::ast
{

    /**
     * Structure definition of a token set.
     * A token set is a data properties that contains information about a
     * sequence of tokens, such as a pointer to the memory location of the tokens
     * themselves, as the length of the array.
     */
    typedef struct
    {
        /**
         * Pointer to the memory location of the tokens
         */
        token_t *tokens;

        /**
         * The amount of tokens this structure stores.
         */
        size_t token_count;

    } ast_token_set_t;

    /**
     *  Token seeking definitions.
     *  These functions can peak forward and backward in a token set,
     *  to analyse for surrounding tokens, and whether the syntax is correct.
     */

    token_t *previous(ast_token_set_t &token_set, cursor_t index);

    token_t *next(ast_token_set_t &token_set, cursor_t index);

    token_t *peak(ast_token_set_t &token_set, cursor_t index, int offset);

    bool hasNext(ast_token_set_t &token_set, cursor_t index);

    bool hasPrevious(cursor_t index);

    bool isPrevious(ast_token_set_t &token_set, token_type_t type, cursor_t index);

    bool isNext(ast_token_set_t &token_set, token_type_t type, cursor_t index);

    bool isInRange(ast_token_set_t &token_set, token_type_t type, cursor_t index, int range);

    int indexOfToken(ast_token_set_t &token_set, token_type_t type, int token_count, int from_index_inclusive);

    /**
     * AST Error handling function definitions
     */

    void error(const char *errorMessage, ...);

    void requiresToken(token_type_t type, ast_token_set_t &token_set, cursor_t index, const char *error_message, ...);

    bool isValidType(token_type_t type);

/*
 * Abstract syntax tree Node.
 */
    class Node
    {

    public:

        /**
         * Checks whether the Node is a leaf.
         * A leaf is a Node that has no branches.
         * @return
         */
        bool isLeaf() const;

        /**
         * Checks whether the Node has empty leaves.
         * An empty leaf is a Node that has no branches, and no value.
         * @return
         */
        bool hasEmptyLeaves() const;

        /**
         * Ensures that the Node has the minimum amount of branches.
         * This function will allocate more memory for the branches if the
         * current amount of branches is less than the minimum amount of branches.
         */
        void ensureMinimumBranches();

        /**
         * The upper limit of branches.
         * This is the maximum amount of branches this Node can have,
         * before allocating more memory.
         */
        size_t branch_count_upper_limit;

        /**
         * The value of the Node.
         * This can contain specific information about the Node,
         * such as the value of a variable, or the name of a function.
         */
        void *value;

        /**
         * The branches of the Node.
         * This is an array of AST nodes, which are the children of this Node.
         */
        Node *branches;
        size_t branch_count;

        /**
         * The flags of the Node.
         * This is used to store additional information about the Node,
         * such as whether the Node is a shared function, or an external function.
         */
        unsigned int flags;

        /**
         * The operation of the Node.
         * This is used to store the operation of the Node, such as addition, subtraction, etc.
         */
        unsigned int operation;

        void addBranch(Node *node);

        void setValue(void *newVal)
        {
            this->value = newVal;
        }

        /**
         * Default destructor for Node.
         * This destructor will free the memory allocated for the branches
         * and the value of the Node.
         */
        ~Node()
        {
            if ( branches != nullptr )
            {
                free(branches);
            }

            if ( value != nullptr )
            {
                free(value);
            }
        }

        /**
         * Constructor for an Node with a value.
         * This is used for when the Node has no branches,
         * but does have a value.
         * @param props The properties of the Node.
         * @param value The value of the Node.
         */
        Node(unsigned int operation, unsigned int flags, void *value) : Node(operation, flags, 0, nullptr, value) {}

        /**
         * Constructor for an Node without a value.
         * This is used for when the Node has no branches,
         * and no value. This constructor will mostly be used for specific
         * keywords, such as `return` or `break`.
         * @param props The properties of the Node.
         */
        Node(unsigned int operation, unsigned int flags) : Node(operation, flags, 0, nullptr, nullptr) {}

        /**
         * Default constructor for Node.
         * This constructor will create a new AST Node with the provided properties.
         * @param props The properties of the Node.
         * @param branch_count The amount of branches this Node has.
         * @param branches The branches of this Node.
         * @param value The value of this Node.
         */
        Node(unsigned int operation, unsigned int flags, size_t branch_count, Node *branches, void *value)
        {
            this->flags = flags;
            this->operation = operation;
            this->branch_count = branch_count;
            this->value = value;
            this->branch_count_upper_limit = 4;
            if ( branches != nullptr )
            {
                this->branches = branches;
            }
            else
            {
                this->branches = (Node *) malloc(sizeof(Node) * this->branch_count_upper_limit);
            }
        }
    };

    void parsePartial(Node *root, ast_token_set_t &token_set);

    Node *parse(ast_token_set_t & token_set);

    /**
     * Captures a closure with specified token boundaries.
     * If the boundaries are not found or there's an imbalance of boundaries,
     *
     * Defined in src/ast/syntax_parsing/code_blocks.cpp
     *
     * @param token_set The token set to capture the block from.
     * @param start_token The token that indicates the start of the block.
     * @param end_token The token that indicates the end of the block.
     * @param starting_index The index to start capturing from.
     * @param parent_node The parent Node to append the captured block to.
     * @return The amount of tokens that were skipped.
     */
    int capture_block(ast_token_set_t &token_set, token_type_t start_token, token_type_t end_token, int starting_index,
                      Node &parent_node);

    /**
     * Parses a 'shared' code block, which is similar to declaring a module.
     *
     * Defined in src/ast/syntax_parsing/shared_statement.cpp
     *
     * @param token_set The token set to parse the shared statement from.
     * @param index The index of the token set to start parsing from.
     * @param root The root Node to append the shared statement to.
     * @return The amount of tokens that were skipped.
     */
    int parse_shared_statement(ast_token_set_t &token_set, cursor_t index, Node &root);


    /**
     * Parses an import declaration.
     * This function will parse an import declaration in the following format:
     * ```
     * import "path/to/file"
     * ```
     *
     * Defined in src/ast/syntax_parsing/import_statement.cpp
     * @param token_set The token set to parse the import declaration from.
     * @param index The index of the token set to start parsing from.in
     * @param root The root Node to append the import declaration to.
     * @return How many tokens were skipped.
     */
    int parse_import_statement(ast_token_set_t &token_set, cursor_t index, Node &root);

    /**
     * Parses a variable declaration.
     * This function will parse a variable declaration in the following format:
     * ```
     * var name: type = value
     * ```
     * The function has to be called AFTER the var keyword, e.g. the input stream tokens
     * must look like:
     * var < 'keyword identifier: type = value'
     * @param token_set The token set to parse the variable declaration from.
     * @param index The index of the token set to start parsing from.
     * @param root The root Node to append the variable declaration to.
     * @return
     */
    int parse_function_declaration(ast_token_set_t &token_set, cursor_t index, Node &root);

    /**
     * Parses a variable declaration.
     * This function will parse a variable declaration in the following format:
     * ```
     * var name: type = value
     * ```
     * The function has to be called AFTER the var keyword, e.g. the input stream tokens
     * must look like:
     * var < 'keyword identifier: type = value'
     * @param token_set The token set to parse the variable declaration from.
     * @param index The index of the token set to start parsing from.
     * @param root The root Node to append the variable declaration to.
     * @return
     */
    int parse_variable_declaration(ast_token_set_t &token_set, cursor_t index, Node &root);

    /**
     * Validates a variable declaration.
     * This function will validate whether a variable declaration is correct.
     * If the variable declaration is incorrect, the program will exit with an error.r
     * @param token_set The token set to validate the variable declaration from.
     * @param index The index of the token set to start validating from.
     */
    void validate_variable_declaration(ast_token_set_t &token_set, cursor_t index);

    /**
     * Parses an expression, which can be after variable declaration / assignment,
     * or providing function parameters.
     * An expression can be in the following format:
     * ```
     * var k: unknown = (5 + (3 * 4))
     * ```
     * Here, the segment after the `unknown = ...` is the expression,
     * which will be parsed using this function.
     * @param token_set The token set to parse a segment from
     * @param cursor The cursor position in the token set
     * @param token_count The amount of tokens in the subset
     * @param parent_node The parent AST Node to put the result into.
     * @return How many tokens were skipped.
     */
    int parse_expression(ast_token_set_t &token_set, size_t cursor, size_t token_count, Node &parent_node);
}

#endif //STRIDE_LANGUAGE_AST_H
