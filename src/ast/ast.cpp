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
 * Prints the content of this node to the console.
 */
void Node::print(Node &reference, int depth)
{
    printf("%*s", depth * 2, "");
    if ( depth > 0 )
    { printf("↳ "); }
    switch ( reference.node_type )
    {
        case NODE_TYPE_IMPORT:
            printf("IMPORT");
            break;
        case NODE_TYPE_FUNCTION_PARAMETERS:
            printf("FUNCTION PARAMETERS");
            break;
        case NODE_TYPE_IDENTIFIER_REFERENCE:
            printf("IDENTIFIER REFERENCE");
            break;
        case NODE_TYPE_IDENTIFIER:
            printf("IDENTIFIER (%s)", (char *) reference.value);
            break;
        case NODE_TYPE_ENUMERATION_MEMBER:
            printf("ENUMERATION MEMBER");
            break;
        case NODE_TYPE_VARIABLE_TYPE:
            printf("VARIABLE TYPE (%s)", (char *) reference.value);
            if ( reference.flags & FLAG_VARIABLE_ARRAY)
            {
                printf(" (array)");
            }
            break;
        case NODE_TYPE_BLOCK:
            printf("BODY");
            if ( reference.branch_count == 0)
            {
                printf(" (empty) ");
            }
            break;
        case NODE_TYPE_VALUE:
            printf("VALUE (%s)", (char *) reference.value);
            break;
        case NODE_TYPE_FUNCTION_DEFINITION:
            printf("FUNCTION DECLARATION");
            if ( reference.flags & FLAG_FUNCTION_EXTERNAL)
            {
                printf(" (external)");
            }
            break;
        case NODE_TYPE_FOR_LOOP:
            printf("FOR LOOP");
            break;
        case NODE_TYPE_SHARED:
            printf("SHARED BLOCK");
            break;
        case NODE_TYPE_ARRAY:
            printf("ARRAY");
            break;
        case NODE_TYPE_CLASS:
            printf("CLASS");
            break;
        case NODE_TYPE_VARIABLE_DECLARATION:
            printf("VARIABLE DECLARATION");
            break;
        case NODE_TYPE_ENUMERATION:
            printf("ENUMERABLE DEF");
            break;
        default:
            printf("%d", reference.node_type);
    }
    if ( reference.flags & FLAG_OBJECT_SHARED)
    {
        printf(" (shared)");
    }
    printf("\n");

    for ( int i = 0; i < reference.branch_count; i++ )
    {
        print(reference.branches[ i ], depth + 1);
    }
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
        case TOKEN_PRIMITIVE_UNKNOWN:
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
            case TOKEN_KEYWORD_DEFINE:
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
            case TOKEN_IDENTIFIER:
            {
                cursor += parse_function_call(token_set, cursor, *root);
            }
                break;
                /**
                 * Parsing of an enum statement.
                 * An enum statement is denoted in the following format:
                 * enum <name> { KEYWORD: ..., ... }
                 */
            case TOKEN_KEYWORD_ENUM:
            {
                cursor += parse_enumerable(token_set, ++cursor, *root);
            }
                break;
                /**
                 * Parses a for loop.
                 * This statement must be in the following formats:
                 * 'for (var name: type = value; name comparison value; name operation value) { ... }'
                 * 'for (var name: type in array) { ... }'
                 */
            case TOKEN_KEYWORD_FOR:
            {
                cursor += parse_for_loop(token_set, ++cursor, *root);
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

/**
 * Parses the token stream into an abstract syntax tree.
 * @return An AST Node.
 */
Node *stride::ast::parse(ast_token_set_t &token_set)
{
    auto *root = new Node(NODE_TYPE_BLOCK, FLAG_SCOPE_LOCAL);
    stride::ast::parsePartial(root, token_set);
    Node::print(*root);
    return root;
}