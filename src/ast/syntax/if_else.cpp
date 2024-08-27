//
// Created by Luca Warmenhoven on 27/08/2024.
//

#include "../ast.h"

using namespace stride::ast;

int stride::ast::parse_if_else(ast_token_set_t &token_set, cursor_t index, Node &parent_node)
{
    int skipped = 0;

    // Capture the if condition block
    auto *if_condition_tokens = capture_block(token_set, TOKEN_LPAREN, TOKEN_RPAREN, index);

    if ( if_condition_tokens == nullptr )
    {
        error("If statement requires condition block after declaration.\nThis might have gone wrong due to a missing closing parenthesis, at line %d column %d.",
              token_set.tokens[ index ].line, token_set.tokens[ index ].column);
        return 0;
    }
    skipped += if_condition_tokens->token_count + 2;

    auto *if_node = new Node(NODE_TYPE_IF);
    parse_expression(*if_condition_tokens, 0, if_condition_tokens->token_count, *if_node);

    // Capture the if body block
    auto *if_body_tokens = capture_block(token_set, TOKEN_LBRACE, TOKEN_RBRACE,
                                         index + if_condition_tokens->token_count + 2);

    if ( if_body_tokens == nullptr )
    {
        error("If expression requires block statement after declaration.\nThis might have gone wrong due to a missing closing brace, at line %d column %d.",
              token_set.tokens[ index + if_condition_tokens->token_count + 2 ].line,
              token_set.tokens[ index + if_condition_tokens->token_count + 2 ].column);
        return 0;
    }

    skipped += if_body_tokens->token_count + 2;

    auto *if_body_node = new Node(NODE_TYPE_BLOCK);
    parse_tokens(if_body_node, *if_body_tokens);

    if_node->add_branch(if_body_node);

    // If there's an 'else' statement, check all possibilities
    if ( peakeq(token_set, index + skipped, 0, TOKEN_KEYWORD_ELSE))
    {
        if ( peakeq(token_set, index + skipped, 1, TOKEN_KEYWORD_IF)) // If there's an 'else if' statement, parse it
        {
            skipped += parse_if_else(token_set, index + skipped + 2, *if_node);
        }
        else if ( peakeq(token_set, index + skipped, 1,
                         TOKEN_LBRACE)) // If there's an 'else' statement, parse the block contents
        {
            auto *else_body_tokens = capture_block(token_set, TOKEN_LBRACE, TOKEN_RBRACE, index + skipped + 1);

            if ( else_body_tokens == nullptr )
            {
                token_t faulty = token_set.tokens[ index + if_condition_tokens->token_count +
                                                   if_body_tokens->token_count + 4 ];
                error("Else expression requires block statement after declaration.\nThis might have gone wrong due to a missing closing brace, at line %d column %d.",
                      faulty.line,
                      faulty.column);
                return 0;
            }

            auto *else_body_node = new Node(NODE_TYPE_BLOCK);
            parse_tokens(else_body_node, *else_body_tokens);
            if_node->add_branch(else_body_node);
            skipped += else_body_tokens->token_count + 2;
        }
        else
        {
            int semicolon_index = -1;
            // Peak until semicolon and parse
            for ( int i = skipped + 1; i < token_set.token_count; i++ )
            {
                if ( peakeq(token_set, i, 0, TOKEN_SEMICOLON))
                {
                    semicolon_index = i;
                    break;
                }
            }
            if ( semicolon_index == -1 )
            {
                error("A semicolon is required at the end of an 'else' statement, at line %d column %d.",
                      token_set.tokens[ index + skipped + 1 ].line,
                      token_set.tokens[ index + skipped + 1 ].column);
                return 0;
            }
            ast_token_set_t segment = {
                    .tokens = token_set.tokens + skipped + 1,
                    .token_count = static_cast<size_t>(semicolon_index - skipped - 1)
            };
            parse_tokens(if_body_node, segment);
        }
    }

    parent_node.add_branch(if_node);

    return skipped;
}