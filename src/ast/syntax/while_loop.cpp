//
// Created by Luca Warmenhoven on 26/08/2024.
//

#include "../abstractions/AST.h"
#include "../ASTNodes.h"

using namespace stride::ast;

void stride::ast::NWhileLoop::parse(TokenSet &tokens, Node &parent)
{
    return 0;
}

/**
 * Parses a while loop statement.
 * This must be in the following format:
 * `
 * while (condition) {
 *    // body
 * }
 * @param token_set The required_token set to parse a segment from
 * @param index The cursor position in the required_token set
 * @param root The root AST Node to put the result into.
 * @return How many tokens were skipped.
 */
int stride::ast::parse_while_loop(ast_token_set_t &token_set, cursor_t index, Node &root)
{
    requires_token(TOKEN_LPAREN, token_set, index, "While statement requires opening parenthesis after declaration.");

    // Capture the block of the while expression
    ast_token_set_t *while_expression_tokens = captureBlock(token_set, TOKEN_LPAREN, TOKEN_RPAREN, index);

    if ( while_expression_tokens == nullptr )
    {
        error("While expression requires conditional statement after declaration.\nThis might have gone wrong due to a missing closing parenthesis, at line %d column %d.",
              token_set.tokens[ index ].line, token_set.tokens[ index ].column);
        return 0;
    }

    // Capture the block of the while loop
    ast_token_set_t *while_body_tokens = captureBlock(token_set, TOKEN_LBRACE, TOKEN_RBRACE,
                                                      index + while_expression_tokens->token_count + 2);

    if ( while_body_tokens == nullptr )
    {
        error("While expression requires conditional statement after declaration.\nThis might have gone wrong due to a missing closing parenthesis, at line %d column %d.",
              token_set.tokens[ index + while_expression_tokens->token_count + 2 ].line,
              token_set.tokens[ index + while_expression_tokens->token_count + 2 ].column);
        return 0;
    }

    auto *nst_while = new NWhileLoop();
    auto *nst_while_body = new NBlock();
    auto *nst_while_conditional = new NExpression();

    // All abstractions for the while loop AST entry
    auto *while_node = new Node(NODE_TYPE_WHILE_LOOP);
    auto *while_body_node = new Node(NODE_TYPE_BLOCK);
    auto *while_body_conditional_node = new Node(NODE_TYPE_CONDITIONAL);

    parse_expression(*while_expression_tokens, 0, while_expression_tokens->token_count, *while_body_conditional_node);

    while_node->add_branch(while_body_conditional_node);
    while_node->add_branch(while_body_node);

    parse_tokens(while_body_node, *while_body_tokens);

    root.add_branch(while_node);

    return while_expression_tokens->token_count + while_body_tokens->token_count + 4;
}