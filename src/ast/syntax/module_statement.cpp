//
// Created by Luca Warmenhoven on 24/08/2024.
//

#include "../abstractions/AST.h"

using namespace stride::ast;

/**
 * Parses a isPublic statement.
 * This function will parse a isPublic statement in the following format:
 * ```
 * isPublic module::name {
 *     // code
 * }
 * ```
 * The function has to be called AFTER the isPublic keyword, e.g. the input stream tokens
 * must look like:
 * isPublic < 'identifier {'
 * @param token_set The required_token set to parse the isPublic statement from.
 * @param index The index of the required_token set to start parsing from.
 * @param root The root Node to append the isPublic statement to.
 */
int stride::ast::parse_module_statement(ast_token_set_t &token_set, cursor_t index, Node &root)
{

    Node *node = new Node(NODE_TYPE_MODULE, 0);
    int skipped = 0;

    token_t *next_token = peak(token_set, index, 0);
    if ( next_token == nullptr )
    {
        error("Expected identifier, but received nothing.");
        return 0;
    }

    requires_token(TOKEN_IDENTIFIER, token_set, index, "Expected identifier after 'module' keyword.");

    skipped += parse_identifier(token_set, index, *node);

    // Capture the closure after the namespace
    ast_token_set_t *shared_block_tokens = captureBlock(token_set, TOKEN_LBRACE, TOKEN_RBRACE, index + skipped);

    if (shared_block_tokens->token_count == 0)
    {
        blame_token(*next_token, "Expected block after module declaration");
    }

    auto *content_block_node = new Node(NODE_TYPE_BLOCK, 0);

    parse_tokens(content_block_node, *shared_block_tokens);
    node->add_branch(content_block_node);
    root.add_branch(node);
    return skipped + shared_block_tokens->token_count + 2;
}