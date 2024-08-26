//
// Created by Luca Warmenhoven on 24/08/2024.
//

#include "../ast.h"

using namespace stride::ast;

/**
 * Parses a shared statement.
 * This function will parse a shared statement in the following format:
 * ```
 * shared module::name {
 *     // code
 * }
 * ```
 * The function has to be called AFTER the shared keyword, e.g. the input stream tokens
 * must look like:
 * shared < 'identifier {'
 * @param token_set The token set to parse the shared statement from.
 * @param index The index of the token set to start parsing from.
 * @param root The root Node to append the shared statement to.
 */
int stride::ast::parse_shared_statement(ast_token_set_t &token_set, cursor_t index, Node &root)
{

    Node *node = new Node(NODE_TYPE_SHARED, 0);

    token_t *next_token = peak(token_set, index, 0);
    if ( next_token == nullptr )
    {
        error("Expected identifier, but received nothing.");
        return 0;
    }

    if ( next_token->type == TOKEN_KEYWORD_CLASS )
    {
        node->node_type = NODE_TYPE_CLASS;
        index++;
    }
    else if ( next_token->type == TOKEN_KEYWORD_ENUM )
    {
        return parse_enumerable(token_set, ++index, *node);
    }
    requires_token(TOKEN_IDENTIFIER, token_set, index, "Expected identifier, but received '%s.'",
                   token_set.tokens[ index ].value);


    int skipped = parse_identifier(token_set, index, *node);

    // Capture the closure after the namespace
    int closureLength = parse_block(token_set, TOKEN_LBRACE, TOKEN_RBRACE, index + skipped, *node);

    if ( closureLength == 0 )
    {
        error("Expected block after namespace declaration");
    }

    root.addBranch(node);
    return closureLength + skipped;
}