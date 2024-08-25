//
// Created by Luca Warmenhoven on 19/08/2024.
//

#include "ast.h"

using namespace stride::ast;

/**
 * Returns whether the AST Node is a leaf or not (no branches)
 * @return
 */
bool Node::isLeaf() const
{
    return this->branch_count == 0;
}

/**
 * Checks whether the Node has
 * @return
 */
bool Node::hasEmptyLeaves() const
{
    if ( this->branch_count == 0 )
    {
        return false;
    }

    for ( size_t i = 0; i < this->branch_count; i++ )
    {
        if ( this->branches[ i ].branch_count > 0 )
        {
            return false;
        }
    }
    return true;
}

/**
 * Ensures that the Node has a minimum amount of branches.
 * @param minimum The minimum amount of branches.
 */
void Node::ensureMinimumBranches()
{
    if ( this->branch_count + 1 >= this->branch_count_upper_limit )
    {
        // prevent integer overflow
        if ( this->branch_count_upper_limit * BRANCH_GROWTH_FACTOR < 0 )
        {
            fprintf(stderr, "Attempting to increase branch buffer size lead to integer overflow.");
            exit(1);
        }
        // Grows exponentially to reduce frequent resizing of buffer.
        auto *newBranches = (Node *) malloc(sizeof(Node) * this->branch_count_upper_limit * BRANCH_GROWTH_FACTOR);

        if ( !newBranches )
        {
            fprintf(stderr, "Failed to allocate memory for new branches.");
            exit(1);
        }

        memcpy(newBranches, this->branches, sizeof(Node) * this->branch_count);
        free(this->branches);
        this->branches = newBranches;
        this->branch_count_upper_limit *= BRANCH_GROWTH_FACTOR;
    }
}

/**
 * Adds a branch to the Node.
 * @param node The Node to add.
 */
void Node::addBranch(Node *node)
{
    this->ensureMinimumBranches();
    this->branches[ this->branch_count++ ] = *node;
}

/**
 * Whether the provided token properties is an acceptable
 * variable properties. This is only true for primitives and identifiers.
 * @param type The properties to check.
 * @return
 */
bool stride::ast::is_valid_var_type(token_type_t type)
{
    switch ( type )
    {
        case TOKEN_IDENTIFIER:
        case TOKEN_PRIMITIVE_INT8:
        case TOKEN_PRIMITIVE_INT16:
        case TOKEN_PRIMITIVE_INT32:
        case TOKEN_PRIMITIVE_INT64:
        case TOKEN_PRIMITIVE_FLOAT32:
        case TOKEN_PRIMITIVE_FLOAT64:
        case TOKEN_PRIMITIVE_BOOL:
        case TOKEN_PRIMITIVE_STRING:
        case TOKEN_PRIMITIVE_CHAR:
            return true;
        default:
            return false;
    }
}

/**
 * Parses a partial block of code into an abstract syntax tree.
 * This function will recursively parse the block and its children.
 * @param partial_tokens The token stream.
 * @param partial_token_count The number of partial_tokens in the stream.
 * @param index The current index.
 * @return The parsed bracket Node.
 */
void stride::ast::parsePartial(Node *root, ast_token_set_t &token_set)
{
    cursor_t cursor;

    for ( cursor = 0; cursor < token_set.token_count; )
    {
        switch ( token_set.tokens[ cursor ].type )
        {

            // Function definitions must follow the format 'fn <name>( ... )' where ... = param: (...)properties([]), ...
            case TOKEN_KEYWORD_DECLARE:
            {
                cursor += parse_function_declaration(token_set, ++cursor, *root);
            }
                break;

                /**
                 * Variable parsing.
                 * A variable must be declared in the following format:
                 * ```
                 * var name: type = value;
                 *
                 * // Declaring multiple at the same time:
                 * var a: i8, b: i8;
                 * ```
                 */
            case TOKEN_KEYWORD_VAR:
            {
                cursor += parse_variable_declaration(token_set, cursor, *root);
            }
                break;

                /**
                 * Declares a namespace to group code together.
                 */
            case TOKEN_KEYWORD_SHARED:
            {

                cursor += parse_shared_statement(token_set, ++cursor, *root);
            }
                break;
                /**
                 * Allows one to import external files using 'import "..."' or 'import "..." as keyword'
                 */
            case TOKEN_KEYWORD_IMPORT:
            {
                cursor += parse_import_statement(token_set, ++cursor, *root);
            }
                break;
            default:
                cursor++;
                break;
        }
    }
}

void print_type(Node &node)
{
    switch ( node.node_type )
    {
        case NODE_TYPE_BLOCK:
            printf("BLOCK\n");
            break;
        case TOKEN_KEYWORD_CLASS:
            printf("CLASS\n");
            break;
        case NODE_TYPE_FUNCTION_DEFINITION:
            printf("FUNCTION_DEF\n");
            break;
        case NODE_TYPE_VARIABLE_DECLARATION:
            printf("DECLARATION\n");
            break;
        case NODE_TYPE_IDENTIFIER:
            printf("IDENTIFIER\n");
            break;
        case NODE_TYPE_RETURN:
            printf("RETURN\n");
            break;
        case NODE_TYPE_STRUCTURE:
            printf("STRUCTURE\n");
            break;
        default:
            printf("UNKNOWN\n");
            break;
    }
}

void recursive_print(Node &node, int depth)
{
    if ( depth > 0 )
    { printf("|"); }
    for ( int i = 0; i < depth; i++ )
    {
        printf("_");
    }
    print_type(node);

    for ( size_t i = 0; i < node.branch_count; i++ )
    {
        recursive_print(node.branches[ i ], depth + 1);
    }

}

/**
 * Parses the token stream into an abstract syntax tree.
 * @return An AST Node.
 */
Node *stride::ast::parse(ast_token_set_t &token_set)
{
    auto *root = new Node(NODE_TYPE_BLOCK, AST_FLAG_BLOCK_SCOPE_LOCAL);
    stride::ast::parsePartial(root, token_set);

    recursive_print(*root, 0);

    return root;
}