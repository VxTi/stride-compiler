//
// Created by Luca Warmenhoven on 26/08/2024.
//

#include "../abstractions/AST.h"

using namespace stride::ast;

int stride::ast::parse_function_call(ast_token_set_t &token_set, cursor_t index, Node &root)
{

    auto function_call_node = new Node(NODE_TYPE_FUNCTION_CALL);

    int offset = parse_identifier(token_set, index, *function_call_node);
    requires_token(TOKEN_LPAREN, token_set, index + offset, "Expected opening parenthesis after function call.");
    ast_token_set_t *function_call_arguments = captureBlock(token_set, TOKEN_LPAREN, TOKEN_RPAREN, index + offset);

    // Ensure it has parenthesis.
    if ( function_call_arguments == nullptr )
    {
        blame_token(token_set.tokens[ index + offset ], "Expected function call arguments, but received nothing.");
    }

    if ( function_call_arguments->token_count > 0 )
    {
        auto *function_call_params_node = new Node(NODE_TYPE_FUNCTION_PARAMETERS);

        for ( int i = 0, expression_length = 1, temp_expression_length;
              i < function_call_arguments->token_count &&
              expression_length > 0; )
        {
            // Ensure the expression length is either the length of the expression or the length of the arguments.
            temp_expression_length = distance_next_token_outside_block(*function_call_arguments, i, TOKEN_COMMA);
            expression_length =
                    temp_expression_length <= 0 ? function_call_arguments->token_count - i : temp_expression_length;

            parse_expression(*function_call_arguments, i, expression_length, *function_call_params_node);

            if ( expression_length < 0 )
            {
                break;
            }
            i += expression_length + 1;
        }
        function_call_node->add_branch(function_call_params_node);
    }
    offset += function_call_arguments->token_count + 2;


    root.add_branch(function_call_node);

    if ( peekeq(token_set, index + offset, TOKEN_SEMICOLON))
    {
        offset += 1;
    }

    return offset + 1;
}

int stride::ast::is_function_call(ast_token_set_t &token_set, cursor_t index)
{
    int offset;
    if (( offset = is_identifier_sequence(token_set, index)) == 0 )
    {
        return 0;
    }
    return peekeq(token_set, index + offset, TOKEN_LPAREN);
}