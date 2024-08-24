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
    requiresToken(TOKEN_IDENTIFIER, token_set, index, "Expected namespace name");

    auto *namespaceNode = new Node(AST_NODE_OP_SHARED, 0);

    // Append an identifier as first child of namespace
    namespaceNode->addBranch(
            new Node(AST_NODE_OP_IDENTIFIER, 0, token_set.tokens[ index + 1 ].value)
    );

    // Capture the closure after the namespace
    int closureLength = capture_block(token_set, TOKEN_LBRACE, TOKEN_RBRACE, index, *namespaceNode);

    if ( closureLength == 0 )
    {
        error("Expected block after namespace declaration");
    }

    root.addBranch(namespaceNode);
    return closureLength + 1;
}