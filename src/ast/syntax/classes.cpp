//
// Created by Luca Warmenhoven on 27/08/2024.
//

#include "../ast.h"

using namespace stride::ast;

/**
 * Parses a class definition.
 * A class definition is defined as follows: <br />
 * <code>class MyClass { ... }</code>
 */
int stride::ast::parse_class(ast_token_set_t &token_set, cursor_t index, Node &root)
{
    Node *node = new Node(NODE_TYPE_CLASS, 0);

    requires_token(TOKEN_IDENTIFIER, token_set, index + 1, "Expected class name after class definition.");
    int cflags = 0;

    if ( peekeq(token_set, index - 1, TOKEN_KEYWORD_PUBLIC))
    {
        cflags |= FLAG_OBJECT_PUBLIC;
    }

    int generic_length = parse_generic(token_set, index + 2, *node);

    // Capture the closure after the namespace
    ast_token_set_t *class_block_tokens = capture_block(token_set, TOKEN_LBRACE, TOKEN_RBRACE, index + generic_length + 2);

    if ( class_block_tokens == nullptr  )
    {
        blame_token(token_set.tokens[ index + generic_length + 2 ], "Expected block after class declaration");
        return 0;
    }

    node->flags = cflags;
    auto *class_block_node = new Node(NODE_TYPE_BLOCK);

    parse_tokens(class_block_node, *class_block_tokens);

    node->add_branch(class_block_node);
    root.add_branch(node);

    return class_block_tokens->token_count + generic_length + 4;
}