//
// Created by Luca Warmenhoven on 24/08/2024.
//

#include "../ast.h"
#include "variable_types.h"

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
    token_t *next = peak(token_set, index, 0);

    // Move the cursor to the right and iterate over the tokens until we find the function name
    // We also disallow double keywords after declaration, and keywords as function names.
    for ( ; next != nullptr && index < token_set.token_count;
            next = peak(token_set, ++index, 0))
    {
        switch ( next->type )
        {
            case TOKEN_KEYWORD_PUBLIC:
            {
                if ( function_flags & FLAG_OBJECT_PUBLIC)
                {
                    error("Double 'public' keyword at line %d column %d",
                          next->line, next->column);
                }
                function_flags |= FLAG_OBJECT_PUBLIC;
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
            case TOKEN_KEYWORD_ASYNC:
            {
                if ( function_flags & FLAG_FUNCTION_ASYNC)
                {
                    error("Double async keyword at line %d column %d",
                          next->line, next->column);
                }
                function_flags |= FLAG_FUNCTION_ASYNC;
                skipped++;
            }
                break;
                // This is when the name of the function os stated.
            case TOKEN_IDENTIFIER:
            {
                skipped++;
                goto DECLARATION; // Only way to get out of this mess
            }
            default:
                error("Illegal identifier '%s' found after function declaration at line %d column %d.\n",
                      next->value,
                      next->line,
                      next->column);
                return 0;
        }
    }

    DECLARATION:

    // Create function Node with the function name as value
    auto *function_declaration = new Node(NODE_TYPE_FUNCTION_DEFINITION, function_flags);
    function_declaration->add_branch(new Node(NODE_TYPE_IDENTIFIER, 0, token_set.tokens[ index ].value));
    requires_token(TOKEN_LPAREN, token_set, index + 1, "Expected opening parenthesis after function name.");
    index++;

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
        auto *function_parameters_node = new Node(NODE_TYPE_FUNCTION_PARAMETERS, 0);

        for ( int i = 0, variadic_declarations = 0; i < function_parameters_body->token_count; )
        {
            int var_flags = 0;
            if ( peekeq(*function_parameters_body, i, TOKEN_KEYWORD_CONST))
            {
                var_flags |= FLAG_VARIABLE_IMMUTABLE;
                i++;
            }
            // Ensure the variable declaration has
            requires_token(TOKEN_IDENTIFIER, *function_parameters_body, i,
                           "Expected parameter name after function declaration.",
                           function_parameters_body->tokens[ index ].value);

            requires_token(TOKEN_COLON, *function_parameters_body, i + 1,
                           "Expected colon after parameter name in function definition.\nThis is required to denote the parameter_type_token of the parameter.",
                           function_parameters_body->tokens[ index ].value);

            auto function_parameter = new Node(NODE_TYPE_VARIABLE_DECLARATION);
            function_parameter->add_branch(
                    new Node(NODE_TYPE_IDENTIFIER, 0, function_parameters_body->tokens[ i ].value));

            // Check if parameter is variadic
            if ( peekeq(*function_parameters_body, i + 2, TOKEN_THREE_DOTS))
            {
                if ( variadic_declarations++ > 0 )
                {
                    blame_token(function_parameters_body->tokens[ i + 2 ],
                                "Found double variadic expression in function declaration.");
                    return 0;
                }
                i++;
                var_flags |= FLAG_VARIABLE_ARRAY;
            }

            // Validate parameter_type_token after expression
            token_t *parameter_type_token = peak(*function_parameters_body, i, 2);
            if ( parameter_type_token == nullptr || !types::is_valid_variable_type(parameter_type_token->type))
            {
                error("Variadic expression requires valid parameter_type_token, but didn't receive one.");
                return 0;
            }

            // If the parameter has '[]' after it, make it an array (if it does not have '...')
            if ( peekeq(*function_parameters_body, i + 3, TOKEN_LSQUARE_BRACKET)
                 && peekeq(*function_parameters_body, i + 4, TOKEN_RSQUARE_BRACKET))
            {
                if ( var_flags & FLAG_VARIABLE_ARRAY)
                {
                    blame_token(function_parameters_body->tokens[ i + 3 ],
                                "Cannot have variadic expression and array at the same time.");
                    return 0;
                }
                var_flags |= FLAG_VARIABLE_ARRAY;
                i += 2;
            }

            auto *parameter_type_node = new Node(NODE_TYPE_VARIABLE_TYPE, var_flags);
            // If the variable type is a reference to a class within a module, use identifier as type.
            if ( is_identifier_sequence(*function_parameters_body, i + 2))
            {
                i += parse_identifier(*function_parameters_body, i + 2, *parameter_type_node) - 1;
            } else {
                parameter_type_node->add_branch(new Node(NODE_TYPE_IDENTIFIER, 0, function_parameters_body->tokens[ i + 2 ].value));
            }
            function_parameter->add_branch(parameter_type_node);

            if ( i + 3 < function_parameters_body->token_count &&
                 !peekeq(*function_parameters_body, i + 3, TOKEN_COMMA))
            {
                blame_token(function_parameters_body->tokens[ i + 3 ],
                            "Function parameter requires comma or closing parenthesis after declaration.");
                return 0;
            }
            i += 4;
            function_parameters_node->add_branch(function_parameter);
        }
        function_declaration->add_branch(function_parameters_node);
    }

    // If the function is not external, it must have a body.
    if (( function_flags & FLAG_FUNCTION_EXTERNAL) == 0 )
    {

        ast_token_set_t *function_body = capture_block(token_set, TOKEN_LBRACE, TOKEN_RBRACE,
                                                       index + function_parameters_body->token_count + 2);

        if ( function_body == nullptr )
        {
            blame_token(token_set.tokens[ index ],
                        "Function declaration requires a function body, but received none.\nThis can be caused by a missing closing bracket, starting at line %d column %d.");
            return 0;
        }

        // Add the parsed function body to the function declaration node.
        auto content_node = new Node(NODE_TYPE_BLOCK);
        parse_tokens(content_node, *function_body);
        function_declaration->add_branch(content_node);
        skipped += function_body->token_count + 2;

    }
    else
    {
        requires_token(TOKEN_SEMICOLON, token_set, index + function_parameters_body->token_count + 2,
                       "External functions are not allowed to have a function body.\nExternal functions must end with a semicolon.");
        skipped++;
    }

    root.add_branch(function_declaration);

    return skipped + function_parameters_body->token_count + 2;
}