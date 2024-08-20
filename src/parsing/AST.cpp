//
// Created by Luca Warmenhoven on 19/08/2024.
//

#include "AST.h"

using namespace stride::ast;

/**
 * Returns whether the AST Node is a leaf or not (no branches)
 * @return
 */
bool ASTNode::isLeaf() const
{
    return this->branch_count == 0;
}

/**
 * Checks whether the node has
 * @return
 */
bool ASTNode::hasEmptyLeaves() const
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
 * Ensures that the node has a minimum amount of branches.
 * @param minimum The minimum amount of branches.
 */
void ASTNode::ensureMinimumBranches()
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
        auto *newBranches = (ASTNode *) malloc(sizeof(ASTNode) * this->branch_count_upper_limit * BRANCH_GROWTH_FACTOR);

        if ( !newBranches )
        {
            fprintf(stderr, "Failed to allocate memory for new branches.");
            exit(1);
        }

        memcpy(newBranches, this->branches, sizeof(ASTNode) * this->branch_count);
        free(this->branches);
        this->branches = newBranches;
        this->branch_count_upper_limit *= BRANCH_GROWTH_FACTOR;
    }
}

/**
 * Adds a branch to the node.
 * @param node The node to add.
 */
void ASTNode::addBranch(ASTNode *node)
{
    this->ensureMinimumBranches();
    this->branches[ this->branch_count++ ] = *node;
}


/**
 * Creates a new comparative node.
 * @param left The left node of the comparison
 * @param right The right node of the comparison
 * @param comparison The comparative function.
 * @return
 */
ASTNode *createComparison(ASTNode *left, ASTNode *right, token_type_t comparison)
{
    switch ( comparison )
    {
        case TOKEN_LESS:
        case TOKEN_GREATER:
        case TOKEN_LEQUALS:
        case TOKEN_GEQUALS:
        case TOKEN_DOUBLE_EQUALS:
        case TOKEN_NOT_EQUALS:
        {
            auto node = new ASTNode((ast_node_type_t) comparison);
            node->addBranch(left);
            node->addBranch(right);
            return node;
        }
        default:
            fprintf(stderr, "Invalid comparison token\n");
            exit(1);
    }
}

/**
 * Checks if there is a next token in the token stream.
 * @return True if there is a next token, false otherwise.
 */
bool hasNext(ast_token_set_t &token_set, size_t index)
{
    return index + 1 < token_set.token_count;
}

/**
 * Whether there is a token before the current one
 * @return
 */
bool hasPrevious(size_t index)
{
    return index > 0;
}

/**
 * Returns the next token in the token stream, from
 * the current index.
 * @return
 */
token_t *next(ast_token_set_t &token_set, size_t index)
{
    if ( !hasNext(token_set, index))
    {
        return nullptr;
    }

    return &token_set.tokens[ index + 1 ];
}

/**
 * Returns the previous token in the token stream, from the
 * current index.
 * @return
 */
token_t *previous(ast_token_set_t &token_set, size_t index)
{
    if ( !hasPrevious(index))
    {
        return nullptr;
    }

    return &token_set.tokens[ index - 1 ];
}

/**
 * Returns the token at the nth offset from the cursor position,
 * e.g. cursor = 0, peak(1) will return the token at position 1. (cursor + n)
 * @param offset
 * @return
 */
token_t *peak(ast_token_set_t &token_set, size_t index, int offset)
{
    int i = index + offset;
    if ( i < 0 || i >= token_set.token_count )
    {
        return nullptr;
    }

    return &token_set.tokens[ i ];
}

/**
 * Whether the previous token is of the provided type.
 * If there are no tokens left, the function will return false.
 * @param type The type to check for
 * @return
 */
bool isPrevious(ast_token_set_t &token_set, token_type_t type, size_t index)
{
    if ( !hasPrevious(index))
    {
        return false;
    }

    return previous(token_set, index)->type == type;
}

/**
 * Whether the next token is of the provided type.
 * If there are no tokens left, the function will return false.
 * @param type The type to check for
 * @return
 */
bool isNext(ast_token_set_t &token_set, token_type_t type, size_t index)
{
    if ( !hasNext(token_set, index))
    {
        return false;
    }

    return next(token_set, index)->type == type;
}

/**
 * Checks whether the provided type is within the provided range,
 * from the current index. If one wants to check if there's a token of a specific type
 * within the next -3n || +3n of the cursor, this function is yours.
 * If range = 0, the function will check whether the current token is of the provided type.
 * @param type The type to check
 * @param range The offset to provide
 * @return
 */
