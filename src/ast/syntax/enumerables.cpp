//
// Created by Luca Warmenhoven on 25/08/2024.
//

#include "../ast.h"
#include "variable_types.h"

#define ENUM_IDENTIFIER_REGEXP ("([A-Z][A-Z0-9_]*)")

using namespace stride::ast;

regex_t enum_identifier_regexp;

void compile_reg();

int compiled = 0;

/**
 * Validates whether the identifier is in the correct format, in MACRO_CASE.
 * @param identifier The identifier to validate.
 * @return An integer representing the state of the validation. 1 = valid, 0 = invalid
 */
int validate_enum_identifier(char *identifier)
{
    regmatch_t match;

    if ( !compiled )
    {
        compile_reg();
    }

    // Check if the expression matches
    if ( regexec(&enum_identifier_regexp, identifier, 1, &match, 0) == REG_NOMATCH )
    {
        return 0;
    }

    char *match_str = (char *) malloc(match.rm_eo - match.rm_so + 1);
    int status = 0;
    if ( match_str == nullptr )
    {
        error("Failed to allocate memory for enum identifier comparisons.");
        exit(1);
    }

    // Copy match to match_str
    memcpy((void *) match_str, identifier + match.rm_so, match.rm_eo - match.rm_so);

    // Check if match_str is equal to the identifier,
    // if so, the format is correct.
    if ( !strcmp(identifier, match_str))
    {
        status = 1;
    }

    free(match_str);

    return status;
}

/**
 * Parses an enum block.
 * The enum blocks must be in the following format:
 *
 * enum <name> {
 *  VALUE = 1,
 *  VALUE = 2,
 *  ...
 * }
 */
int stride::ast::parse_enumerable(ast_token_set_t &token_set, cursor_t index, Node &root)
{
    requires_token(TOKEN_IDENTIFIER, token_set, index, "An identifier is required after an enum statement.");
    requires_token(TOKEN_LBRACE, token_set, index + 1, "An opening bracket is required after enum identifier.");
    ast_token_set_t *block = capture_block(token_set, TOKEN_LBRACE, TOKEN_RBRACE, index + 1);
    if ( block == nullptr )
    {
        error("Failed to acquire block after enum definition, likely due to a missing closing bracket.");
        return 0;
    }

    bool has_next_entry;
    token_t current;

    Node *enum_block = new Node(NODE_TYPE_ENUMERATION);

    enum_block->add_branch(new Node(NODE_TYPE_IDENTIFIER, 0, token_set.tokens[ index ].value));

    // If the enum is shared, add the SHARED flag.
    if ( peekeq(*block, index - 2, TOKEN_KEYWORD_PUBLIC))
    {
        printf("Enum is shared\n");
        enum_block->flags |= FLAG_OBJECT_PUBLIC;
    }

    int enum_id = 0, i = 0, enum_value, offset;

    do
    {
        current = block->tokens[ i ];
        offset = 2;
        requires_token(TOKEN_IDENTIFIER, *block, i,
                       "Enumerable names requires an identifier, but received '%s'", (char *) current.value);

        // Check whether all characters are uppercase
        if ( !validate_enum_identifier((char *) current.value))
        {
            error("Enumerable members must be formatted in MACRO_CASE\n.Found: '%s' at line %d column %d.",
                  (char *) current.value,
                  current.line,
                  current.column);
            return 0;
        }

        auto enum_member = new Node(NODE_TYPE_ENUMERATION_MEMBER);

        // Add identifier
        enum_member->add_branch(new Node(NODE_TYPE_IDENTIFIER, 0, current.value));

        enum_value = enum_id;

        // Check if a custom value is assigned
        if ( peekeq(*block, i + 1, TOKEN_EQUALS))
        {

            // Check whether the value after enum entry assignment is an integer
            // This is required.
            if ( peak(*block, i, 2) == nullptr
                 || !types::is_integer(block->tokens[ i + 2 ].type))
            {
                error("Value of enumerable must be an integer, received: %s, at line %d column %d (type %d)",
                      (char *) block->tokens[ i + 2 ].value,
                      block->tokens[ i + 2 ].line,
                      block->tokens[ i + 2 ].column,
                      block->tokens[ i + 2 ].type
                );
            }
            enum_value = atoi(block->tokens[ i + 2 ].value);
            offset = 4;
        }

        int * enum_value_allocated = (int *) malloc(sizeof(int));
        *enum_value_allocated = enum_value;

        enum_member->add_branch(new Node(NODE_TYPE_VALUE, 0, enum_value_allocated));

        // There's a next enum member if there's a comma after the previous one.
        enum_id++;
        has_next_entry = peekeq(*block, i + offset - 1, TOKEN_COMMA);

        // If there's no next entry (comma), and there's no semicolon, exit.
        if ( !has_next_entry && !peekeq(*block, i + offset - 1, TOKEN_SEMICOLON))
        {
            error("The last enum entry requires a semicolon after definition.");
        }

        i += offset;

        // Add enumerable member to enum object.
        enum_block->add_branch(enum_member);
    } while ( has_next_entry );

    root.add_branch(enum_block);

    return block->token_count + 2;
}

/**
 * Compiles the regular expression for checking whether the enumerable
 * entry is in the right format, MACRO_CASE.
 */
void compile_reg()
{

    int regex_error = regcomp(&enum_identifier_regexp, ENUM_IDENTIFIER_REGEXP, REG_EXTENDED);

    if ( regex_error )
    {
        char buffer[128];
        regerror(regex_error, &enum_identifier_regexp, buffer, 128);
        fprintf(stderr,
                "\nAn error occurred whilst attempting to compile regular expression for token identifier validation:\n%s\n\n",
                buffer);
        exit(1);
    }
    compiled = 1;
}
