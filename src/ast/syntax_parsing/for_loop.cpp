//
// Created by Luca Warmenhoven on 26/08/2024.
//

#include "../ast.h"

using namespace stride::ast;

/**
 * Parses a for loop and adds the required nodes to the root node.
 */
int stride::ast::parse_for_loop(ast_token_set_t &token_set, cursor_t index, Node &root)
{

    requires_token(TOKEN_LPAREN, token_set, index, "For loop requires an opening parenthesis after declaration.");
    ast_token_set_t *declaration = capture_block(token_set, TOKEN_LPAREN, TOKEN_RPAREN, index);

    if ( declaration == nullptr )
    {
        error("For loop declaration must have a statement after, but received none. This is likely caused by a missing closing parenthesis. (starting from line %d, column %d).",
              token_set.tokens[ index ].line, token_set.tokens[ index ].column);
        return 0;
    }

    ast_token_set_t *content = capture_block(token_set, TOKEN_LBRACE, TOKEN_RBRACE,
                                             index + declaration->token_count + 2);

    if ( content == nullptr )
    {
        error("For loop must have content after statement declaration, but received none. This is likely caused by a missing closing bracket. (starting from line %d, column %d).",
              token_set.tokens[ index ].line, token_set.tokens[ index ].column);
        return 0;
    }

    auto *for_loop_node = new Node(NODE_TYPE_FOR_LOOP);

    parse_tokens(for_loop_node, *content);
    root.addBranch(for_loop_node);

    // Now we're sure we have a statement and content for the loop.

    return declaration->token_count + content->token_count +
           4; // 4 is the amount of tokens we've skipped. (4 = block boundaries)
}