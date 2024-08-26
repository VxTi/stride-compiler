//
// Created by Luca Warmenhoven on 24/08/2024.
//

#include "../ast.h"

using namespace stride::ast;

/**
 * Parses a variable declaration expression.
 * This must be in one of the following formats:
 * ```
 * var name: type = value;
 * var name1: type1 = value1, name2: type2 = value2;
 * var name1: type1, name2: type2;
 * ```
 * This function starts parsing after the 'var' keyword, in the parsePartial function.
 * @param token_set The token set to parse the variable declaration from.
 * @param index  The index of the token set to start parsing from.
 */
int stride::ast::parse_variable_declaration(ast_token_set_t &token_set, cursor_t index, Node &root)
{

    int skipped = 1;
    bool hasNext;

    do
    {
        // Validate whether the variable has the 'name: type' format.
        validate_variable_declaration(token_set, index + 1);

        // We ensured this is not NULL in previous function call
        token_t *type_token = peak(token_set, index, 3);
        skipped += 3;

        // Variable declaration Node
        auto *variable_declaration_node = new Node(NODE_TYPE_VARIABLE_DECLARATION, 0);

        // The identifier containing the name of the variable
        variable_declaration_node->addBranch(
                new Node(NODE_TYPE_IDENTIFIER, 0,
                         token_set.tokens[ index + 1 ].value)
        );

        // Node containing the variable type
        variable_declaration_node->addBranch(
                new Node(NODE_TYPE_VARIABLE_TYPE, 0,
                         type_token->value)
        );

        variable_declaration_node->flags = is_previous(token_set, TOKEN_KEYWORD_CONST, index) ?
                                           FLAG_VARIABLE_IMMUTABLE : 0;
        token_t *nextToken = peak(token_set, index, 4);

        // Ensure there's a next token. We want either a comma or a value assignment
        if ( nextToken == nullptr )
        {
            error("Expected semicolon or variable value after variable declaration, but received neither.");
        }

        if ( nextToken->type == TOKEN_EQUALS )
        {
            int length = 0;
            // Calculate expression length
            for ( int i = index; i < token_set.token_count; i++ )
            {
                if ( token_set.tokens[ i ].type == TOKEN_SEMICOLON )
                {
                    length = i - index;
                    break;
                }
            }
            // Parse the expression after the equals sign
            skipped += parse_expression(token_set, index + 4, token_set.token_count, *variable_declaration_node);
            nextToken = peak(token_set, index, skipped);
            hasNext = nextToken->type == TOKEN_COMMA;
        }
        else
        {
            hasNext = nextToken->type == TOKEN_COMMA;
        }
        root.addBranch(variable_declaration_node);

    } while ( hasNext );

    return skipped;
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
    requires_token(TOKEN_IDENTIFIER, token_set, index, "Expected variable name, but received %s",
                   token_set.tokens[ index ].value);
    requires_token(TOKEN_COLON, token_set, index + 1, "Expected colon after variable name, but received none.");
    token_t *type_token = peak(token_set, index, 2);
    if ( type_token == nullptr || !is_valid_var_type(type_token->type))
    {
        error("Received invalid properties after token declaration at line %d column %d: %s",
              token_set.tokens[ index + 1 ].line, token_set.tokens[ index + 1 ].column,
              token_set.tokens[ index + 1 ].value);
    }
}