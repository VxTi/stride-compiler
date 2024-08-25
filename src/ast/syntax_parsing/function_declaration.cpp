//
// Created by Luca Warmenhoven on 24/08/2024.
//

#include "../ast.h"

using namespace stride::ast;


/**
 * Parses a function declaration.
 * This function will parse a function declaration in the following format:
 * ```
 * declare (external?) (shared?) name(param1: type1, param2: type2) -> return_type {
 *     // code
 * }
 * ```
 * The function has to be called AFTER the declaration keyword, e.g. the input stream tokens
 * must look like:
 * declare < 'keyword keyword identifier(...)'
 * @param token_set The token set to parse the function declaration from.
 * @param index The index of the token set to start parsing from.
 * @param root The root Node to append the function declaration to.
 */
int stride::ast::parse_function_declaration(ast_token_set_t &token_set, cursor_t index, Node &root)
{
    int flags = 0;
    int start_index = index;

    if ( !has_next(token_set, index))
    {
        error("Expected function name or identifiers, but received none.");
    }
    token_t func_identifier;
    token_t *next = peak(token_set, index, 0);

    // Move the cursor to the right and iterate over the tokens until we find the function name
    // We also disallow double keywords after declaration, and keywords as function names.
    for ( ; next != nullptr && index < token_set.token_count;
            index++, next = peak(token_set, index, 0))
    {
        switch ( next->type )
        {
            case TOKEN_KEYWORD_SHARED:
            {
                if ( flags & FLAG_OBJECT_SHARED)
                {
                    error("Double shared keyword at line %d column %d",
                          next->line, next->column);
                }
                flags |= FLAG_OBJECT_SHARED;
            }
                break;
            case TOKEN_KEYWORD_EXTERNAL:
            {
                if ( flags & FLAG_FUNCTION_EXTERNAL)
                {
                    error("Double external keyword at line %d column %d",
                          next->line, next->column);
                }
                flags |= FLAG_FUNCTION_EXTERNAL;
            }
                break;
            case TOKEN_IDENTIFIER:
            {
                func_identifier = *next;
                requires_token(TOKEN_LPAREN, token_set, ++index, "Expected opening parenthesis");
                goto DECLARATION; // Only way to get out of this mess
            }
            default:
                error("Expected function name or identifiers, but received %s",
                      next->value);
        }
    }

    DECLARATION:
    // Create function Node with the function name as value
    auto *functionNode = new Node(NODE_TYPE_FUNCTION_DEFINITION, flags, func_identifier.value);

    /*
     * Parse function parameters
     * This loop checks for each parameter and whether its types are correctly defined.
     */
    for ( ++index; index < token_set.token_count; )
    {
        // End if token is RPAREN
        if ( token_set.tokens[ index ].type == TOKEN_RPAREN )
        {
            index++;
            break;
        }

        // Create function parameter Node
        auto *paramNode = new Node(NODE_TYPE_FUNCTION_PARAMETERS, 0);
        int flags = 0;

        // If there's a 'const' keyword before the parameter declaration,
        // we'll move the cursor and declare it immutable.
        if ( token_set.tokens[ index ].type == TOKEN_KEYWORD_CONST )
        {
            flags |= FLAG_VARIABLE_IMMUTABLE;
            index++;
        }
        requires_token(TOKEN_IDENTIFIER, token_set, index, "Expected parameter name, but received %s",
                       token_set.tokens[ index ].value);
        requires_token(TOKEN_COLON, token_set, index + 1, "Expected colon after parameter name, but received %s",
                       token_set.tokens[ index ].value);

        // Function parameter Node, resides in a Node 'FUNCTION PARAMETERS'
        paramNode->addBranch(new Node(NODE_TYPE_IDENTIFIER, flags, token_set.tokens[ index ].value));

        // Check if there's a variadic array declared (...)
        if ( peak(token_set, index, 2)->type == TOKEN_THREE_DOTS &&
                is_valid_var_type(peak(token_set, index, 3)->type))
        {
            paramNode->addBranch(
                    new Node(NODE_TYPE_VARIABLE_TYPE,
                             paramNode->flags | FLAG_VARIABLE_ARRAY,
                             token_set.tokens[ index + 3 ].value
                    )
            );
            index += 4;
        } // Check if type is right
        else if ( is_valid_var_type(peak(token_set, index, 2)->type))
        {
            paramNode->addBranch(
                    new Node(NODE_TYPE_VARIABLE_TYPE, 0,
                             token_set.tokens[ index + 2 ].value));

            index += 3;

            if ( peak(token_set, index, 3)->type == TOKEN_LSQUARE_BRACKET &&
                 peak(token_set, index, 4)->type == TOKEN_RSQUARE_BRACKET )
            {
                printf("(array)\n");
                paramNode->flags |= FLAG_VARIABLE_ARRAY;
                index += 2;
            }
        }
        else
        {
            error("Received invalid properties after token declaration at line %d column %d: %s",
                  token_set.tokens[ index + 2 ].line, token_set.tokens[ index + 2 ].column,
                  token_set.tokens[ index + 2 ].value);
        }
    }

    if ( flags & FLAG_FUNCTION_EXTERNAL)
    {
        requires_token(TOKEN_SEMICOLON, token_set, index,
                       "Expected semicolon after external function declaration, but received %s",
                       token_set.tokens[ index ].value);
    }
    else
    {
        requires_token(TOKEN_LBRACE, token_set, index,
                       "Expected opening bracket after function parameters, but received %s",
                       token_set.tokens[ index ].value);
    }
    root.addBranch(functionNode);

    return (int) ( index - start_index + 1 );
}