bool isInRange(ast_token_set_t &token_set, token_type_t type, size_t index, int range)
{
    if ( range == 0 )
    {
        return token_set.tokens[ index ].type == type;
    }

    long start = index;
    long end = index + range;


    // Swap range if range is negative
    if ( start > end )
    {
        std::swap(start, end);
    }

    // Check whether token type is in range
    for ( size_t i = start; i < end; i++ )
    {
        if ( token_set.tokens[ i ].type == type )
        {
            return true;
        }
    }
    return false;
}

/**
 * Whether the provided token type is an acceptable
 * variable type. This is only true for primitives and identifiers.
 * @param type The type to check.
 * @return
 */
bool isValidType(token_type_t type)
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
 * Exits the program with an error message.
 * @param errorMessage The error message to display.
 */
void error(const char *errorMessage, ...)
{
    va_list args;
    va_start(args, errorMessage);
    vfprintf(stderr, errorMessage, args);
    va_end(args);
    exit(1);
}

/**
 * Exits the program if the next token is not of a certain type.
 * @param type The type to check for.
 * @param offset The offset to check.p
 * @param errorMessage The error message to display.
 */
void requiresAt(token_type_t type, ast_token_set_t &token_set, size_t index, const char *error_message, ...)
{
    if ( index >= token_set.token_count || token_set.tokens[ index ].type != type )
    {

        va_list args;
        va_start(args, error_message);

        // Prevent empty token sets from causing a crash
        if ( token_set.token_count == 0 )
        {
            fprintf(stderr, "\n\nError parsing tokens - No tokens found.");
            exit(1);
        }

        token_t ref = token_set.tokens[
                ( index >= token_set.token_count ? token_set.token_count - 1 : index == 0 ? 0 : index - 1 )
        ];

        fprintf(stderr, "\n\nError at line %d column %d\n", ref.line, ref.column);
        vfprintf(stderr, error_message, args);
        va_end(args);
        exit(1);
    }
}

int parseVariableDeclaration(ast_token_set_t &token_set, size_t cursor, size_t length, ASTNode &parent_node)
{
    bool has_next = false;
    int skipped = 0;

    do {
        if ( !hasNext(token_set, cursor))
        {
            error("Expected variable name, but received none.");
        }

        requiresAt(TOKEN_IDENTIFIER, token_set, cursor, "Expected variable name, but received none.");

        // Check whether the colon is also present
        requiresAt(TOKEN_COLON, token_set, cursor + 1, "Expected colon after variable name, but received none.");

        // Check if the type is valid
        if ( !isValidType(peak(token_set, cursor, 2)->type))
        {
            error("Received invalid type after token declaration at line %d column %d: %s",
                  token_set.tokens[ cursor + 1 ].line, token_set.tokens[ cursor + 1 ].column,
                  token_set.tokens[ cursor + 1 ].value);
        }

        // Create a declaration node
        auto *declaration = (ast_declaration_node_t *) malloc(sizeof(ast_declaration_node_t));
        declaration->variable_name = token_set.tokens[ cursor ].value;
        declaration->variable_type = token_set.tokens[ cursor + 2 ].value;
        declaration->immutable = isPrevious(token_set, TOKEN_KEYWORD_CONST, cursor);

        cursor += 3;

        auto *declNode = new ASTNode(DECLARATION);
        declNode->setValue(declaration);

        // The first node is the one containing information about the variable
        auto nameNode = new ASTNode(IDENTIFIER);
        nameNode->setValue((void *) declaration);
        declNode->addBranch(nameNode);

        if ( token_set.tokens[cursor].type == TOKEN_EQUALS) {
            // Parse the content of the variable assignment.
        }

        has_next = isNext(token_set, TOKEN_COMMA, cursor);

    } while ( has_next );

    return skipped;
}

/**
 * Parses a partial block of code into an abstract syntax tree.
 * This function will recursively parse the block and its children.
 * @param partial_tokens The token stream.
 * @param partial_token_count The number of partial_tokens in the stream.
 * @param index The current index.
 * @return The parsed bracket node.
 */
