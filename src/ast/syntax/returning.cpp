//
// Created by Luca Warmenhoven on 03/09/2024.
//

#include "../abstractions/AST.h"

using namespace stride::ast;

/**
 * Parses a returning statement.
 */
int stride::ast::parse_returning(ast_token_set_t &token_set, cursor_t index, Node &root)
{
    if ( !peekeq(token_set, index, TOKEN_KEYWORD_RETURN))
    {
        return 0;
    }
    auto *return_node = new Node(NODE_TYPE_RETURN);
    root.add_branch(return_node);
    if ( peekeq(token_set, index + 1, TOKEN_SEMICOLON))
    {
        return 2;
    }

    int expression_length = distance_next_token(token_set, index, TOKEN_SEMICOLON);

    parse_expression(token_set, index + 1, expression_length - 1, *return_node);

    return expression_length + 1;
}