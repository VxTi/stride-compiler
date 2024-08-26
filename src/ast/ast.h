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
 * These types determine what kind of node_type will be performed.
 */
#define NODE_TYPE_OPERATION            (0x01)
#define NODE_TYPE_CONDITIONAL          (0x02)
#define NODE_TYPE_REPETITION           (0x03)

#define NODE_TYPE_VARIABLE_DECLARATION (0x04)
#define NODE_TYPE_VARIABLE_ASSIGNMENT  (0x05)
#define NODE_TYPE_VARIABLE_REFERENCE   (0x06)

#define NODE_TYPE_RETURN               (0x07)
#define NODE_TYPE_IMPORT               (0x08)
#define NODE_TYPE_IDENTIFIER_REFERENCE (0x09)
#define NODE_TYPE_THROW                (0x0A)
#define NODE_TYPE_ARRAY                (0x0B)
#define NODE_TYPE_SWITCH               (0x0C)
#define NODE_TYPE_CASE                 (0x0D)
#define NODE_TYPE_DEFAULT              (0x0E)
#define NODE_TYPE_BREAK                (0x0F)
#define NODE_TYPE_FUNCTION_DEFINITION  (0x10)
#define NODE_TYPE_FUNCTION_CALL        (0x11)
#define NODE_TYPE_FUNCTION_PARAMETERS  (0x12)
#define NODE_TYPE_IDENTIFIER           (0x13)
#define NODE_TYPE_STRUCTURE            (0x14)
#define NODE_TYPE_ENUMERATION          (0x15)
#define NODE_TYPE_ENUMERATION_MEMBER   (0x16)
#define NODE_TYPE_CLASS                (0x17)
#define NODE_TYPE_BLOCK                (0x18)

#define NODE_TYPE_VARIABLE_TYPE        (0x19)
#define NODE_TYPE_VALUE                (0x1A)
#define NODE_TYPE_EXPRESSION           (0x1B)
#define NODE_TYPE_SHARED               (0x1C)
#define NODE_TYPE_FOR_LOOP             (0x1D)
#define NODE_TYPE_FOR_LOOP_INIT        (0x1E)
#define NODE_TYPE_FOR_LOOP_CONDITIONAL (0x1F)


#define FLAG_VARIABLE_IMMUTABLE (0x1) // Whether a variable is immutable
#define FLAG_VARIABLE_ARRAY     (0x2) // Whether a variable is an array

#define FLAG_OBJECT_SHARED     (0x1) // Whether the function or variable declaration is shared with other modules.
#define FLAG_FUNCTION_EXTERNAL (0x2)

#define FLAG_SCOPE_GLOBAL (0x1)
#define FLAG_SCOPE_LOCAL  (0x2)
#define FLAG_SCOPE_CLASS  (0x3)

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
 * Similar to node_type types, these are also used as value in
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

    /**
     * Returns an integer representing whether the the token in the set at the given position
     * is equal to the provided type.
     * @param token_set The token set to check from
     * @param index The index to start from
     * @param offset The offset, relative from the index
     * @param type The type to check for at the provided location
     * @return An integer representing whether the token at the given location is equal to the provided type.
     * Returns 1 for a match, 0 otherwise.
     */
    int peakeq(ast_token_set_t &token_set, cursor_t index, int offset, token_type_t type);

    bool has_next(ast_token_set_t &token_set, cursor_t index);

    bool has_previous(cursor_t index);

    bool is_previous(ast_token_set_t &token_set, token_type_t type, cursor_t index);

    bool is_next(ast_token_set_t &token_set, token_type_t type, cursor_t index);

    /**
     * AST Error handling function definitions
     */

    void error(const char *errorMessage, ...);

    void requires_token(token_type_t type, ast_token_set_t &token_set, cursor_t index, const char *error_message, ...);

    bool is_valid_var_type(token_type_t type);

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
         * Prints the content of this node to the console.
         */
        static void print(Node &reference, int depth = 0);

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
        
        /**
         * The amount of branches this Node has.
         */
        size_t branch_count;

        /**
         * The flags of the Node.
         * This is used to store additional information about the Node,
         * such as whether the Node is a shared function, or an external function.
         */
        unsigned int flags;

        /**
         * The node_type of the Node.
         * This is used to store the node_type of the Node, such as addition, subtraction, etc.
         */
        unsigned int node_type;

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
        Node(unsigned int node_type, unsigned int flags, void *value) : Node(node_type, flags, 0, nullptr, value) {}

        /**
         * Constructor for an Node without a value.
         * This is used for when the Node has no branches,
         * and no value. This constructor will mostly be used for specific
         * keywords, such as `return` or `break`.
         * @param props The properties of the Node.
         */
        Node(unsigned int node_type, unsigned int flags = 0) : Node(node_type, flags, 0, nullptr, nullptr) {}

        /**
         * Default constructor for Node.
         * This constructor will create a new AST Node with the provided properties.
         * @param props The properties of the Node.
         * @param branch_count The amount of branches this Node has.
         * @param branches The branches of this Node.
         * @param value The value of this Node.
         */
        Node(unsigned int node_type, unsigned int flags, size_t branch_count, Node *branches, void *value)
        {
            this->flags = flags;
            this->node_type = node_type;
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
    int parse_block(ast_token_set_t &token_set, token_type_t start_token, token_type_t end_token, int starting_index,
                    Node &parent_node);

    /**
     * Captures a block and returns the referring token set.
     * This function is similar to 'parse_block', except this one does not parse the content of the acquired
     * block. This can be used in other parsing functions to validate whether the content of an abstraction
     * follows the right format.
     * @param token_set The token set to analyse
     * @param start_token The starting token of the block
     * @param end_token The ending token of the block
     * @param starting_index The index to start scanning from
     * @return An object containing the parsed tokens.
     */
    ast_token_set_t *capture_block(ast_token_set_t &token_set, token_type_t start_token, token_type_t end_token, int starting_index);

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

    /**
     * Parses an identifier into either a single or multiple nodes.
     * This parsing depends on whether the identifier refers to a module,
     * or a locally declared variable/function.
     * @param token_set The token set to parse the identifier from.
     * @param index The index of the token set to start parsing from.in
     * @param root The root Node to append the identifier to.
     * @return How many tokens were skipped.
     */
    int parse_identifier(ast_token_set_t &token_set, cursor_t index, Node &root);

    /**
     * Parsing of an enumerable. Enumerable statements currently accept integers only.
     * The parsing function starts after the enum keyword, like all other parsing functions.
     * It is therefore required to only call this method after an 'enum' keyword has been
     * found, otherwise the program will exit immediately.
     * @param token_set The token set to parse the enum from
     * @param index The index to start parsing from
     * @param root The root element to add the enumerable object to.
     * @return How many tokens were skipped.
     */
    int parse_enumerable(ast_token_set_t &token_set, cursor_t index, Node &root);

    /**
     * Parsing of a for loop.
     * This accepts a few different kinds of formats.
     * The regular format, e.g. 'for (var i: i8 = 0; i < ...; i++) { ... }
     * and the iterable one: 'for (var k: string in [...]) {}'
     * @param token_set The token set to parse the for loop from
     * @param index The index to start parsing from
     * @param root The root element to add the resulting nodes to.
     * @return How many tokens were skipped.
     */
    int parse_for_loop(ast_token_set_t &token_set, cursor_t index, Node &root);

    /**
     * Parses a function call.
     * Function calls can be in the format of 'module::function(...)'
     */
    int parse_function_call(ast_token_set_t &token_set, cursor_t index, Node &root);
}

#endif //STRIDE_LANGUAGE_AST_H
