//
// Created by Luca Warmenhoven on 26/08/2024.
//

#include "../ast.h"

using namespace stride::ast;

int stride::ast::parse_function_call(ast_token_set_t &token_set, cursor_t index, Node &root)
{
    // If the preceding token is not a double colon or opening parenthesis, skip.
    if ( !peakeq(token_set, index, 1, TOKEN_DOUBLE_COLON) && !peakeq(token_set, index, 1, TOKEN_LPAREN))
    {
        return 1;
    }

    auto function_call_node = new Node(NODE_TYPE_FUNCTION_CALL);

}