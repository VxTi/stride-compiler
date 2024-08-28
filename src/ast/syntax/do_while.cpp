//
// Created by Luca Warmenhoven on 27/08/2024.
//

#include "../ast.h"

using namespace stride::ast;

int stride::ast::parse_do_while(stride::ast::ast_token_set_t &token_set, cursor_t index, stride::ast::Node &root)
{
    ast_token_set_t *do_block_tokens = capture_block(token_set, TOKEN_LBRACE, TOKEN_RBRACE, index);
    if ( do_block_tokens == nullptr )
    {
        error("Do statement requires block statement after declaration.\nThis might have gone wrong due to a missing closing brace, at line %d column %d.",
              token_set.tokens[ index ].line, token_set.tokens[ index ].column);
        return 0;
    }

    requires_token(TOKEN_KEYWORD_WHILE, token_set, index + do_block_tokens->token_count + 2,
                   "A do-while statement requires 'while' keyword after body.");

    ast_token_set_t *while_condition_tokens = capture_block(token_set, TOKEN_LPAREN, TOKEN_RPAREN,
                                                            index + do_block_tokens->token_count + 3);

    if ( while_condition_tokens == nullptr || while_condition_tokens->token_count == 0 )
    {
        blame_token(token_set.tokens[ index + do_block_tokens->token_count + 3 ],
                    "While expression requires condition after while keyword.\nThis might have gone wrong due to a missing closing parenthesis");
    }

    requires_token(TOKEN_SEMICOLON, token_set,
                   index + do_block_tokens->token_count + while_condition_tokens->token_count + 5,
                   "Expected semicolon after while condition.");


    // Parse expression in while condition
    auto *do_while_node = new Node(NODE_TYPE_DO_WHILE);
    auto *do_body_node = new Node(NODE_TYPE_BLOCK);
    auto *while_condition_node = new Node(NODE_TYPE_EXPRESSION);

    parse_tokens(do_body_node, *do_block_tokens);
    parse_expression(*while_condition_tokens, 0, while_condition_tokens->token_count, *while_condition_node);

    do_while_node->add_branch(while_condition_node);
    do_while_node->add_branch(do_body_node);

    root.add_branch(do_while_node);

    return do_block_tokens->token_count + while_condition_tokens->token_count + 6;
}