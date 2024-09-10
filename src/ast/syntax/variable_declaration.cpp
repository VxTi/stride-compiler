//
// Created by Luca Warmenhoven on 24/08/2024.
//

#include "../abstractions/AST.h"
#include "variable_types.h"

using namespace stride::ast;

void stride::ast::parse_variable_declaration_segment(ast_token_set_t &token_set, cursor_t index, int token_count,
                                                     bool allow_assignment, Node &root, int flags)
{
    requires_token(TOKEN_IDENTIFIER, token_set, index, "Expected variable name");
    requires_token(TOKEN_COLON, token_set, index + 1, "Expected colon after variable name, but received none.");

    // Check if the type is a valid variable type.
    // This can be either a primitive type or an identifier. After this,
    // we'll check whether it's a primitive of identifier (sequence)
    if ( !types::is_valid_variable_type(token_set.tokens[ index + 2 ].type))
    {
        blame_token(token_set.tokens[ index + 2 ], "Variable declaration requires valid type after colon.");
        return;
    }

    auto *variable_declaration_node = new Node(NODE_TYPE_VARIABLE_DECLARATION, flags);

    variable_declaration_node->add_branch(
            new Node(NODE_TYPE_IDENTIFIER, 0,
                     token_set.tokens[ index ].current)
    );

    int identifier_len = 1;

    // If it's an identifier, we'll have to check whether it's a sequence, e.g. whether the identifier
    // is defined in a module, with the 'module::submodule::identifier' syntax.
    auto *variable_type_node = new Node(NODE_TYPE_VARIABLE_TYPE, 0);
    if ( peekeq(token_set, index + 2, TOKEN_IDENTIFIER))
    {
        identifier_len = parse_identifier(token_set, index + 2, *variable_type_node);
    }
    else // Regular variable type (primitives)
    {
        variable_type_node->add_branch(new Node(NODE_TYPE_IDENTIFIER, 0, token_set.tokens[ index + 2 ].current));
    }
    variable_declaration_node->add_branch(variable_type_node);
    // Check if the variable is an array.
    if ( peekeq(token_set, index + identifier_len + 2, TOKEN_LSQUARE_BRACKET) &&
         peekeq(token_set, index + identifier_len + 3, TOKEN_RSQUARE_BRACKET))
    {
        variable_declaration_node->flags |= FLAG_VARIABLE_ARRAY;
        identifier_len += 2;
    } // Add the identifier to the variable declaration node, if assignment is allowed.

    if ( peekeq(token_set, index + identifier_len + 2, TOKEN_EQUALS))
    {
        if ( !allow_assignment )
        {
            blame_token(token_set.tokens[ index + identifier_len + 2 ],
                        "Variable declaration does not allow assignment.");
        }
        parse_expression(token_set, index + identifier_len + 3, token_count - identifier_len - 3,
                         *variable_declaration_node);
    }
    else if ( !peekeq(token_set, index + identifier_len + 2, TOKEN_SEMICOLON) &&
              !peekeq(token_set, index + identifier_len + 2, TOKEN_COMMA))
    {
        blame_token(token_set.tokens[ index + identifier_len + 2 ], "Variable declaration requires a valid postfix notation. This can be done using assignment, a comma separator for next declaration or a semicolon.");
    }
    root.add_branch(variable_declaration_node);
}

/**
 * Parses a variable declaration expression.
 * This must be in one of the following formats:
 * ```
 * var name: type = current;
 * var name1: type1 = value1, name2: type2 = value2;
 * var name1: type1, name2: type2;
 * ```
 * This function starts parsing after the 'var' keyword, in the parse_tokens function.
 * @param token_set The required_token set to parse the variable declaration from.
 * @param index  The index of the required_token set to start parsing from.
 */
int stride::ast::parse_variable_declaration(ast_token_set_t &token_set, cursor_t index, Node &root)
{
    // We start from the keyword of the declaration, e.g. after 'let'

    if ( !peekeq(token_set, index, TOKEN_KEYWORD_LET) &&
         !peekeq(token_set, index, TOKEN_KEYWORD_CONST))
    {
        blame_token(token_set.tokens[ index ], "Variable declaration requires either 'const' or 'let' as initializer.");
    }

    int var_flags = peekeq(token_set, index, TOKEN_KEYWORD_CONST) ? FLAG_VARIABLE_IMMUTABLE : 0;
    int statement_length = distance_next_token(token_set, index + 1, TOKEN_SEMICOLON);

    if ( statement_length < 0 )
    {
        blame_token(token_set.tokens[ index ], "Expected semicolon after variable declaration, but received none");
        return 0;
    }

    // Calculate the length of the secondary expression, if there is one.
    int sub_expression_length = distance_next_token_outside_block(token_set, index + 1, TOKEN_COMMA);

    /*
     * Regular variable declaration (singular expression)
     * This is the parsing for when there's only one variable declared on the same line.
     */
    if ( sub_expression_length < 0 || sub_expression_length >= statement_length )
    {
        parse_variable_declaration_segment(token_set, index + 1, statement_length, true, root, var_flags);
        return statement_length + 2;
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
        parse_variable_declaration_segment(token_set, i, sub_expression_length, true, root, var_flags);

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
 * This function checks whether the provided required_token sequence has a variable declaration
 * in the following format:
 * `name: type` or `name: module::class`
 * @param token_set The required_token set to validate the variable declaration from.
 * @param index The index of the required_token set to start validating from.
 * @return The AST Node representing the variable declaration.
 */
void stride::ast::validate_variable_declaration(ast_token_set_t &token_set, cursor_t index)
{
    requires_token(TOKEN_IDENTIFIER, token_set, index, "Expected variable name.");
    requires_token(TOKEN_COLON, token_set, index + 1, "Expected colon after variable name.");

    int identifier_len = is_identifier_sequence(token_set, index + 1);

    token_t *type_token = peak(token_set, index, 2);
    if ( type_token == nullptr || ( !types::is_valid_variable_type(type_token->type) && identifier_len <= 0 ))
    {
        blame_token(token_set.tokens[ index + identifier_len + 1 ],
                    "Variable declaration requires valid type after colon. Use either primitives or identifiers.");
    }
}