//
// Created by Luca Warmenhoven on 19/08/2024.
//

#include "ast_parser.h"

ast_node_t gen_node(node_type_t type, size_t branch_count, ast_node_t *branches, void *value)
{
    ast_node_t node;
    node.type = type;
    node.branch_count = branch_count;
    node.branches = branches;
    node.value = value;
    return node;
}

ast_node_t gen_conditional_node(ast_node_t condition, ast_node_t if_true, ast_node_t if_false)
{
    ast_node_t branches[2] = { if_true, if_false };
    return gen_node(CONDITIONAL, 2, branches, &condition);
}

ast_node_t gen_repetition_node(ast_node_t condition, ast_node_t body)
{
    ast_node_t branches[1] = { body };
    return gen_node(REPETITION, 1, branches, &condition);
}

ast_node_t gen_comparison_node(node_type_t type, ast_node_t left, ast_node_t right)
{
    ast_node_t branches[2] = { left, right };
    return gen_node(type, 2, branches, nullptr);
}

/**
 * Checks if there is a next token in the token stream.
 * @param tokens The token stream.
 * @param token_count The number of tokens in the stream.
 * @param index The current index.
 * @return True if there is a next token, false otherwise.
 */
bool has_next_token(token_t *tokens, size_t token_count, size_t index)
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
    return has_next_token(tokens, token_count, current_index) && tokens[ current_index + 1 ].type == type;
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
void exit_if_not_next(token_t *tokens, size_t token_count, size_t current_index, token_type_t type, char * message)
{
    if ( !is_token_next(tokens, token_count, current_index, type)) {
        fprintf(stderr, "Error at line %d column %d -- %s\n", tokens[current_index].line, tokens[current_index].column, message);
        exit(1);
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

    for ( i = 0; i < token_count; ) {
        switch ( tokens[ i ].type ) {

            /* Parses a block of code **/
            case TOKEN_LBRACE: {

                // This will be done in the following steps
                // Iterate through following tokens and see if there's another LBRACE present, if so,
                // call the AST function on that set.

                n = 0;

                // Find index of RBRACE
                for ( j = i, p_end = -1; j < token_count; j++ ) {
                    // Increase block count
                    if ( tokens[ j ].type == TOKEN_LBRACE ) {
                        n++;
                        p_start = j;
                    }
                    else if ( tokens[ j ].type == TOKEN_RBRACE && --n == 0 ) {
                        p_end = j;
                        break;
                    }
                }

                if ( p_end == -1 ) {
                    fprintf(stderr, "Expected closing brace at line %d column %d\n", tokens[ i ].line, tokens[ i ].column);
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
            case TOKEN_KEYWORD_IMPORT: {
                exit_if_not_next(tokens, token_count, i, TOKEN_STRING_LITERAL, "Expected string literal");

                i += 2;
            }
                break;
            case TOKEN_KEYWORD_NAMESPACE: {
                exit_if_not_next(tokens, token_count, i, TOKEN_IDENTIFIER, "Expected namespace name");

                i += 2;
            }
                break;

            case TOKEN_IDENTIFIER: {
                i++;
            }
            default:
                i++;
                break;
        }
    }

    for ( i = 0; i < size; i++ ) {
        if ( nodes[i].type == FUNCTION_DEF ) {
            auto *function_node = (ast_function_node_t *) nodes[i].value;
            printf("Function definition -- %s -- param count: %zu -- public: %s -- external: %s\n",
                   function_node->function_name, function_node->parameter_count,
                   function_node->publicly_visible ? "true" : "false", function_node->external ? "true" : "false");
        }
    }

    *dst_size = size;
    *dst = nodes;
}
