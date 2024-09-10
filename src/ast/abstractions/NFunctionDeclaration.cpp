//
// Created by Luca Warmenhoven on 24/08/2024.
//

#include "AST.h"
#include "../syntax/variable_types.h"

using namespace stride::ast;


/**
 * Parses a function declaration.
 * This function will parse a function declaration in the following format:
 * ```
 * define (external?) (isPublic?) name(param1: type1, param2: type2) -> returnType {
 *     // code
 * }
 * ```
 * The function has to be called AFTER the declaration keyword, e.g. the input stream tokens
 * must look like:
 * define < 'keyword keyword identifier(...)'
 * @param token_set The required_token set to parse the function declaration from.
 * @param index The index of the required_token set to start parsing from.
 * @param root The root Node to append the function declaration to.
 */

void stride::ast::NFunctionDeclaration::parse(TokenSet &token_set, Node &parent)
{
    int function_flags = 0;
    int skipped = 0;

    if ( !token_set.hasNext())
    {
        error("Expected function name or identifiers, but received none.");
    }

    auto *nst_function = new NFunctionDeclaration();

    for ( ; !token_set.canConsume(TOKEN_IDENTIFIER); )
    {
        if ( token_set.consume(TOKEN_KEYWORD_PUBLIC) )
        {
            if ( nst_function->isPublic )
            {
                blame_token(token_set.current(), "Double 'public' keyword.");
            }
            nst_function->isPublic = true;
        }

        if ( token_set.consume(TOKEN_KEYWORD_EXTERNAL) )
        {
            if ( nst_function->external )
            {
                blame_token(token_set.current(), "Double 'external' keyword.");
            }
            nst_function->external = false;
        }

        if ( token_set.consume(TOKEN_KEYWORD_ASYNC) )
        {
            if ( nst_function->async )
            {
                blame_token(token_set.current(), "Double 'async' keyword.");
            }
            nst_function->async = true;
        }
    }

    nst_function->setFunctionName((char *) token_set.consumeRequired(TOKEN_IDENTIFIER, "Expected function name after function declaration.").value);
    token_set.consumeRequired(TOKEN_LPAREN, "Expected opening parenthesis after function name.");

    // Capture block for function parameter body, aka the part after the function name between the parenthesis
    ast_token_set_t *function_parameters_body = captureBlock(token_set, TOKEN_LPAREN, TOKEN_RPAREN, index);

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
        for ( int i = 0, variadic_declarations = 0; i < function_parameters_body->token_count; )
        {
            auto *nst_parameter = new NVariableDeclaration();
            if ( peekeq(*function_parameters_body, i, TOKEN_KEYWORD_CONST))
            {
                nst_parameter->immutable = true;
                i++;
            }
            // Ensure the variable declaration has
            requires_token(TOKEN_IDENTIFIER, *function_parameters_body, i,
                           "Expected parameter name after function declaration.",
                           function_parameters_body->tokens[ index ].current);

            requires_token(TOKEN_COLON, *function_parameters_body, i + 1,
                           "Expected colon after parameter name in function definition.\nThis is required to denote the parameter_type_token of the parameter.",
                           function_parameters_body->tokens[ index ].current);

            nst_parameter->setVariableName(function_parameters_body->tokens[ i ].current);

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
                nst_parameter->is_array = true;
            }

            // Validate parameter_type_token after expression
            token_t *parameter_type_token = peak(*function_parameters_body, i, 2);
            if ( parameter_type_token == nullptr || !types::is_valid_variable_type(parameter_type_token->type))
            {
                error("Variadic expression requires valid parameter_type_token, but didn't receive one.");
                return 0;
            }

            // If the variable type is a reference to a class within a module, use identifier as type.
            if ( is_identifier_sequence(*function_parameters_body, i + 2))
            {
                auto nst_identifier = parse_identifier()
                i += parse_identifier(*function_parameters_body, i + 2, *parameter_type_node) - 1;
            }
            else
            {
                parameter_type_node->add_branch(
                        new Node(NODE_TYPE_IDENTIFIER, 0, function_parameters_body->tokens[ i + 2 ].current));
            }
            function_parameter->add_branch(parameter_type_node);

            // If the parameter has '[]' after it, make it an array (if it does not have '...')
            if ( peekeq(*function_parameters_body, i + 3, TOKEN_LSQUARE_BRACKET)
                 && peekeq(*function_parameters_body, i + 4, TOKEN_RSQUARE_BRACKET))
            {
                if ( nst_parameter->is_array )
                {
                    blame_token(function_parameters_body->tokens[ i + 3 ],
                                "Cannot have variadic expression and array at the same time.");
                    return 0;
                }

                nst_parameter->is_array = true;
                i += 2;
            }

            if ( i + 3 < function_parameters_body->token_count &&
                 !peekeq(*function_parameters_body, i + 3, TOKEN_COMMA))
            {
                blame_token(function_parameters_body->tokens[ i + 3 ],
                            "Function parameter requires comma or closing parenthesis after declaration.");
                return 0;
            }
            i += 4;
        }
    }

    // If the function is not external, it must have a body.
    if (( function_flags & FLAG_FUNCTION_EXTERNAL) == 0 )
    {

        auto *function_body = captureBlock(token_set, TOKEN_LBRACE, TOKEN_RBRACE,
                                           index + function_parameters_body->token_count + 2);

        if ( function_body == nullptr )
        {
            blame_token(token_set.tokens[ index ],
                        "Function declaration requires a function body, but received none.\nThis can be caused by a missing closing bracket, starting at line %d column %d.");
            return 0;
        }

        // Add the parsed function body to the function declaration node.
        auto nst_block = new NBlock();
        parse_tokens(nst_block, *function_body);
        nst_function->body = nst_block;
        skipped += function_body->token_count + 2;

    }
    else
    {
        requires_token(TOKEN_SEMICOLON, token_set, index + function_parameters_body->token_count + 2,
                       "External functions are not allowed to have a function body.\nExternal functions must end with a semicolon.");
        skipped++;
    }

    root.addChild(nst_function);

    return skipped + function_parameters_body->token_count + 2;
}