void stride::ast::parsePartial(ASTNode *root, ast_token_set_t &token_set)
{
    size_t cursor, j, k, l;

    printf("Parsing partial %zu\n", token_set.token_count);

    for ( cursor = 0; cursor < token_set.token_count; )
    {
        switch ( token_set.tokens[ cursor ].type )
        {

            // Function definitions must follow the format 'fn <name>( ... )' where ... = param: (...)type([]), ...
            case TOKEN_KEYWORD_FN:
            {
                if ( !hasNext(token_set, cursor))
                {
                    error("Expected function name, but received none.");
                }

                requiresAt(TOKEN_IDENTIFIER, token_set, cursor + 1, "Expected function name");
                requiresAt(TOKEN_LPAREN, token_set, cursor + 2, "Expected opening parenthesis");

                std::vector<ast_function_param_t> parameters;

                // Move forward until we find a closing parenthesis
                for ( j = cursor + 3, k = 1;
                      j < token_set.token_count; j++ )
                {
                    // End if token is RPAREN
                    if ( token_set.tokens[ j ].type == TOKEN_RPAREN )
                    {
                        break;
                    }

                    ast_function_param_t parameter;

                    // If there's a 'const' keyword before the parameter declaration,
                    // we'll move the cursor and declare it immutable.
                    if ( token_set.tokens[ j ].type == TOKEN_KEYWORD_CONST )
                    {
                        parameter.immutable = true;
                        printf("Found const variable");
                        j++;
                    }

                    // Check whether the identifier is present at the required location
                    requiresAt(TOKEN_IDENTIFIER, token_set, j,
                               "Expected parameter name, but received %s",
                               token_set.tokens[ j ].value);

                    parameter.name = (char *) token_set.tokens[ j ].value;

                    // Check whether the colon is also present
                    requiresAt(TOKEN_COLON, token_set, j + 1,
                               "Expected colon after parameter, but received %s",
                               token_set.tokens[ j + 1 ].value);


                    // Check if the parameter is variadic (array)
                    if ( peak(token_set, j, 2)->type == TOKEN_THREE_DOTS &&
                         isValidType(peak(token_set, j, 3)->type))
                    {
                        parameter.array = true;
                        parameter.name = (char *) peak(token_set, j, 3)->value;
                    }
                    else if ( isValidType(peak(token_set, j, 2)->type))
                    {
                        parameter.type = peak(token_set, j, 2)->type;
                        if ( peak(token_set, j, 3)->type == TOKEN_LBRACKET &&
                             peak(token_set, j, 4)->type == TOKEN_RBRACKET )
                        {
                            parameter.array = true;
                        }
                    }
                    else
                    {
                        error("Received invalid type after token declaration at line %d column %d: %s",
                              token_set.tokens[ j + 2 ].line, token_set.tokens[ j + 2 ].column,
                              token_set.tokens[ j + 2 ].value);
                    }
                    parameters.push_back(parameter);
                }

                // Create a function node with the required information
                auto *function_node = (ast_function_node_t *) malloc(sizeof(ast_function_node_t));

                function_node->function_name = token_set.tokens[ cursor + 1 ].value;
                function_node->parameter_count = parameters.size();
                function_node->parameters = parameters.data();
                function_node->publicly_visible = isInRange(token_set, TOKEN_KEYWORD_PUB, cursor, -2);
                function_node->external = isInRange(token_set, TOKEN_KEYWORD_EXT, cursor, -2);

                auto *returnNode = new ASTNode(RETURN);
                auto *contentNode = new ASTNode(BLOCK);
                auto *fnRoot = new ASTNode(FUNCTION_DEF);
                fnRoot->addBranch(contentNode);
                fnRoot->addBranch(returnNode);
                fnRoot->setValue(function_node);
                root->addBranch(fnRoot);

                cursor += 3;

            }
                break;
            case TOKEN_KEYWORD_VAR:
            {
                // Type name is required after 'var' keyword
                requiresAt(TOKEN_IDENTIFIER, token_set, cursor + 1,
                           "Expected variable name, but received %s",
                           token_set.tokens[ cursor + 1 ].value);

                // Require colon after typeToken name
                requiresAt(TOKEN_COLON, token_set, cursor + 2,
                           "Expected colon after typeToken name",
                           token_set.tokens[ cursor + 2 ].value);

                token_t *typeToken = peak(token_set, cursor, 3);
                // Ensure typeToken is not null and
                if ( !( typeToken != nullptr && isValidType(typeToken->type)))
                {
                    error("Error at line %d column %d: Expected a type after variable declaration name.\n",
                          token_set.tokens[ cursor + 3 ].line, token_set.tokens[ cursor + 3 ].column);
                }

                auto declaration = (ast_declaration_node_t *) malloc(sizeof(ast_declaration_node_t));
                declaration->variable_name = token_set.tokens[ cursor + 1 ].value;
                declaration->variable_type = token_set.tokens[ cursor + 3 ].value;
                declaration->immutable = isPrevious(token_set, TOKEN_KEYWORD_CONST, cursor);

                cursor += 3;

                auto *declNode = new ASTNode(DECLARATION);

                // Find position of the semicolon
                for (
                        j = 0, k = 0, l = token_set.tokens[j].line;
                        j < token_set.token_count; j++ )
                {
                    // If the token is on the next line
                    if ( token_set.tokens[j].line > l)
                    {
                        break;
                    }
                    if ( token_set.tokens[ j ].type == TOKEN_SEMICOLON )
                    {
                        k = 1;
                        break;
                    }
                }
                // If no semicolon found, throw error.
                if ( !k )
                {
                    error("Expected semicolon after variable declaration at line %d column %d\n",
                          token_set.tokens[ cursor ].line, token_set.tokens[ cursor ].column);
                }

                // The first node is the one containing information about the variable
                auto nameNode = new ASTNode(IDENTIFIER);
                nameNode->setValue((void *) declaration);
                declNode->addBranch(nameNode);

                // Check whether there's variable assignment
                if ( isNext(token_set, TOKEN_EQUALS, cursor))
                {
                    // we'll have to parse the value of the expression as a sub-expression,
                    // ... recursion
                    // Now we'll have to call this function with a sub
                    // array from cursor to j
                    auto *subset = (ast_token_set_t *) malloc(sizeof(ast_token_set_t));
                    subset->token_count = j - cursor;
                    subset->tokens = token_set.tokens + cursor;
                    stride::ast::parsePartial(declNode, *subset);
                }
                root->addBranch(declNode);
            }
                break;
                /**
                 * Returns a value from a function.
                 */
            case TOKEN_IDENTIFIER:
            {
                auto *identifierNode = new ASTNode(IDENTIFIER);
                identifierNode->setValue((void *) token_set.tokens[ cursor ].value);
                root->addBranch(identifierNode);
                cursor++;
            }
                break;
                /**
                 * Declares a namespace to group code together.
                 */
            case TOKEN_KEYWORD_NAMESPACE:
            {
                requiresAt(TOKEN_IDENTIFIER, token_set, cursor + 1, "Expected namespace name");

                auto *namespaceNode = new ASTNode(STRUCTURE);
                namespaceNode->setValue((void *) token_set.tokens[ cursor + 1 ].value);
                root->addBranch(namespaceNode);
                cursor += 2;
                // TODO: Add block parsing.
            }
                break;
                /**
                 * Allows one to import external files using 'import "..."' or 'import "..." as keyword'
                 */
            case TOKEN_KEYWORD_IMPORT:
            {
                requiresAt(TOKEN_STRING_LITERAL, token_set, cursor + 1, "Expected string literal for import");

                auto importNode = new ASTNode(INCLUDE_EXTERNAL);

                if ( peak(token_set, cursor, 2)->type == TOKEN_KEYWORD_AS )
                {
                    printf("Importing as\n");
                    requiresAt(TOKEN_IDENTIFIER, token_set, cursor + 3, "Expected identifier after 'as' keyword");
                    importNode->setValue((void *) token_set.tokens[ cursor + 3 ].value);
                    cursor += 2;
                }
                else
                {
                    importNode->setValue((void *) token_set.tokens[ cursor + 1 ].value);
                }
                requiresAt(TOKEN_SEMICOLON, token_set, cursor + 2, "Expected semicolon after import statement");

                root->addBranch(importNode);

                cursor += 2; // Skip import name and colon
            }
                break;
            default:
                cursor++;
                break;
        }
    }
}

void print_type(ASTNode &node)
{
    switch ( node.type )
    {
        case BLOCK:
            printf("BLOCK\n");
            break;
        case FUNCTION_DEF:
            printf("FUNCTION_DEF\n");
            break;
        case DECLARATION:
            printf("DECLARATION\n");
            break;
        case IDENTIFIER:
            printf("IDENTIFIER\n");
            break;
        case RETURN:
            printf("RETURN\n");
            break;
        case STRUCTURE:
            printf("STRUCTURE\n");
            break;
        default:
            printf("UNKNOWN\n");
            break;
    }
}

void recursive_print(ASTNode &node, int depth)
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
ASTNode *stride::ast::parse(ast_token_set_t &token_set)
{
    auto *root = new ASTNode(BLOCK);
    stride::ast::parsePartial(root, token_set);

    recursive_print(*root, 0);

    return root;
}