//
// Created by Luca Warmenhoven on 19/08/2024.
//

#include "ast_parser.h"

ast_node_t gen_node(ast_node_type_t type, size_t branch_count, ast_node_t *branches, void *value)
{
    ast_node_t node;
    node.type = type;
    node.branch_count = branch_count;
    node.branches = branches;
    node.value = value;
    return node;
}

/**
 * Checks if there is a next token in the token stream.
 * @param tokens The token stream.
 * @param token_count The number of tokens in the stream.
 * @param index The current index.
 * @return True if there is a next token, false otherwise.
 */
bool has_next_token(size_t token_count, size_t index)
{
    return index + 1 < token_count;
}

/**
 * Checks if the next token in the token stream is of a certain type.
 * @param tokens The token stream.
 * @param token_count The number of tokens in the stream.
 * @param current_index The current index.
 * @param type The type to check for.
 * @return True if the next token is of the specified type, false otherwise.
 */
bool is_token_next(token_t *tokens, size_t token_count, size_t current_index, token_type_t type)
{
    return has_next_token(token_count, current_index) && tokens[ current_index + 1 ].type == type;
}

/**
 * Checks if the previous token in the token stream is of a certain type.
 * @param tokens The token stream.
 * @param token_count The number of tokens in the stream.
 * @param current_index The current index.
 * @param type The type to check for.
 * @return True if the previous token is of the specified type, false otherwise.
 */
bool is_token_before(token_t *tokens, size_t current_index, token_type_t type)
{
    return current_index > 0 && tokens[ current_index - 1 ].type == type;
}

/**
 * Exits the program if the next token is not of a certain type.
 * @param tokens The token stream.
 * @param token_count The number of tokens in the stream.
 * @param current_index The current index.
 * @param type The type to check for.
 */
void exit_if_not_next(token_t *tokens, size_t token_count, size_t current_index, token_type_t type, char *message)
{
    if ( !is_token_next(tokens, token_count, current_index, type)) {
        fprintf(stderr, "Error at line %d column %d -- %s\n", tokens[ current_index ].line,
                tokens[ current_index ].column, message);
        exit(1);
    }
}

/**
 * Checks whether the token at the given position is a valid 'type' token,
 * e.g. a primitive or a keyword.
 * @param tokens The tokens to check
 * @param index The index to check
 * @return Whether the token is valid.
 */
