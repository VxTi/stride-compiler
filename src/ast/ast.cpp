//
// Created by Luca Warmenhoven on 19/08/2024.
//

#include "ast.h"
#include "syntax/variable_types.h"

using namespace stride::ast;

std::string stride::ast::current_file_name;
std::string stride::ast::current_file_content;

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
 * Adds the provided node to this node.
 * @param node The Node to add to this node (the root)
 */
void Node::add_branch(Node *node)
{
    this->ensureMinimumBranches();
    node->parent_index = (int) this->branch_count;
    this->branches[ this->branch_count++ ] = *node;
    node->parent = this;
}

void Node::print_static(int node_type)
{
    switch ( node_type )
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
            printf("IDENTIFIER");
            break;
        case NODE_TYPE_ENUMERATION_MEMBER:
            printf("ENUMERATION MEMBER");
            break;
        case NODE_TYPE_VARIABLE_TYPE:
            printf("VARIABLE TYPE");
            break;
        case NODE_TYPE_BLOCK:
            printf("BODY");
            break;
        case NODE_TYPE_VALUE:
            printf("VALUE");
            break;
        case NODE_TYPE_GENERICS:
            printf("GENERICS");
            break;
        case NODE_TYPE_FUNCTION_DEFINITION:
            printf("FUNCTION DECLARATION");
            break;
        case NODE_TYPE_SWITCH:
            printf("SWITCH");
            break;
        case NODE_TYPE_CASE:
            printf("CASE");
            break;
        case NODE_TYPE_DEFAULT:
            printf("DEFAULT");
            break;
        case NODE_TYPE_IF:
            printf("IF STATEMENT");
            break;
        case NODE_TYPE_FOR_LOOP:
            printf("FOR LOOP");
            break;
        case NODE_TYPE_INHERITS:
            printf("INHERITS");
            break;
        case NODE_TYPE_STRUCTURE:
            printf("STRUCTURE DEFINITION");
            break;
        case NODE_TYPE_MODULE:
            printf("MODULE DEFINITION");
            break;
        case NODE_TYPE_ARRAY:
            printf("ARRAY");
            break;
        case NODE_TYPE_TRY_CATCH:
            printf("TRY CATCH");
            break;
        case NODE_TYPE_CLASS:
            printf("CLASS DEFINITION");
            break;
        case NODE_TYPE_VARIABLE_DECLARATION:
            printf("VARIABLE DECLARATION");
            break;
        case NODE_TYPE_FUNCTION_CALL:
            printf("FUNCTION CALL");
            break;
        case NODE_TYPE_RETURN:
            printf("RETURN");
            break;
        case NODE_TYPE_ENUMERATION:
            printf("ENUMERABLE DEF");
            break;
        case NODE_TYPE_CONDITIONAL:
            printf("CONDITIONAL");
            break;
        case NODE_TYPE_WHILE_LOOP:
            printf("WHILE LOOP");
            break;
        case NODE_TYPE_THROW:
            printf("THROW");
            break;
        case NODE_TYPE_DO_WHILE:
            printf("DO WHILE LOOP");
            break;
        case NODE_TYPE_EXPRESSION:
            printf("EXPRESSION");
            break;
        default:
            printf("%d", node_type);
    }
    printf("\n");
}

/**
 * Prints the content of this node to the console.
 */
