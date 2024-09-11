//
// Created by Luca Warmenhoven on 26/08/2024.
//

#include "../abstractions/AST.h"

using namespace stride::ast;

int stride::ast::parse_try_catch(stride::ast::ast_token_set_t &token_set, cursor_t index, stride::ast::Node &root)
{
    requires_token(TOKEN_LBRACE, token_set, index, "Try statement requires opening brace after declaration.");

    // Capture the block of the try expression
    ast_token_set_t *try_body_tokens = captureBlock(token_set, TOKEN_LBRACE, TOKEN_RBRACE, index);

    if ( try_body_tokens == nullptr )
    {
        error("Try expression requires block statement after declaration.\nThis might have gone wrong due to a missing closing brace, at line %d column %d.",
              token_set.tokens[ index ].line, token_set.tokens[ index ].column);
        return 0;
    }

    requires_token(TOKEN_KEYWORD_CATCH, token_set, index + try_body_tokens->token_count + 2,
                   "Catch statement requires after try block.");

    // Capture the block of the catch variable
    // This is the variable that will be caught in the catch block
    ast_token_set_t *catch_variable_tokens = captureBlock(token_set, TOKEN_LPAREN, TOKEN_RPAREN,
                                                          index + try_body_tokens->token_count + 3);
    if ( catch_variable_tokens == nullptr )
    {

        error("Catch expression requires variable declaration after catch keyword.\nThis might have gone wrong due to a missing closing parenthesis, at line %d column %d.",
              token_set.tokens[ index + try_body_tokens->token_count + 3 ].line,
              token_set.tokens[ index + try_body_tokens->token_count + 3 ].column);
        return 0;
    }
    validate_variable_declaration(*catch_variable_tokens, 0);

    if ( strcmp((char *) catch_variable_tokens->tokens[ 2 ].current, "Error") != 0 )
    {
        error("\nCatch expression requires 'Error' type after parameter name, at line %d column %d.\nThis is to ensure all exceptions are handled properly.\n",
              catch_variable_tokens->tokens[ 2 ].line,
              catch_variable_tokens->tokens[ 2 ].column);
    }

    // Capture the block of the catch expression
    ast_token_set_t *catch_body_tokens = captureBlock(token_set, TOKEN_LBRACE, TOKEN_RBRACE,
                                                      index + try_body_tokens->token_count +
                                                      catch_variable_tokens->token_count + 5);
    if ( catch_body_tokens == nullptr )
    {
        token_t fault_start = token_set.tokens[ index + try_body_tokens->token_count +
                                                catch_variable_tokens->token_count + 5 ];
        error("Catch expression requires block statement after declaration.\nThis might have gone wrong due to a missing closing brace, at line %d column %d.",
              fault_start.line,
              fault_start.column);
        return 0;
    }

    // All abstractions for the try catch AST entry
    auto *try_catch_node = new Node(NODE_TYPE_TRY_CATCH);
    auto *catch_body_node = new Node(NODE_TYPE_BLOCK);
    auto *try_body_node = new Node(NODE_TYPE_BLOCK);

    auto *catch_expression_node = new Node(NODE_TYPE_VARIABLE_DECLARATION);
    catch_expression_node->add_branch(new Node(NODE_TYPE_IDENTIFIER, 0, catch_variable_tokens->tokens[ 0 ].current));
    catch_expression_node->add_branch(new Node(NODE_TYPE_VARIABLE_TYPE, 0, catch_variable_tokens->tokens[ 2 ].current));

    parse_tokens(try_body_node, *try_body_tokens);
    parse_tokens(catch_body_node, *catch_body_tokens);

    try_catch_node->add_branch(try_body_node);
    try_catch_node->add_branch(catch_expression_node);
    try_catch_node->add_branch(catch_body_node);

    root.add_branch(try_catch_node);
    return try_body_tokens->token_count + catch_variable_tokens->token_count + catch_body_tokens->token_count + 7;
}