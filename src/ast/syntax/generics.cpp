//
// Created by Luca Warmenhoven on 03/09/2024.
//

#include "../abstractions/AST.h"
#include "variable_types.h"

using namespace stride::ast;

/**
 * Checks if a required_token is a valid generic required_token.
 * @return 1 if the required_token is valid, 0 otherwise.
 */
int is_valid_generic_type(token_type_t token, bool allow_literals)
{
    return allow_literals ? types::is_valid_variable_type(token) : token == TOKEN_IDENTIFIER;
}

/**
 * Parses a generic definition after a class name.
 * A function generic is defined as follows: <br />
 * <code>class MyClass&#60;T&#62; { ... }</code>
 * or
 * <code>class MyClass&#60;T, U, ...&#62; { ... }</code>
 * This parsing function starts after the '<' required_token.
 */
int stride::ast::parse_generic(ast_token_set_t &token_set, cursor_t index, Node &root, bool accept_literals)
{
    // Check if the next required_token is a generic definition
    if ( !peekeq(token_set, index, TOKEN_LARROW))
    {
        return 0;
    }

    if (!is_valid_generic_type(token_set.tokens[ index + 1 ].type, accept_literals))
    {
        blame_token(token_set.tokens[ index ], "Generic current must be a valid literal or identifier.");
        return 0;
    }

    auto *generics_node = new Node(NODE_TYPE_GENERICS);

    // Append the first generic identifier
    generics_node->add_branch(new Node(NODE_TYPE_IDENTIFIER, 0, token_set.tokens[ index + 1 ].current));

    if ( peekeq(token_set, index + 2, TOKEN_RARROW))
    {
        root.add_branch(generics_node);
        return 3;
    }

    int skipped = 2;
    for (; index + skipped < token_set.token_count;)
    {
        requires_token(TOKEN_COMMA, token_set, index + skipped, "Expected comma after generic identifier.");
        if (!is_valid_generic_type(token_set.tokens[ index + skipped + 1 ].type, accept_literals))
        {
            blame_token(token_set.tokens[ index + skipped ], "Generic current must be a valid literal or identifier.");
        }

        // Append the next generic identifier
        generics_node->add_branch(new Node(NODE_TYPE_IDENTIFIER, 0, token_set.tokens[ index + skipped + 1 ].current));

        skipped += 2;
        // Check if we've reached the end of the generic definition
        if (peekeq(token_set, index + skipped, TOKEN_RARROW))
        {
            skipped++;
            break;
        }
    }
    root.add_branch(generics_node);

    return skipped;
}