bool is_token_valid_type(token_t *tokens, size_t token_count, size_t index)
{
    if ( index >= token_count ) {
        return false;
    }

    switch ( tokens[ index ].type ) {
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
 * Grows the branches of a node to a new size.
 * @param node The node to grow.
 * @param new_size The new size.
 */
void ast_grow_branches(ast_node_t *node, size_t new_size)
{
    auto *new_branches = (ast_node_t *) malloc(sizeof(ast_node_t) * new_size);
    memcpy(new_branches, node->branches, sizeof(ast_node_t) * node->branch_count);
    free(node->branches);
    node->branches = new_branches;
    node->branch_count = new_size;
}

void print_recursively(ast_node_t *node, int depth)
{
    if ( depth > 0 ) {
        printf("|");
        for ( int i = 1; i < depth; i++ )
            printf("_");
    }
    ast_node_t branch;
    for ( int i = 0; i < node->branch_count; i++ ) {
        branch = node->branches[ i ];
        switch ( branch.type ) {
            case FUNCTION_DEF: {
                auto *function_node = (ast_function_node_t *) branch.value;
                printf("Function definition -- %s -- param count: %zu -- public: %s -- external: %s\n",
                       function_node->function_name, function_node->parameter_count,
                       function_node->publicly_visible ? "true" : "false", function_node->external ? "true" : "false");
                break;
            }
            case INCLUDE_EXTERNAL: {
                break;
            }
            case STRUCTURE: {
                printf("Structure definition -- %s\n\n", (char *) branch.value);
                break;
            }
            case DECLARATION: {
                auto decl = (ast_declaration_node_t *) branch.value;
                printf("Variable Declaration -- %s -- type: %s -- immutable: %s\n\n",
                       decl->variable_name,
                       decl->variable_type,
                       decl->immutable ? "true" : "false");
                break;
            }
        }
    }
}

/**
 * Parses a block of code into an abstract syntax tree.
 * This function will recursively parse the block and its children.
 * @param tokens The token stream.
 * @param token_count The number of tokens in the stream.
 * @param index The current index.
 * @return The parsed block node.
 */
void ast_parse(token_t *tokens, size_t token_count, ast_node_t **dst, size_t *dst_size)
{
    int i, j, p_end, p_start, n;
    size_t size = 0;
    auto *nodes = (ast_node_t *) malloc(sizeof(ast_node_t) * token_count);

    ast_node_t root_content = gen_node(RETURN, 1, nullptr, nullptr);
    ast_node_t root = gen_node(BLOCK, 1, &root_content, nullptr);

    auto block_stack = (ast_node_t *) malloc(sizeof(ast_node_t) * token_count);
    int block_stack_size = 0;
    int block_stack_cursor = -1;

    for ( i = 0; i < token_count; ) {
        switch ( tokens[ i ].type ) {

            /* Parses a block of code **/
            case TOKEN_LBRACE: {

                // This will be done in the following steps
                // Iterate through following tokens and see if there's another LBRACE present, if so,
                // call the AST function on that set.

                ast_grow_branches(block_stack, ++block_stack_size);
                block_stack[ ++block_stack_cursor ] = gen_node(BLOCK, 0, nullptr, nullptr);

                // Find index of RBRACE
                for (
                        j = i,
                        p_end = -1,
                        n = 0;
                        j < token_count; j++ ) {
                    // Increase block count
                    if ( tokens[ j ].type == TOKEN_LBRACE ) {
                        n++;
                        p_start = j;
                        block_stack[ block_stack_cursor ].branch_count++;
                    }
                    else if ( tokens[ j ].type == TOKEN_RBRACE && --n == 0 ) {
                        p_end = j;
                        break;
                    }
                }

                if ( p_end == -1 ) {
                    fprintf(stderr, "Expected closing brace at line %d column %d\n", tokens[ i ].line,
                            tokens[ i ].column);
                    exit(1);
                }

                i++;

            }
                break;
                /** Function declaration **/
            case TOKEN_KEYWORD_FN: {
                exit_if_not_next(tokens, token_count, i, TOKEN_IDENTIFIER, "Expected function name");

                // Create a function node with the required information
                auto *branches = (ast_node_t *) malloc(sizeof(ast_node_t) * 2);
                auto *function_node = (ast_function_node_t *) malloc(sizeof(ast_function_node_t));

                function_node->function_name = tokens[ i + 1 ].value;
                function_node->parameter_count = 0;
                function_node->publicly_visible = ( i > 0 && tokens[ i - 1 ].type == TOKEN_KEYWORD_PUB ) ||
                                                  ( i > 1 && tokens[ i - 2 ].type == TOKEN_KEYWORD_PUB );

                function_node->external = ( i > 0 && tokens[ i - 1 ].type == TOKEN_KEYWORD_EXT ) ||
                                          ( i > 1 && tokens[ i - 2 ].type == TOKEN_KEYWORD_EXT );


                branches[ 0 ] = gen_node(BLOCK, 0, nullptr, nullptr);
                branches[ 1 ] = gen_node(RETURN, 1, nullptr, nullptr);

                ast_node_t root = gen_node(FUNCTION_DEF, 0, branches, (void *) function_node);

                nodes[ size++ ] = root;

                i += 2;
            }
                break;
            case TOKEN_KEYWORD_CLASS: {
                exit_if_not_next(tokens, token_count, i, TOKEN_IDENTIFIER, "Expected class name");

                // Parse class definition
                nodes[ size++ ] = gen_node(STRUCTURE, 0, nullptr, (void *) tokens[ i + 1 ].value);

                i += 2;
            }
                break;
            case TOKEN_KEYWORD_VAR: {
                exit_if_not_next(tokens, token_count, i, TOKEN_IDENTIFIER, "Expected variable name");

                auto declaration = (ast_declaration_node_t *) malloc(sizeof(ast_declaration_node_t));
                declaration->variable_name = tokens[ i + 1 ].value;
                declaration->immutable = is_token_before(tokens, i, TOKEN_KEYWORD_CONST);

                // Acquire variable type
                // We require a colon and a type after variable declaration.
                if ( is_token_next(tokens, token_count, i + 1, TOKEN_COLON)
                     && is_token_valid_type(tokens, token_count, i + 3)) {
                    declaration->variable_type = tokens[ i + 3 ].value;
                    i += 2;
                }
                else {
                    // Exit the parsing, the syntax is clearly wrong.
                    fprintf(stderr,
                            "\nError at line %d column %d -- Expected type name for variable declaration\n",
                            tokens[ i + 3 ].line, tokens[ i + 3 ].column);
                    exit(1);
                }

                // Everything good so far, now let's check whether there's an assignment

                if ( is_token_next(tokens, token_count, i, TOKEN_EQUALS)) {

                }

                ast_node_t root = gen_node(DECLARATION, 0, nullptr, (void *) declaration);
                nodes[ size++ ] = root;

            }
                break;
            case TOKEN_KEYWORD_IMPORT: {
                exit_if_not_next(tokens, token_count, i, TOKEN_STRING_LITERAL, "Expected string literal");

                nodes[ size++ ] = gen_node(INCLUDE_EXTERNAL, 0, nullptr, (void *) tokens[ i + 1 ].value);

                i += 2;
            }
                break;
            case TOKEN_KEYWORD_NAMESPACE: {
                exit_if_not_next(tokens, token_count, i, TOKEN_IDENTIFIER, "Expected namespace name");

                nodes[ size++ ] = gen_node(STRUCTURE, 0, nullptr, (void *) tokens[ i + 1 ].value);

                i += 2;
            }
                break;
            default:
                i++;
                break;
        }
    }

    print_recursively(&root, 0);

    *dst_size = size;
    *dst = nodes;
}

/*
 * Constructor
 */
AST::AST(token_t **tokens, size_t token_count)
{
    this->tokens = tokens;
    this->token_count = token_count;
}


ASTNode* AST::createComparison(ASTNode *left, ASTNode *right, token_type_t comparison)
{
    switch ( comparison ) {
        case TOKEN_LESS:
        case TOKEN_GREATER:
        case TOKEN_LEQUALS:
        case TOKEN_GEQUALS:
        case TOKEN_DOUBLE_EQUALS:
        case TOKEN_NOT_EQUALS: {
            auto branches = (ASTNode *) malloc(sizeof(ASTNode) * 2);
            branches[ 0 ] = *left;
            branches[ 1 ] = *right;
            return new ASTNode((ast_node_type_t) comparison, 2, branches, nullptr);
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
bool AST::hasNext() {
    return this->current_index < this->token_count;
}

/**
 * Whether there is a token before the current one
 * @return
 */
bool AST::hasPrevious() {
    return this->current_index - 1 >= 0;
}

/**
 * Returns the next token in the token stream, from
 * the current index.
 * @return
 */
token_t* AST::next() {
    if ( !this->hasNext() )
        return nullptr;

    return this->tokens[this->current_index + 1];
}

/**
 * Returns the previous token in the token stream, from the
 * current index.
 * @return
 */
token_t *AST::previous() {
    if (!this->hasPrevious())
        return nullptr;

    return this->previous();
}

/**
 * Returns the token at the nth offset from the cursor position,
 * e.g. cursor = 0, peak(1) will return the token at position 1. (cursor + n)
 * @param offset
 * @return
 */
token_t *AST::peak(int offset) {
    int index = offset + this->current_index;
    if ( index < 0 || index >= this->token_count )
        return nullptr;

    return this->tokens[index];
}

/**
 * Whether the next token is of the provided type.
 * If there are no tokens left, the function will return false.
 * @param type The type to check for
 * @return
 */
bool AST::isNext(token_type_t type) {
    if ( !hasNext() )
        return false;

    return this->next()->type == type;
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
bool AST::isNextInRange(token_type_t type, int range ) {
    if ( range == 0 )
        return this->tokens[this->token_count]->type == type;

    size_t start = this->current_index;
    size_t end = this->current_index + range;

    // Swap range if range is negative
    if ( start > end ) {
        size_t tmp = start;
        start = end;
        end = tmp;
    }

    for ( size_t i = start; i < end; i++ ) {
        if ( this->tokens[i]->type == type )
            return true;
    }
    return false;
}

/**
 * Whether the provided token type is an acceptable
 * variable type. This is only true for primitives and identifiers.
 * @param type The type to check.
 * @return
 */
bool AST::isValidType(token_type_t type)
{
    switch ( type ) {
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
 * Parses the token stream into an abstract syntax tree.
 * @return An AST Node.
 */
ASTNode AST::parse() {
    auto *rootNode = new ASTNode(BLOCK, 1, nullptr, nullptr);

    return *rootNode;
}