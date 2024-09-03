//
// Created by Luca Warmenhoven on 03/09/2024.
//

#include "../ast.h"

using namespace stride::ast;


/**
 * Parses a generic definition after a class name.
 * A function generic is defined as follows: <br />
 * <code>class MyClass&#60;T&#62; { ... }</code>
 * or
 * <code>class MyClass&#60;T, U, ...&#62; { ... }</code>
 * This parsing function starts after the '<' token.
 */
int stride::ast::parse_generic(ast_token_set_t &token_set, cursor_t index, Node &root)
{
    // Check if the next token is a generic definition
    if ( !peekeq(token_set, index, TOKEN_LARROW))
    {
        return 0;
    }
    requires_token(TOKEN_IDENTIFIER, token_set, index + 1, "Expected identifier after '<' in generic definition.");
    auto *generics_node = new Node(NODE_TYPE_GENERICS);

    for ( int i = 1; index + i < token_set.token_count; )
    {
        requires_token(TOKEN_IDENTIFIER, token_set, index + i, "Expected identifier in generic definition.");
        auto *generic_identifier_node = new Node(NODE_TYPE_IDENTIFIER);
        generic_identifier_node->value = token_set.tokens[ index + i ].value;
        generics_node->add_branch(generic_identifier_node);

        if ( peekeq(token_set, index + i + 1, TOKEN_COMMA))
        {
            requires_token(TOKEN_IDENTIFIER, token_set, index + i + 2,
                           "Expected identifier after ',' in generic definition.");
            i += 2;
        }
        else if ( peekeq(token_set, index + i + 1, TOKEN_RARROW))
        {
            root.add_branch(generics_node);
            return i + 2;
        }
        else
        {
            delete generics_node;
            blame_token(token_set.tokens[ index + i + 1 ],
                        "Expected ',' or '>' after identifier in generic definition.");
        }
    }
    blame_token(token_set.tokens[ index ], "Expected '>' after generic definition.");
    return 0;
}