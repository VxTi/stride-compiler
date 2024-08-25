//
// Created by Luca Warmenhoven on 24/08/2024.
//

#include "../ast.h"

using namespace stride::ast;

/**
 * Parses an expression, which can be after variable declaration / assignment,
 * or providing function parameters.
 * An expression can be in the following format:
 * ```
 * var k: unknown = (5 + (3 * 4))
 * ```
 * Here, the segment after the `unknown = ...` is the expression,
 * which will be parsed using this function.
 * @param token_set The token set to parse a segment from
 * @param cursor The cursor position in the token set
 * @param token_count The amount of tokens in the subset
 * @param parent_node The parent AST Node to put the result into.
 * @return How many tokens were skipped.
 */
int stride::ast::parse_expression(ast_token_set_t &token_set, size_t cursor, size_t token_count, Node &parent_node)
{
    int skipped = 0;

    /*
     * In case the expression is just a singular value,
     * we'll just return that as a Node and return the ast.
     * No need to check for any other cases.
     */
    if ( token_count == 1 &&
         token_set.tokens[ cursor ].type == TOKEN_NUMBER_INTEGER ||
         token_set.tokens[ cursor ].type == TOKEN_STRING_LITERAL ||
         token_set.tokens[ cursor ].type == TOKEN_IDENTIFIER )
    {
        auto *node = new Node(TOKEN_IDENTIFIER, 0);
        node->setValue((void *) token_set.tokens[ cursor ].value);
        parent_node.addBranch(node);
        return 1;
    }

    // TODO: Implement.

    return skipped;
}