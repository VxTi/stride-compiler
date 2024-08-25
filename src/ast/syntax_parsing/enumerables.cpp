//
// Created by Luca Warmenhoven on 25/08/2024.
//

#include "../ast.h"
#include "types.h"

using namespace stride::ast;

#define ENUM_IDENTIFIER_REGEXP ("[A-Z]+[a-zA-Z0-9_")

/**
 * Validates whether the identifier is in the correct format, in MACRO_CASE.
 * @param identifier The identifier to validate.
 * @return An integer representing the state of the validation. 1 = valid, 0 = invalid
 */
int validate_enum_identifier(char *identifier)
{
    regex_t regex;
    regmatch_t match;

    if ( !regcomp(&regex, ENUM_IDENTIFIER_REGEXP, REG_EXTENDED))
    {
        error("Invalid regular expression received for parsing enum identifiers.");
    }

    // Check if the expression matches
    if ( !regexec(&regex, identifier, 1, &match, 0) || match.rm_so < 0)
    {
        return 0;
    }

    printf("enum identifier match: %lld, %lld", match.rm_so, match.rm_eo);

    const char *match_str = (char *) malloc(match.rm_eo - match.rm_so + 1);
    int status = 0;
    if ( !match_str )
    {
        error("Failed to allocate memory for enum identifier comparisons.");
        exit(1);
    }

    // Copy match to match_str
    memcpy((void *) identifier, identifier + match.rm_so, match.rm_eo - match.rm_so);

    // Check if match_str is equal to the identifier,
    // if so, the format is correct.
    if ( !strcmp(match_str, identifier))
    {
        status = 1;
    }

    free((void *) match_str);

    return status;
}

int stride::ast::parse_enumerable(ast_token_set_t &token_set, cursor_t index, Node &root)
{
    requires_token(TOKEN_IDENTIFIER, token_set, index, "An identifier is required after an enum statement.");
    requires_token(TOKEN_LBRACE, token_set, index + 1, "An opening bracket is required after enum identifier.");
    ast_token_set_t *block = capture_block(token_set, TOKEN_LBRACE, TOKEN_RBRACE, index);
    if ( block == nullptr )
    {
        error("Failed to acquire block after enum definition, likely due to a missing closing bracket.");
        return 0;
    }

    bool has_next_entry;
    token_t current;

    Node *enum_block = new Node(NODE_TYPE_ENUMERATION);

    // If the enum is shared, add the SHARED flag.
    if ( peakcmp(token_set, index, -2, TOKEN_KEYWORD_SHARED))
    {
        enum_block->flags |= FLAG_OBJECT_SHARED;
    }

    int enum_id = 0;

    do
    {
        current = block->tokens[ index ];
        requires_token(TOKEN_IDENTIFIER, token_set, index++,
                       "Enumerable names requires an identifier, but received '%'", (char *) current.value);

        // Check whether all characters are uppercase
        if ( !validate_enum_identifier((char *) current.value))
        {
            error("Enum entries must be in MACRO_CASE, but received %s at line %d column %d.",
                  (char *) current.value,
                  current.line,
                  current.column);
            return 0;
        }

        auto enum_member = new Node(NODE_TYPE_ENUMERATION_MEMBER);


        // Add identifier
        enum_member->addBranch(new Node(NODE_TYPE_IDENTIFIER, 0, current.value));

        int enum_value = enum_id;

        // Check if a custom value is assigned
        if ( peakcmp(token_set, index, 1, TOKEN_EQUALS))
        {
            // Check whether the value after enum entry assignment is an integer
            // This is required.
            if ( peak(token_set, index, 1) == nullptr
                 || !type_is_integer(token_set.tokens[ index + 1 ].type))
            {
                error("Value of enumerable must be an integer, received: %s, at line %d column %d.",
                      (char *) token_set.tokens[ index + 1 ].value,
                      token_set.tokens[ index + 1 ].line,
                      token_set.tokens[ index + 1 ].column
                );
            }
            enum_value = atoi(token_set.tokens[ index + 1].value);
        }
        enum_block->addBranch(new Node(NODE_TYPE_VALUE, 0, (void *) ( enum_value )));

        enum_id++;
        has_next_entry = is_next(token_set, TOKEN_COMMA, index);

        // If there's no next entry (comma), and there's no next token, stop.
        if ( !has_next_entry && !peakcmp(token_set, index, 1, TOKEN_SEMICOLON))
        {
            error("The last enum entry requires a semicolon after definition.");
        }

        // Add enumerable member to enum object.
        enum_block->addBranch(enum_member);
    } while ( has_next_entry );

    root.addBranch(enum_block);

    return block->token_count + 2;
}


