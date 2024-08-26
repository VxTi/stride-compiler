//
// Created by Luca Warmenhoven on 24/08/2024.
//

#include "../ast.h"

using namespace stride::ast;


/**
 * Captures a closure with specified token boundaries.
 * If the boundaries are not found or there's an imbalance of boundaries,
 * an error will be thrown.
 * A closure can be captured using the following example:
 * ```
 * // token_set tokens: LBRACE, OP1, OP2, OP3, ... RBRACE
 * captureBlock(token_set, LBRACE, RBRACE, parent)
 * // Appends the capture 'OP1, OP2, OP3, ...' to parent Node and returns the
 * // skipped amount of tokens.
 * ```
 * @param token_set The token set to capture the closure from
 * @param start_token The starting token from the closure
 * @param end_token The ending token for the closure
 * @param parent_node The parent Node to append the closure to.
 * @return
 */
int stride::ast::parse_block(ast_token_set_t &token_set, token_type_t start_token, token_type_t end_token,
                             int starting_index,
                             Node &parent_node)
{
    requires_token(start_token, token_set, starting_index, "Expected opening token with id %d, but received %s.", start_token, token_set.tokens[ starting_index ].value);

    int index, skipped_tokens, branch_depth;
    auto *closureNode = new Node(NODE_TYPE_BLOCK, 0);

    for (
            index = starting_index, skipped_tokens = 0, branch_depth = 0;
            index < token_set.token_count;
            index++, skipped_tokens++
            )
    {
        if ( token_set.tokens[ index ].type == start_token )
        {
            branch_depth++;
        } // If the ending of the closure was found with balancing tokens, we can safely end the loop.
        else if ( token_set.tokens[ index ].type == end_token && --branch_depth == 0 )
        {
            break;
        }
        // Make sure there's a balance...
        if ( branch_depth < 0 )
        {
            error("Imbalanced closure at line %d column %d",
                  token_set.tokens[ index ].line, token_set.tokens[ index ].column);
        }
    }

    // If there aren't any tokens skipped, there's no closure to append.
    if ( skipped_tokens > 0 )
    {
        ast_token_set_t subset;
        subset.tokens = token_set.tokens + starting_index;
        subset.token_count = skipped_tokens;

        // Now we'll have to parse the content of the closure and append that to the closure Node.
        // This allows us to add code branching, otherwise the parsing will end immediately after
        // the first closure.
        parse_tokens(closureNode, subset);
        parent_node.add_branch(closureNode);
    }

    return skipped_tokens;
}

ast_token_set_t *
stride::ast::capture_block(ast_token_set_t &token_set, token_type_t start_token, token_type_t end_token,
                           int starting_index)
{
    requires_token(start_token, token_set, starting_index, "Expected opening token with id %d, but received %s.", start_token, token_set.tokens[ starting_index ].value);
    int index, skipped_tokens, branch_depth;
    int block_start_index = starting_index;

    for (
            index = starting_index, skipped_tokens = 0, branch_depth = 0;
            index < token_set.token_count;
            index++, skipped_tokens++
            )
    {
        if ( token_set.tokens[ index ].type == start_token )
        {
            if ( branch_depth++ == 0 )
            {
                block_start_index = index + 1;
            }
        } // If the ending of the closure was found with balancing tokens, we can safely end the loop.
        else if ( token_set.tokens[ index ].type == end_token && --branch_depth == 0 )
        {
            break;
        }
        // Make sure there's a balance...
        if ( branch_depth < 0 )
        {
            error("Imbalanced closure at line %d column %d",
                  token_set.tokens[ index ].line, token_set.tokens[ index ].column);
        }
    }

    // If there aren't any tokens skipped, there's no closure to append.
    if ( skipped_tokens > 0 )
    {
        auto *subset = (ast_token_set_t *) malloc(sizeof(ast_token_set_t));
        subset->tokens = token_set.tokens + block_start_index;
        subset->token_count = skipped_tokens - 1;
        return subset;
    }

    return nullptr;
}