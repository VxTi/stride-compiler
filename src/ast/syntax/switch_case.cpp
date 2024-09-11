//
// Created by Luca Warmenhoven on 29/08/2024.
//

#include "../abstractions/AST.h"
#include "variable_types.h"

using namespace stride::ast;

int is_valid_switch_type(token_type_t type)
{
    switch ( type )
    {
        case TOKEN_NUMBER_INTEGER:
        case TOKEN_STRING_LITERAL:
        case TOKEN_BOOLEAN_LITERAL:
            return true;
        default:
            return false;
    }
}

/**
 * Parses the content of a switch case and adds the required abstractions to the root node.
 */
void parse_switch_case_content(ast_token_set_t &token_set, Node &root)
{
    bool has_default = false;
    Node *default_node = nullptr;
    for ( cursor_t i = 0, block_start_offset; i < token_set.token_count; )
    {
        // Ensure either 'case' or 'default' keyword is present.
        if ( !peekeq(token_set, i, TOKEN_KEYWORD_CASE) && !peekeq(token_set, i, TOKEN_KEYWORD_DEFAULT))
        {
            blame_token(token_set.tokens[ i ], "Unexpected required_token in switch statement.");
        }

        // Now create the appropriate switch_case_content_node for the case.
        Node *case_node = new Node(NODE_TYPE_CASE);
        if ( token_set.tokens[ i ].type == TOKEN_KEYWORD_CASE )
        {
            requires_token(TOKEN_COLON, token_set, i + 2, "Expected colon after switch case.");
            if ( !is_valid_switch_type(token_set.tokens[ i + 1 ].type))
            {
                blame_token(token_set.tokens[ i + 1 ], "Invalid switch case expression.");
            }
            block_start_offset = 3;
        }
        else if ( token_set.tokens[ i ].type == TOKEN_KEYWORD_DEFAULT )
        {
            requires_token(TOKEN_COLON, token_set, i + 1, "Expected colon after switch case.");

            // Prevent duplicate defaults
            if ( has_default )
            {
                blame_token(token_set.tokens[ i ], "Switch statement already has a default case.");
            }
            has_default = true;
            case_node->node_type = NODE_TYPE_DEFAULT;
            block_start_offset = 2;
        }
        else
        {
            blame_token(token_set.tokens[ i ], "Unexpected required_token in switch statement.");
        }

        // Parse case content
        int block_length = distance_next_token_outside_block(token_set, i + block_start_offset, TOKEN_KEYWORD_BREAK);
        if ( block_length == -1 )
        {
            block_length = distance_next_token_outside_block(token_set, i + block_start_offset, TOKEN_KEYWORD_RETURN);
            if ( block_length == -1 )
            {
                blame_token(token_set.tokens[ i ], "Expected break or return statement after switch case.");
            }
        }

        auto *switch_case_content_node = new Node(NODE_TYPE_BLOCK);
        ast_token_set_t case_content_tokens = {
                .tokens = token_set.tokens + i + 3,
                .token_count = static_cast<unsigned int>(block_length)
        };
        parse_tokens(switch_case_content_node, case_content_tokens);

        case_node->add_branch(switch_case_content_node);

        if ( case_node->node_type == NODE_TYPE_DEFAULT )
        {
            default_node = case_node;
        }
        else
        {
            root.add_branch(case_node);
        }

        i += block_length + block_start_offset + 2;
    }
    // Default node is always at last.
    if ( default_node != nullptr )
    {
        root.add_branch(default_node);
    }
}

/**
 * Parses a switch case and adds the required abstractions to the root node.
 */
int stride::ast::parse_switch_case(ast_token_set_t &token_set, cursor_t index, Node &root)
{
    ast_token_set_t *switch_expression_tokens = captureBlock(token_set, TOKEN_LPAREN, TOKEN_RPAREN, index);
    if ( switch_expression_tokens == nullptr || switch_expression_tokens->token_count == 0 )
    {
        blame_token(token_set.tokens[ index ], "Switch statement requires expression in parentheses.");
        return 0;
    }

    ast_token_set_t *switch_block_tokens = captureBlock(token_set, TOKEN_LBRACE, TOKEN_RBRACE,
                                                        index + switch_expression_tokens->token_count + 2);

    if ( switch_block_tokens == nullptr )
    {
        blame_token(token_set.tokens[ index + switch_expression_tokens->token_count + 1 ],
                    "Switch statement requires block statement after expression.");
        return 0;
    }

    auto *switch_node = new Node(NODE_TYPE_SWITCH);

    auto *switch_expression_node = new Node(NODE_TYPE_EXPRESSION);

    parse_expression(*switch_expression_tokens, 0, switch_expression_tokens->token_count, *switch_expression_node);

    switch_node->add_branch(switch_expression_node);

    parse_switch_case_content(*switch_block_tokens, *switch_node);

    root.add_branch(switch_node);

    return switch_expression_tokens->token_count + switch_block_tokens->token_count + 4;
}