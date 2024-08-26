//
// Created by Luca Warmenhoven on 24/08/2024.
//

#include "../ast.h"

using namespace stride::ast;


/**
 * Parses a function declaration.
 * This function will parse a function declaration in the following format:
 * ```
 * define (external?) (shared?) name(param1: type1, param2: type2) -> return_type {
 *     // code
 * }
 * ```
 * The function has to be called AFTER the declaration keyword, e.g. the input stream tokens
 * must look like:
 * define < 'keyword keyword identifier(...)'
 * @param token_set The token set to parse the function declaration from.
 * @param index The index of the token set to start parsing from.
 * @param root The root Node to append the function declaration to.
 */
int stride::ast::parse_function_declaration(ast_token_set_t &token_set, cursor_t index, Node &root)
{
    int function_flags = 0;
    int skipped = 0;

    if ( !has_next(token_set, index))
    {
        error("Expected function name or identifiers, but received none.");
    }
    token_t func_identifier;
    token_t *next = peak(token_set, index, 0);

    // Move the cursor to the right and iterate over the tokens until we find the function name
    // We also disallow double keywords after declaration, and keywords as function names.
    for ( ; next != nullptr && index < token_set.token_count;
            next = peak(token_set, ++index, 0))
    {
        switch ( next->type )
        {
            case TOKEN_KEYWORD_SHARED:
            {
                if ( function_flags & FLAG_OBJECT_SHARED)
                {
                    error("Double shared keyword at line %d column %d",
                          next->line, next->column);
                }
                function_flags |= FLAG_OBJECT_SHARED;
                skipped++;
            }
                break;
            case TOKEN_KEYWORD_EXTERNAL:
            {
                if ( function_flags & FLAG_FUNCTION_EXTERNAL)
                {
                    error("Double external keyword at line %d column %d",
                          next->line, next->column);
                }
                function_flags |= FLAG_FUNCTION_EXTERNAL;
                skipped++;
            }
                break;
                // This is when the name of the function os stated.
            case TOKEN_IDENTIFIER:
            {
                func_identifier = *next;
                requires_token(TOKEN_LPAREN, token_set, ++index, "Expected opening parenthesis");
                skipped++;
                goto DECLARATION; // Only way to get out of this mess
            }
            default:
                error("Expected function name or identifiers, but received %s",
                      next->value);
                return 0;
        }
    }

    DECLARATION:

    // Create function Node with the function name as value
    auto *function_declaration = new Node(NODE_TYPE_FUNCTION_DEFINITION, function_flags);
    function_declaration->addBranch(new Node(NODE_TYPE_IDENTIFIER, 0, func_identifier.value));

    // Capture block for function parameter body, aka the part after the function name between the parenthesis
    ast_token_set_t *function_parameters_body = capture_block(token_set, TOKEN_LPAREN, TOKEN_RPAREN, index);

    if ( function_parameters_body == nullptr )
    {
        error("Function declaration requires parameter statement, but received none. This is likely caused by a missing closing parenthesis, starting from line %d column %d.",
              token_set.tokens[ index ].line, token_set.tokens[ index ].column);
        return 0;
    }

    // If the function declaration has parameters,
    // add the 'parameters' node
    if ( function_parameters_body->token_count > 0 )
    {
        auto *parameters_node = new Node(NODE_TYPE_FUNCTION_PARAMETERS, 0);

        for ( int i = 0; i < function_parameters_body->token_count; )
        {
            int var_flags = 0;
            if ( peakeq(*function_parameters_body, i, 0, TOKEN_KEYWORD_CONST))
            {
                var_flags |= FLAG_VARIABLE_IMMUTABLE;
                i++;
            }
            // Ensure the variable declaration has
            requires_token(TOKEN_IDENTIFIER, *function_parameters_body, i,
                           "Function definition: Expected parameter name, but received %s",
                           function_parameters_body->tokens[ index ].value);

            requires_token(TOKEN_COLON, *function_parameters_body, i + 1,
                           "Function definition: Expected colon after parameter name, but received %s",
                           function_parameters_body->tokens[ index ].value);

            auto function_parameter = new Node(NODE_TYPE_VARIABLE_DECLARATION);
            function_parameter->addBranch(
                    new Node(NODE_TYPE_IDENTIFIER, 0, function_parameters_body->tokens[ i ].value));

            // Check if parameter is variadic
            if ( peakeq(*function_parameters_body, i, 2, TOKEN_THREE_DOTS))
            {
                i++;
                var_flags |= FLAG_VARIABLE_ARRAY;
            }

            // Validate type after expression
            token_t *type = peak(*function_parameters_body, i, 2);
            if ( type == nullptr || !is_valid_var_type(type->type))
            {
                error("Variadic expression requires valid type, but didn't receive one.");
                return 0;
            }

            // If the parameter has '[]' after it, make it an array (if it does not have '...')
            if ( peakeq(*function_parameters_body, i, 3, TOKEN_LSQUARE_BRACKET)
                 && peakeq(*function_parameters_body, i, 4, TOKEN_RSQUARE_BRACKET))
            {
                if ( var_flags & FLAG_VARIABLE_ARRAY)
                {
                    error("Cannot have variadic expression and array at the same time, at line %d column %d.",
                          function_parameters_body->tokens[i + 2].line,
                          function_parameters_body->tokens[i + 2].column);
                    return 0;
                }
                var_flags |= FLAG_VARIABLE_ARRAY;
                i += 2;
            }

            function_parameter->addBranch(new Node(NODE_TYPE_VARIABLE_TYPE, var_flags, type->value));

            if ( i + 3 < function_parameters_body->token_count && !peakeq(*function_parameters_body, i, 3, TOKEN_COMMA))
            {
                error("Non-last function parameter requires comma after declaration, but received '%s' at line %d column %d.",
                      function_parameters_body->tokens[ i + 3 ].value,
                      function_parameters_body->tokens[ i + 3 ].line,
                      function_parameters_body->tokens[ i + 3 ].column);
                return 0;
            }
            i += 4;
            parameters_node->addBranch(function_parameter);
        }
        function_declaration->addBranch(parameters_node);
    }

    // If the function is not external, it must have a body.
    if (( function_flags & FLAG_FUNCTION_EXTERNAL) == 0 )
    {

        ast_token_set_t *function_body = capture_block(token_set, TOKEN_LBRACE, TOKEN_RBRACE,
                                                       index + function_parameters_body->token_count + 2);

        if ( function_body == nullptr )
        {
            error("Function declaration requires a function body, but received none.\nThis can be caused by a missing closing bracket, starting at line %d column %d.",
                  token_set.tokens[ index ].line, token_set.tokens[ index ].column);
            return 0;
        }

        // Add the parsed function body to the function declaration node.
        auto content_node = new Node(NODE_TYPE_BLOCK);
        parse_tokens(content_node, *function_body);
        function_declaration->addBranch(content_node);
        skipped += function_body->token_count + 2;

    }
    else
    {
        requires_token(TOKEN_SEMICOLON, token_set, index + skipped + function_parameters_body->token_count,
                       "External function declaration must have a semicolon after parameter declaration.");
    }

    root.addBranch(function_declaration);

    return skipped + function_parameters_body->token_count + 2;
}