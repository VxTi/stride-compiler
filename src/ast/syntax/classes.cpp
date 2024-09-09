//
// Created by Luca Warmenhoven on 27/08/2024.
//

#include "../ast.h"

using namespace stride::ast;

/**
 * Parses class inheritance.
 * Class intheritance is in the format of <br />
 * <code>
 * class MyClass : ParentClass, SecondaryClass, Generics<A, B>, ... { ... }
 * </code>  <br />
 * This function must be called after the colon token.
 * @return The number of tokens skipped.
 * @see parse_class
 */
int parse_class_inheritance(ast_token_set_t &token_set, cursor_t index, Node &root)
{
    // First inheritance class
    requires_token(TOKEN_IDENTIFIER, token_set, index, "Class inheritance requires parent class name.");
    auto *inherits_node = new Node(NODE_TYPE_INHERITS);
    auto *first_inherits_node = new Node(NODE_TYPE_CLASS);

    int skipped = 1;
    first_inherits_node->add_branch(
            new Node(NODE_TYPE_IDENTIFIER, 0, token_set.tokens[ index ].value)); // First parent class name

    skipped += parse_generic(token_set, index + skipped, *first_inherits_node, true); // Optional generics
    inherits_node->add_branch(first_inherits_node);


    if ( !peekeq(token_set, index + skipped, TOKEN_KEYWORD_AND))
    {
        root.add_branch(inherits_node);
        return skipped + 1;
    }

    // More inheritance classes
    for ( ; index + skipped < token_set.token_count; )
    {
        requires_token(TOKEN_KEYWORD_AND, token_set, index + skipped, "Expected comma after generic identifier.");
        requires_token(TOKEN_IDENTIFIER, token_set, index + skipped + 1,
                       "Class inheritance requires parent class name.");

        auto *inherits_class_node = new Node(NODE_TYPE_CLASS);
        inherits_class_node->add_branch(
                new Node(NODE_TYPE_IDENTIFIER, 0, token_set.tokens[ index + skipped + 1 ].value));

        skipped += 2;
        skipped += parse_generic(token_set, index + skipped, *inherits_class_node, true); // Optional generics
        inherits_node->add_branch(inherits_class_node);
        if ( peekeq(token_set, index + skipped, TOKEN_LBRACE))
        {
            skipped++;
            break;
        }
    }
    root.add_branch(inherits_node);
    return skipped;
}

/**
 * Parses a class definition.
 * A class definition is defined as follows: <br />
 * <code>class MyClass { ... }</code>
 * This parsing function starts at the 'class' keyword, which is at offset 0.
 */
int stride::ast::parse_class(ast_token_set_t &token_set, cursor_t index, Node &root)
{
    Node *node = new Node(NODE_TYPE_CLASS, 0);

    int cflags = 0;

    if ( peekeq(token_set, index - 1, TOKEN_KEYWORD_PUBLIC))
    {
        cflags |= FLAG_OBJECT_PUBLIC;
    }
    // Ensure class is named
    requires_token(TOKEN_IDENTIFIER, token_set, index + 1, "Expected class name after class definition.");
    node->add_branch(new Node(NODE_TYPE_IDENTIFIER, 0, token_set.tokens[ index + 1 ].value));

    // Parse optional generics.
    int generics_length = parse_generic(token_set, index + 2, *node);

    // Check if class extends other, base : parent1, parent2, ... { ... }
    if ( peekeq(token_set, index + 2 + generics_length, TOKEN_KEYWORD_HAS))
    {
        generics_length += parse_class_inheritance(token_set, index + generics_length + 3, *node);
    }

    // Capture the closure after the namespace
    ast_token_set_t *class_block_tokens = capture_block(token_set, TOKEN_LBRACE, TOKEN_RBRACE,
                                                        index + generics_length + 2);

    if ( class_block_tokens == nullptr )
    {
        blame_token(token_set.tokens[ index + generics_length + 2 ], "Expected block after class declaration");
        return 0;
    }

    node->flags = cflags;
    auto *class_block_node = new Node(NODE_TYPE_BLOCK);

    parse_tokens(class_block_node, *class_block_tokens);

    node->add_branch(class_block_node);
    root.add_branch(node);

    return class_block_tokens->token_count + generics_length + 4;
}