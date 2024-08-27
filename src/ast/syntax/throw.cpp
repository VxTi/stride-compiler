//
// Created by Luca Warmenhoven on 27/08/2024.
//

#include "../ast.h"

using namespace stride::ast;

int stride::ast::parse_throw(ast_token_set_t &token_set, cursor_t index, Node &parent_node)
{
    // Currently this shit is hardcoded, though this will change in the future. This is just so the
    // Error module will work correctly. Content parsing will be added later.
    requires_token(TOKEN_IDENTIFIER, token_set, index, "Throw statement requires an identifier after the 'throw' keyword.");
    if (strcmp((char *) token_set.tokens[index].value, "Error") != 0)
    {
        error("Throw statement requires an 'Error' type after the 'throw' keyword.");
    }
    requires_token(TOKEN_LPAREN, token_set, index + 1, "Throw statement requires an opening parenthesis after the 'Error' type.");
    requires_token(TOKEN_STRING_LITERAL, token_set, index + 2, "Throw statement requires a string literal after the 'Error' type.");
    requires_token(TOKEN_RPAREN, token_set, index + 3, "Throw statement requires a closing parenthesis after the string literal.");
    requires_token(TOKEN_SEMICOLON, token_set, index + 4, "Throw statement requires a semicolon after the closing parenthesis.");

    Node *throw_node = new Node(NODE_TYPE_THROW, 0);
    throw_node->add_branch(new Node(NODE_TYPE_VALUE, 0, token_set.tokens[index + 2].value));
    parent_node.add_branch(throw_node);

    return 4;
}