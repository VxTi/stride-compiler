//
// Created by Luca Warmenhoven on 24/08/2024.
//

#include "../ast.h"

using namespace stride::ast;

/**
 * Parses an import declaration.
 * This function will parse an import declaration in the following format:
 * ```
 * import "path/to/file"
 * ```
 * The function has to be called AFTER the import keyword, e.g. the input stream tokens
 * must look like:
 * import < 'string'
 * @param token_set The token set to parse the import declaration from.
 * @param index The index of the token set to start parsing from.
 * @param root The root Node to append the import declaration to.
 */
int stride::ast::parse_import_statement(ast_token_set_t &token_set, cursor_t index, Node &root)
{
    requiresToken(TOKEN_STRING_LITERAL, token_set, index, "Expected string literal for import");

    auto *importNode = new Node(AST_NODE_OP_IMPORT, 0);
    importNode->addBranch(new Node(AST_NODE_OP_IDENTIFIER, 0, token_set.tokens[ index ].value));

    requiresToken(TOKEN_SEMICOLON, token_set, index + 1, "Expected semicolon after import statement");

    root.addBranch(importNode);

    return 2;
}