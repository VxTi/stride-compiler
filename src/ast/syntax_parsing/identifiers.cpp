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
    }
    else if ( token->type != TOKEN_IDENTIFIER )
    {
        error("Expected keyword, but received '%s'.", token->value);
    }

    Node *keywordNode;

    token_t *next;
    if (( next = peak(token_set, index, 1)) != nullptr && next->type == TOKEN_DOUBLE_COLON )
    {
        bool accepts_next = true;
        keywordNode = new Node(NODE_TYPE_IDENTIFIER_REFERENCE, 0);

        for ( ++index; index < token_set.token_count && accepts_next; index += 2, skipped += 2 )
        {
            if ( token_set.tokens[ index ].type != TOKEN_DOUBLE_COLON )
            {
                accepts_next = false;
            }
            else if ( token_set.tokens[ index ].type == TOKEN_IDENTIFIER )
            {
                keywordNode->addBranch(new Node(NODE_TYPE_IDENTIFIER, 0, token_set.tokens[ index ].value));
            }
            else
            {
                break;
            }
        }
    }
    else
    {
        keywordNode = new Node(NODE_TYPE_IDENTIFIER, 0, token->value);
    }
    root.addBranch(keywordNode);
    return skipped;
}