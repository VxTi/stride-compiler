//
// Created by Luca Warmenhoven on 26/08/2024.
//

#include "../abstractions/AST.h"

using namespace stride::ast;

// TODO: Add inner expression.

/**
 * Parses a for loop and adds the required abstractions to the root node.
 */
int stride::ast::parse_for_loop(ast_token_set_t &token_set, cursor_t index, Node &root)
{

    requires_token(TOKEN_LPAREN, token_set, index,
                   "For loop requires an opening parenthesis after for_loop_expression_tokens.");
    ast_token_set_t *for_loop_expression_tokens = captureBlock(token_set, TOKEN_LPAREN, TOKEN_RPAREN, index);

    if ( for_loop_expression_tokens == nullptr || for_loop_expression_tokens->token_count < 2 )
    {
        blame_token(token_set.tokens[ index + ( for_loop_expression_tokens == nullptr ? 0
                                                                                      : for_loop_expression_tokens->token_count ) ],
                    "For loop for_loop_expression_tokens requires an expression, but received none. This is likely caused by a missing closing parenthesis, or missing tokens.");
        return 0;
    }

    ast_token_set_t *content = captureBlock(token_set, TOKEN_LBRACE, TOKEN_RBRACE,
                                            index + for_loop_expression_tokens->token_count + 2);

    if ( content == nullptr )
    {
        blame_token(token_set.tokens[ index ],
                    "For loop must have content after statement for_loop_expression_tokens, but received none. This is likely caused by a missing closing bracket.");
        return 0;
    }


    auto *for_loop_node = new Node(NODE_TYPE_FOR_LOOP);

    // Parse the content of the expression and append it to the for loop node.
    for ( int i = 0, stage = 0; i < for_loop_expression_tokens->token_count; )
    {
        int len_next_semicolon = distance_next_token(*for_loop_expression_tokens, i, TOKEN_SEMICOLON);
        if ( len_next_semicolon == 0 )
        {
            i++;
            continue;
        }

        if ( len_next_semicolon < 0 )
        {
            if ( stage == 2 )
            {
                break;
            }
            blame_token(for_loop_expression_tokens->tokens[ i ],
                        "For loop expression requires expression separation with semicolons.");
        }

        if ( stage++ == 0 )
        {
            parse_variable_declaration(*for_loop_expression_tokens, i, *for_loop_node);
        }
        else
        {
            parse_expression(*for_loop_expression_tokens, i, len_next_semicolon, *for_loop_node);
        }

        i += len_next_semicolon + 1;
    }

    // If there's no declarations in the for loop,
    // we'll add a truthy conditional.
    if ( for_loop_node->branch_count == 0 )
    {
        auto *truthy_node = new Node(NODE_TYPE_EXPRESSION);
        truthy_node->add_branch(new Node(NODE_TYPE_VALUE, 0, (void *) "true"));
        for_loop_node->add_branch(truthy_node);
    }

    // The content node of the for loop.
    auto *for_loop_content_node = new Node(NODE_TYPE_BLOCK);
    parse_tokens(for_loop_content_node, *content);
    for_loop_node->add_branch(for_loop_content_node);

    root.add_branch(for_loop_node);

    // Now we're sure we have a statement and content for the loop.

    return for_loop_expression_tokens->token_count + content->token_count +
           4; // 4 is the amount of tokens we've skipped. (4 = block boundaries)
}