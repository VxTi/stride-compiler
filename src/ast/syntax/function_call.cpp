//
// Created by Luca Warmenhoven on 26/08/2024.
//

#include "../ast.h"

using namespace stride::ast;

int stride::ast::parse_function_call(ast_token_set_t &token_set, cursor_t index, Node &root)
{
    int offset = 0;

    auto function_call_node = new Node(NODE_TYPE_FUNCTION_CALL);

    offset += parse_identifier(token_set, index, *function_call_node);
    requires_token(TOKEN_LPAREN, token_set, index + offset, "Expected opening parenthesis after function call.");
    ast_token_set_t *function_call_arguments = capture_block(token_set, TOKEN_LPAREN, TOKEN_RPAREN, index + offset);

    if ( function_call_arguments == nullptr )
    {
        blame_token(token_set.tokens[ index + offset ], "Expected function call arguments, but received nothing.");
    }

    offset += function_call_arguments->token_count + 2;

    /*for ( int i = 0, expression_start = 0, expression_length = 0; i < function_call_arguments->token_count; i++ )
    {

    }*/

    root.add_branch(function_call_node);

    if ( peakeq(token_set, index, offset, TOKEN_SEMICOLON) )
    {
        offset += 1;
    }

    return offset + 1;
}

int stride::ast::is_function_call(ast_token_set_t &token_set, cursor_t index)
{
    int offset;
    if ((offset = is_identifier_sequence(token_set, index)) == 0)
    {
        return 0;
    }
    return peakeq(token_set, index + offset, 0, TOKEN_LPAREN);
}