void Node::print(Node &reference, int depth)
{
    /** ** ** TO BE REMOVED IN THE FUTURE ** ** **/
    // This is a temporary function to print the AST to the console, for debugging purposes.

    printf("\e[38;5;%dm%*s", 9 + depth % 6, depth * 2, "");
    if ( depth > 0 )
    {
        printf("─ ");
    }
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
            printf("VARIABLE TYPE");
            if ( reference.flags & FLAG_VARIABLE_ARRAY)
            {
                printf(" (array)");
            }
            break;
        case NODE_TYPE_BLOCK:
            printf("BODY");
            if ( reference.branch_count == 0 )
            {
                printf(" (empty) ");
            }
            break;
        case NODE_TYPE_VALUE:
            if ( reference.parent != nullptr && reference.parent->node_type == NODE_TYPE_ENUMERATION_MEMBER)
            {
                printf("ENUM VALUE (%lld)", (long long) reference.value);
            }
            else
            { printf("VALUE (%s)", (char *) reference.value); }
            break;
        case NODE_TYPE_GENERICS:
            printf("GENERICS");
            break;
        case NODE_TYPE_FUNCTION_DEFINITION:
            printf("FUNCTION DECLARATION");
            if ( reference.flags & FLAG_FUNCTION_EXTERNAL)
            {
                printf(" (external)");
            }
            if ( reference.flags & FLAG_FUNCTION_ASYNC)
            {
                printf(" (async)");
            }
            break;
        case NODE_TYPE_SWITCH:
            printf("SWITCH");
            break;
        case NODE_TYPE_CASE:
            printf("CASE");
            break;
        case NODE_TYPE_DEFAULT:
            printf("DEFAULT");
            break;
        case NODE_TYPE_IF:
            printf("IF STATEMENT");
            break;
        case NODE_TYPE_FOR_LOOP:
            printf("FOR LOOP");
            break;
        case NODE_TYPE_INHERITS:
            printf("INHERITS");
            break;
        case NODE_TYPE_STRUCTURE:
            printf("STRUCTURE DEFINITION");
            break;
        case NODE_TYPE_MODULE:
            printf("MODULE DEFINITION");
            break;
        case NODE_TYPE_ARRAY:
            printf("ARRAY");
            break;
        case NODE_TYPE_TRY_CATCH:
            printf("TRY CATCH");
            break;
        case NODE_TYPE_CLASS:
            printf("CLASS DEFINITION");
            break;
        case NODE_TYPE_VARIABLE_DECLARATION:
            printf("VARIABLE DECLARATION");
            if ( reference.flags & FLAG_VARIABLE_IMMUTABLE)
            {
                printf(" (immutable)");
            }
            if ( reference.flags & FLAG_VARIABLE_ARRAY)
            {
                printf(" (array)");
            }
            break;
        case NODE_TYPE_FUNCTION_CALL:
            printf("FUNCTION CALL");
            break;
        case NODE_TYPE_RETURN:
            printf("RETURN");
            break;
        case NODE_TYPE_ENUMERATION:
            printf("ENUMERABLE DEF");
            break;
        case NODE_TYPE_CONDITIONAL:
            printf("CONDITIONAL");
            break;
        case NODE_TYPE_WHILE_LOOP:
            printf("WHILE LOOP");
            break;
        case NODE_TYPE_THROW:
            printf("THROW");
            break;
        case NODE_TYPE_DO_WHILE:
            printf("DO WHILE LOOP");
            break;
        case NODE_TYPE_EXPRESSION:
            printf("EXPRESSION");
            break;
        default:
            printf("%d", reference.node_type);
    }
    if ( reference.flags & FLAG_OBJECT_PUBLIC)
    {
        printf(" (public)");
    }
    printf("\n");

    for ( int i = 0; i < reference.branch_count; i++ )
    {
        print(reference.branches[ i ], depth + 1);
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
void stride::ast::parse_tokens(Node *root, ast_token_set_t &token_set)
{
    cursor_t cursor;

    for ( cursor = 0; cursor < token_set.token_count; )
    {
        if ( token_set.tokens[ cursor ].type == 0 )
        {
            cursor++;
        }
        else
        {
            switch ( token_set.tokens[ cursor ].type )
            {

                // Function definitions must follow the format 'fn <name>( ... )' where ... = param: (...)properties([]), ...
                case TOKEN_KEYWORD_DEFINE:
                {
                    cursor += parse_function_declaration(token_set, ++cursor, *root);
                }
                    break;
                case TOKEN_KEYWORD_IF:
                {
                    cursor += parse_if_else(token_set, ++cursor, *root);
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
                case TOKEN_KEYWORD_LET:
                case TOKEN_KEYWORD_CONST:
                {
                    cursor += parse_variable_declaration(token_set, cursor, *root);
                }
                    break;
                case TOKEN_KEYWORD_STRUCT:
                {
                    cursor += parse_structure(token_set, ++cursor, *root);
                }
                    break;
                case TOKEN_KEYWORD_TRY:
                {
                    cursor += parse_try_catch(token_set, ++cursor, *root);
                }
                    break;
                case TOKEN_KEYWORD_SWITCH:
                {
                    cursor += parse_switch_case(token_set, ++cursor, *root);
                }
                    break;
                case TOKEN_IDENTIFIER:
                {
                    if ( is_function_call(token_set, cursor))
                    {
                        cursor += parse_function_call(token_set, cursor, *root);
                    }
                    else
                    {
                        blame_token(token_set.tokens[ cursor ], "Unexpected trailing token found.");
                    }
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
                case TOKEN_KEYWORD_PUBLIC:
                {
                    token_t *next_token = peak(token_set, cursor, 1);
                    if ( next_token != nullptr && types::is_functional_keyword(next_token->type))
                    {
                        cursor++;
                    }
                    else
                    {
                        blame_token(token_set.tokens[ cursor ],
                                    "Illegal 'public' keyword found. Public identifiers may only be used before functional statements.");
                    }
                }
                    break;
                case TOKEN_KEYWORD_THROW:
                {
                    cursor += parse_throw(token_set, ++cursor, *root);
                }
                    break;
                case TOKEN_KEYWORD_DO:
                {
                    cursor += parse_do_while(token_set, ++cursor, *root);
                }
                    break;
                case TOKEN_KEYWORD_WHILE:
                {
                    cursor += parse_while_loop(token_set, ++cursor, *root);
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
                case TOKEN_KEYWORD_MODULE:
                {
                    cursor += parse_module_statement(token_set, ++cursor, *root);
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
                case TOKEN_KEYWORD_CLASS:
                {
                    cursor += parse_class(token_set, cursor, *root);
                }
                    break;
                case TOKEN_KEYWORD_RETURN:
                {
                    cursor += parse_returning(token_set, cursor, *root);
                }
                    break;
                case TOKEN_SEMICOLON:
                    cursor++;
                    break;
                default:
                    blame_token(token_set.tokens[ cursor ], "Unexpected token found.");
            }
        }
    }
}

/**
 * Parses the token stream into an abstract syntax tree.
 * @return An AST Node.
 */
Node *stride::ast::parse(ast_token_set_t &token_set)
{
    auto *root = new Node(NODE_TYPE_BLOCK, FLAG_SCOPE_GLOBAL);
    stride::ast::parse_tokens(root, token_set);
    Node::print(*root);
    return root;
}