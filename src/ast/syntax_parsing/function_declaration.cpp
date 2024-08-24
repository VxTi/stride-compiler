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

    if ( !hasNext(token_set, index))
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
                if ( flags & AST_FLAG_FUNCTION_SHARED)
                {
                    error("Double shared keyword at line %d column %d",
                          next->line, next->column);
                }
                flags |= AST_FLAG_FUNCTION_SHARED;
                printf("Function definition with shared keyword\n");
            }
                break;
            case TOKEN_KEYWORD_EXTERNAL:
            {
                if ( flags & AST_FLAG_FUNCTION_EXTERNAL)
                {
                    error("Double external keyword at line %d column %d",
                          next->line, next->column);
                }
                flags |= AST_FLAG_FUNCTION_EXTERNAL;
                printf("Function definition with external keyword\n");
            }
                break;
            case TOKEN_IDENTIFIER:
            {
                func_identifier = *next;
                requiresToken(TOKEN_LPAREN, token_set, ++index, "Expected opening parenthesis");
                goto DECLARATION; // Only way to get out of this mess
            }
            default:
                error("Expected function name or identifiers, but received %s",
                      next->value);
        }
    }

    DECLARATION:
    // Create function Node with the function name as value
    auto *functionNode = new Node(AST_NODE_OP_FUNCTION_DEFINITION, flags, func_identifier.value);


    // Move forward until we find a closing parenthesis
    for ( ++index; index < token_set.token_count; index++ )
    {
        // End if token is RPAREN
        if ( token_set.tokens[ index ].type == TOKEN_RPAREN )
        {
            break;
        }

        // Create function parameter Node
        auto *paramNode = new Node(AST_NODE_OP_FUNCTION_PARAMETERS, 0);
        int flags = 0;

        // If there's a 'const' keyword before the parameter declaration,
        // we'll move the cursor and declare it immutable.
        if ( token_set.tokens[ index ].type == TOKEN_KEYWORD_CONST )
        {
            flags |= AST_VARIABLE_IMMUTABLE;
            index++;
        }
        validate_variable_declaration(token_set, index);

        // Function parameter Node, resides in a Node 'FUNCTION PARAMETERS'
        paramNode->addBranch(new Node(AST_NODE_OP_IDENTIFIER, flags, token_set.tokens[ index ].value));

        // Check if there's a variadic array declared (...)
        if ( peak(token_set, index, 2)->type == TOKEN_THREE_DOTS &&
             isValidType(peak(token_set, index, 3)->type))
        {
            paramNode->addBranch(
                    new Node(AST_NODE_OP_VARIABLE_TYPE,
                             paramNode->flags | AST_VARIABLE_ARRAY,
                             token_set.tokens[ index + 3 ].value
                    )
            );
        } // Check if type is right
        else if ( isValidType(peak(token_set, index, 2)->type))
        {
            paramNode->addBranch(
                    new Node(AST_NODE_OP_VARIABLE_TYPE, 0,
                             token_set.tokens[ index + 2 ].value));

            if ( peak(token_set, index, 3)->type == TOKEN_LBRACKET &&
                 peak(token_set, index, 4)->type == TOKEN_RBRACKET )
            {
                paramNode->flags |= AST_VARIABLE_ARRAY;
            }
        }
        else
        {
            error("Received invalid properties after token declaration at line %d column %d: %s",
                  token_set.tokens[ index + 2 ].line, token_set.tokens[ index + 2 ].column,
                  token_set.tokens[ index + 2 ].value);
        }
    }

    requiresToken(TOKEN_LBRACKET, token_set, index, "Expected opening bracket after function parameters");

    root.addBranch(functionNode);

    return (int) ( index - start_index + 1 );
}