//
// Created by Luca Warmenhoven on 24/08/2024.
//

#include "../ast.h"
#include "variable_types.h"

using namespace stride::ast;

/**
 * Parses a variable declaration expression.
 * This must be in one of the following formats:
 * ```
 * var name: type = value;
 * var name1: type1 = value1, name2: type2 = value2;
 * var name1: type1, name2: type2;
 * ```
 * This function starts parsing after the 'var' keyword, in the parse_tokens function.
 * @param token_set The token set to parse the variable declaration from.
 * @param index  The index of the token set to start parsing from.
 */
int stride::ast::parse_variable_declaration(ast_token_set_t &token_set, cursor_t index, Node &root)
{
    // We start from the keyword of the declaration, e.g. after 'let'

    if ( !peakeq(token_set, index, 0, TOKEN_KEYWORD_LET) &&
         !peakeq(token_set, index, 0, TOKEN_KEYWORD_CONST))
    {
        blame_token(token_set.tokens[ index ], "Variable declaration requires either 'const' or 'let' as initializer.");
    }

    int var_flags = peakeq(token_set, index, 0, TOKEN_KEYWORD_CONST) ? FLAG_VARIABLE_IMMUTABLE : 0;
    int statement_length = distance_next_token(token_set, index + 1, TOKEN_SEMICOLON);

    if ( statement_length == -1 )
    {
        blame_token(token_set.tokens[ index ], "Expected semicolon after variable declaration, but received none");
        return 0;
    }

    // Calculate the length of the secondary expression.
    int sub_expression_length = distance_next_token_outside_block(token_set, index + 1, TOKEN_COMMA);

    /*
     * Regular variable declaration (singular expression)
     * This is the parsing for when there's only one variable declared on the same line.
     */
    if ( sub_expression_length < 0 || sub_expression_length >= statement_length )
    {
        auto *variable_declaration_node = new Node(NODE_TYPE_VARIABLE_DECLARATION, 0);
        validate_variable_declaration(token_set, index + 1);
        variable_declaration_node->add_branch(
                new Node(NODE_TYPE_IDENTIFIER, 0,
                         token_set.tokens[ index + 1 ].value)
        );
        variable_declaration_node->add_branch(
                new Node(NODE_TYPE_VARIABLE_TYPE, 0,
                         token_set.tokens[ index + 3 ].value)
        );
        variable_declaration_node->flags = var_flags;

        // If there's an equals sign, parse the expression after it.
        if ( peakeq(token_set, index, 4, TOKEN_EQUALS))
        {
            int length = statement_length - 4;
            if ( length < 1 )
            {
                length = 1;
            }
            parse_expression(token_set, index + 5, length, *variable_declaration_node);
        }
        else if ( !peakeq(token_set, index, 4, TOKEN_COMMA) && !peakeq(token_set, index, 4, TOKEN_SEMICOLON))
        {
            blame_token(token_set.tokens[ index + 4 ],
                        "Expected equals sign, semicolon or comma after variable declaration.");
        }
        root.add_branch(variable_declaration_node);
        return statement_length + 1;
    }
    /*
     * Multiple variable declaration (multiple expressions)
     * This is the parsing for when there's multiple variables declared on the same line,
     * such as: `var name1: type1, name2: type2;`
     */
    for ( int i = index + 1;
          i < index + statement_length + 1;
          sub_expression_length = distance_next_token_outside_block(token_set, i, TOKEN_COMMA))
    {
        validate_variable_declaration(token_set, i);
        auto *variable_declaration_node = new Node(NODE_TYPE_VARIABLE_DECLARATION, 0);
        variable_declaration_node->flags = var_flags;

        variable_declaration_node->add_branch(
                new Node(NODE_TYPE_IDENTIFIER, 0,
                         token_set.tokens[ i ].value)
        );

        variable_declaration_node->add_branch(
                new Node(NODE_TYPE_VARIABLE_TYPE, 0,
                         token_set.tokens[ i + 2 ].value)
        );

        // If there's an equals sign, parse the expression after it.

        if ( peakeq(token_set, i, 3, TOKEN_EQUALS))
        {
            int length = distance_next_token(token_set, i + 4, TOKEN_COMMA);
            if ( length < 0 || i + length + 4 > index + statement_length + 1 )
            {
                length = statement_length - ( index - i ) + 1;
            }
            parse_expression(token_set, i + 4, length, *variable_declaration_node);
        }

        root.add_branch(variable_declaration_node);

        // If there's no next expression, break the loop and prevent infinity
        if ( sub_expression_length < 0 || i + sub_expression_length > statement_length )
        {
            break;
        }

        i += sub_expression_length + 1;
    }

    return statement_length + 1;
}


/**
 * Validates a variable declaration.
 * This function checks whether the provided token sequence has a variable declaration
 * in the following format:
 * ```
 * name: type
 * ```
 * @param token_set The token set to validate the variable declaration from.
 * @param index The index of the token set to start validating from.
 * @return The AST Node representing the variable declaration.
 */
void stride::ast::validate_variable_declaration(ast_token_set_t &token_set, cursor_t index)
{
    requires_token(TOKEN_IDENTIFIER, token_set, index, "Expected variable name");
    requires_token(TOKEN_COLON, token_set, index + 1, "Expected colon after variable name, but received none.");
    token_t *type_token = peak(token_set, index, 2);
    if ( type_token == nullptr || !types::is_valid_variable_type(type_token->type))
    {
        blame_token(token_set.tokens[ index + 2 ], "Variable declaration requires valid type after colon.");
    }
}