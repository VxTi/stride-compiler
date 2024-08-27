//
// Created by Luca Warmenhoven on 27/08/2024.
//

#include "../ast.h"

using namespace stride::ast;

int stride::ast::parse_class(ast_token_set_t &token_set, cursor_t index, Node &root)
{
    Node *node = new Node(NODE_TYPE_CLASS, 0);

    requires_token(TOKEN_IDENTIFIER, token_set, index, "Expected identifier after 'class' keyword.");
    int cflags = 0;

    if ( peakeq(token_set, index, -2, TOKEN_KEYWORD_SHARED))
    {
        cflags |= FLAG_OBJECT_SHARED;
    }

    // Capture the closure after the namespace
    ast_token_set_t *class_block_tokens = capture_block(token_set, TOKEN_LBRACE, TOKEN_RBRACE, index);

    if ( class_block_tokens->token_count == 0 )
    {
        error("Expected block after class declaration");
    }

    node->flags = cflags;

    parse_tokens(node, *class_block_tokens);

    root.add_branch(node);

    return class_block_tokens->token_count + 2;
}