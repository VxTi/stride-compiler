//
// Created by Luca Warmenhoven on 03/09/2024.
//

#include "../ast.h"
#include "variable_types.h"

using namespace stride::ast;

int stride::ast::parse_structure(ast_token_set_t &token_set, cursor_t index, Node &root)
{

    auto *structure_node = new Node(NODE_TYPE_STRUCTURE);

    requires_token(TOKEN_IDENTIFIER, token_set, index, "Expected structure name after structure definition.");

    auto *structure_name_node = new Node(NODE_TYPE_IDENTIFIER);
    structure_name_node->value = token_set.tokens[ index ].value;
    structure_node->add_branch(structure_name_node);
    int generic_length = parse_generic(token_set, index + 1, *structure_node);
    index += generic_length;

    auto *structure_content_node = new Node(NODE_TYPE_BLOCK);
    ast_token_set_t *structure_content_tokens = capture_block(token_set, TOKEN_LBRACE, TOKEN_RBRACE, index + 1);

    if ( structure_content_tokens == nullptr )
    {
        blame_token(token_set.tokens[ index + 1 ], "Expected block after structure declaration");
        return 0;
    }

    for ( int i = 0, identifier_length; i < structure_content_tokens->token_count; )
    {
        requires_token(TOKEN_IDENTIFIER, *structure_content_tokens, i, "Expected identifier in structure definition.");
        requires_token(TOKEN_COLON, *structure_content_tokens, i + 1,
                       "Expected colon after identifier in structure definition.");

        auto *struct_variable_node = new Node(NODE_TYPE_VARIABLE_DECLARATION);

        // Struct variable name node
        auto *struct_variable_name_node = new Node(NODE_TYPE_IDENTIFIER);
        struct_variable_name_node->value = structure_content_tokens->tokens[ i ].value;
        struct_variable_node->add_branch(struct_variable_name_node);

        identifier_length = is_identifier_sequence(*structure_content_tokens, i + 2);

        if ( identifier_length <= 0 &&
             !types::is_valid_variable_type(structure_content_tokens->tokens[ i + 2 ].type))
        {
            blame_token(structure_content_tokens->tokens[ i + 2 ],
                        "Expected type after colon in structure definition.");
        }
        i += 2;

        // Check if there's an identifier sequence as type (module > class?)
        if ( identifier_length > 0 )
        {
            i += parse_identifier(*structure_content_tokens, i, *struct_variable_node);
        }
        else
        {
            auto *struct_variable_type_node = new Node(NODE_TYPE_VARIABLE_TYPE);
            struct_variable_type_node->value = structure_content_tokens->tokens[ i ].value;
            struct_variable_node->add_branch(struct_variable_type_node);
            i++;
        }

        requires_token(TOKEN_SEMICOLON, *structure_content_tokens, i,
                       "Expected semicolon after struct variable declaration.");
        structure_content_node->add_branch(struct_variable_node);
        i++;
    }

    structure_node->add_branch(structure_content_node);

    root.add_branch(structure_node);

    return structure_content_tokens->token_count + generic_length + 3;
}