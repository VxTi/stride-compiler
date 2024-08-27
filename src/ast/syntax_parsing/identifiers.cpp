//
// Created by Luca Warmenhoven on 24/08/2024.
//

#include "../ast.h"

using namespace stride::ast;

/**
 * Parses a keyword.
 * This function will parse a keyword in the following format:
 * ```
 * keyword::secondary
 * ```
 * @param token_set The token set to parse the keyword from.
 * @param index The index of the token set to start parsing from.
 * @param root The root Node to append the keyword to.
 */
int stride::ast::parse_identifier(ast_token_set_t &token_set, cursor_t index, Node &root)
{
    int skipped = 1;
    token_t *token = peak(token_set, index, 0);

    if ( token == nullptr )
    {
        error("Expected keyword, but received nothing.");
        return 0;
    }

    if ( token->type != TOKEN_IDENTIFIER )
    {
        error("Expected keyword, but received '%s'.", token->value);
        return 0;
    }

    Node *keyword_node;

    // If the next token is a double colon, we have a secondary keyword
    // This is the case when a module has a submodule, e.g. module::submodule
    // We will parse this as a separate identifier.
    if ( peakeq(token_set, index, 1, TOKEN_DOUBLE_COLON))
    {
        keyword_node = new Node(NODE_TYPE_IDENTIFIER_REFERENCE, 0);
        keyword_node->add_branch(new Node(NODE_TYPE_IDENTIFIER, 0, token->value));
        for ( ++index; index < token_set.token_count; index += 2 )
        {
            if ( token_set.tokens[ index ].type != TOKEN_DOUBLE_COLON )
            {
                break;
            }
            requires_token(TOKEN_IDENTIFIER, token_set, index + 1, "Expected identifier, but received '%s.'",
                           token_set.tokens[ index + 1 ].value);

            keyword_node->add_branch(new Node(NODE_TYPE_IDENTIFIER, 0, token_set.tokens[ index + 1 ].value));
            skipped += 2;
        }
    }
    else
    {
        keyword_node = new Node(NODE_TYPE_IDENTIFIER, 0, token->value);
    }
    root.add_branch(keyword_node);
    return skipped;
}

int stride::ast::is_identifier_sequence(stride::ast::ast_token_set_t &token_set, cursor_t index)
{
    if ( !peakeq(token_set, index, 0, TOKEN_IDENTIFIER))
    {
        return 0;
    }

    // If the next token is a double colon, we have a secondary keyword
    // This is the case when a module has a submodule, e.g. module::submodule
    // We will parse this as a separate identifier.
    if ( peakeq(token_set, index, 1, TOKEN_DOUBLE_COLON))
    {
        int length = 1;
        if ( !peakeq(token_set, index, 2, TOKEN_IDENTIFIER))
        {
            return length;
        }

        for ( ++index; index < token_set.token_count; index += 2 )
        {
            // If the next token is not a double colon, we have reached the end of the identifier sequence
            if ( !peakeq(token_set, index, 0, TOKEN_DOUBLE_COLON) ||
                 !peakeq(token_set, index, 1, TOKEN_IDENTIFIER))
            {
                break;
            }
            length += 2;
        }
        return length;
    }
    return 1;
}