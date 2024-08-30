//
// Created by Luca Warmenhoven on 24/08/2024.
//

#include <queue>
#include <map>
#include "../ast.h"
#include "variable_types.h"

using namespace stride::ast;

#define ASSOCIATIVITY_NONE (0)
#define ASSOCIATIVITY_LEFT (1)
#define ASSOCIATIVITY_RIGHT (2)

/**
 * A segment of an expression.
 */
struct expression_segment_t
{

    /**
     * The associativity of the operator.
     * -1 = left, 1 = right, 0 = none
     */
    int associativity;

    /**
     * The precedence of the operator.
     * This is used to determine the order of operations.
     */
    int precedence;
};

const std::map<token_type_t, struct expression_segment_t> expressions = {
        /* Enclosing tokens  */
        { TOKEN_LPAREN,               { .associativity = ASSOCIATIVITY_NONE, .precedence = 10 }}, // (
        { TOKEN_RPAREN,               { .associativity = ASSOCIATIVITY_NONE, .precedence = 10 }}, // )
        { TOKEN_LSQUARE_BRACKET,      { .associativity = ASSOCIATIVITY_NONE, .precedence = 10 }}, // [
        { TOKEN_RSQUARE_BRACKET,      { .associativity = ASSOCIATIVITY_NONE, .precedence = 10 }}, // ]

        /* High precedence operators */
        { TOKEN_STAR,                 { .associativity = ASSOCIATIVITY_NONE, .precedence = 10 }}, // *
        { TOKEN_SLASH,                { .associativity = ASSOCIATIVITY_NONE, .precedence = 10 }}, // /
        { TOKEN_PLUS,                 { .associativity = ASSOCIATIVITY_NONE, .precedence = 10 }}, // +
        { TOKEN_MINUS,                { .associativity = ASSOCIATIVITY_NONE, .precedence = 10 }}, // -
        { TOKEN_PERCENT,              { .associativity = ASSOCIATIVITY_NONE, .precedence = 10 }}, // %
        { TOKEN_DOUBLE_STAR,          { .associativity = ASSOCIATIVITY_NONE, .precedence = 10 }}, // **

        /* Left associative operators */
        { TOKEN_STAR_EQUALS,          { .associativity = ASSOCIATIVITY_LEFT, .precedence = 10 }},  // *=
        { TOKEN_SLASH_EQUALS,         { .associativity = ASSOCIATIVITY_LEFT, .precedence = 10 }},  // /=
        { TOKEN_PLUS_EQUALS,          { .associativity = ASSOCIATIVITY_LEFT, .precedence = 10 }},  // +=
        { TOKEN_MINUS_EQUALS,         { .associativity = ASSOCIATIVITY_LEFT, .precedence = 10 }},  // -=
        { TOKEN_AMPERSAND_EQUALS,     { .associativity = ASSOCIATIVITY_LEFT, .precedence = 10 }},  // &=
        { TOKEN_PERCENT_EQUALS,       { .associativity = ASSOCIATIVITY_LEFT, .precedence = 10 }},  // %=
        { TOKEN_BANG_EQUALS,          { .associativity = ASSOCIATIVITY_LEFT, .precedence = 10 }},  // !=
        { TOKEN_PIPE_EQUALS,          { .associativity = ASSOCIATIVITY_LEFT, .precedence = 10 }},  // |=
        { TOKEN_TILDE_EQUALS,         { .associativity = ASSOCIATIVITY_LEFT, .precedence = 10 }},  // ~=
        { TOKEN_DOUBLE_LARROW_EQUALS, { .associativity = ASSOCIATIVITY_LEFT, .precedence = 10 }},  // <<=
        { TOKEN_DOUBLE_RARROW_EQUALS, { .associativity = ASSOCIATIVITY_LEFT, .precedence = 10 }},  // >>=
        { TOKEN_DOUBLE_STAR_EQUALS,   { .associativity = ASSOCIATIVITY_LEFT, .precedence = 10 }},  // **=
        { TOKEN_CARET_EQUALS,         { .associativity = ASSOCIATIVITY_LEFT, .precedence = 10 }}, // ^=

        /* Bitwise operators */
        { TOKEN_DOUBLE_RARROW,        { .associativity = ASSOCIATIVITY_LEFT, .precedence = 10 }}, // >>
        { TOKEN_DOUBLE_LARROW,        { .associativity = ASSOCIATIVITY_LEFT, .precedence = 10 }}, // <<
        { TOKEN_DOUBLE_PIPE,          { .associativity = ASSOCIATIVITY_LEFT, .precedence = 10 }}, // ||
        { TOKEN_DOUBLE_AMPERSAND,     { .associativity = ASSOCIATIVITY_LEFT, .precedence = 10 }}, // &&
        { TOKEN_CARET,                { .associativity = ASSOCIATIVITY_LEFT, .precedence = 10 }}, // ^
        { TOKEN_AMPERSAND,            { .associativity = ASSOCIATIVITY_LEFT, .precedence = 10 }}, // &
        { TOKEN_TILDE,                { .associativity = ASSOCIATIVITY_LEFT, .precedence = 10 }}, // ~
        { TOKEN_PIPE,                 { .associativity = ASSOCIATIVITY_LEFT, .precedence = 10 }}, // |
        { TOKEN_BANG,                 { .associativity = ASSOCIATIVITY_LEFT, .precedence = 10 }}, // !
        { TOKEN_PERCENT,              { .associativity = ASSOCIATIVITY_LEFT, .precedence = 10 }}, // %


        { TOKEN_DOUBLE_PLUS,          { .associativity = ASSOCIATIVITY_LEFT |
                                                         ASSOCIATIVITY_RIGHT, .precedence = 10 }}, // ++
        { TOKEN_DOUBLE_MINUS,         { .associativity = ASSOCIATIVITY_LEFT |
                                                         ASSOCIATIVITY_RIGHT, .precedence = 10 }}, // --

        /* Comparison operators */
        { TOKEN_LARROW,               { .associativity = ASSOCIATIVITY_NONE, .precedence = 10 }}, // <
        { TOKEN_RARROW,               { .associativity = ASSOCIATIVITY_NONE, .precedence = 10 }}, // >
        { TOKEN_LEQUALS,              { .associativity = ASSOCIATIVITY_NONE, .precedence = 10 }}, // <=
        { TOKEN_GEQUALS,              { .associativity = ASSOCIATIVITY_NONE, .precedence = 10 }}, // >=
        { TOKEN_DOUBLE_EQUALS,        { .associativity = ASSOCIATIVITY_NONE, .precedence = 10 }}, // ==
        { TOKEN_NOT_EQUALS,           { .associativity = ASSOCIATIVITY_NONE, .precedence = 10 }}, // !=
        { TOKEN_EQUALS,               { .associativity = ASSOCIATIVITY_NONE, .precedence = 10 }}, // = (assignment)


};

/**
 * Parses an expression, which can be after variable declaration / assignment,
 * or providing function parameters.
 * An expression can be in the following format:
 * ```
 * var k: unknown = (5 + (3 * 4))
 * ```
 * Here, the segment after the `unknown = ...` is the expression,
 * which will be parsed using this function.
 * @param token_set The token set to parse a segment from
 * @param cursor The cursor position in the token set
 * @param token_count The amount of tokens in the subset
 * @param parent_node The parent AST Node to put the result into.
 * @return How many tokens were skipped.
 */
int stride::ast::parse_expression(ast_token_set_t &token_set, size_t cursor, size_t token_count, Node &parent_node)
{
    int skipped = 0;

    /*
     * In case the expression is just a singular value,
     * we'll just return that as a Node and return the ast.
     * No need to check for any other cases.
     */
    if ( token_count == 1 )
    {
        token_type_t type = token_set.tokens[ cursor ].type;
        if ( types::is_valid_literal_value(type))
        {
            parent_node.add_branch(new Node(NODE_TYPE_VALUE, 0, token_set.tokens[ cursor ].value));
            return 1;
        }
        blame_token(token_set.tokens[ cursor ], "Expected value or expression");
    }

    auto *expression_node = new Node(NODE_TYPE_EXPRESSION);

    std::queue<Node *> operator_queue;
    std::queue<Node *> literal_queue;

    for ( int i = 0; i < token_count; i++ )
    {
        if ( is_function_call(token_set, cursor + i))
        {
            auto *fn_call_node = new Node(NODE_TYPE_FUNCTION_CALL);
            i += parse_identifier(token_set, cursor + i, *fn_call_node);
            printf("Parsing function call\n");
            expression_node->add_branch(fn_call_node);
        }
        else
        {
            // Check whether the token is an operator or a value.
            if ( !expressions.contains(token_set.tokens[ cursor + i ].type) &&
                 !types::is_valid_literal_value(token_set.tokens[ cursor + i ].type))
            {
                blame_token(token_set.tokens[ cursor + i ], "Expected operator or value in expression.");
            }
        }
    }

    parent_node.add_branch(expression_node);

    // TODO: Implement.

    return skipped;
}