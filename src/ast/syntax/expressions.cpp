//
// Created by Luca Warmenhoven on 24/08/2024.
//

#include <queue>
#include <map>
#include "../ast.h"
#include "variable_types.h"

using namespace stride::ast;

#define ASSOCIATIVITY_NONE      (0)
#define ASSOCIATIVITY_LEFT      (1)
#define ASSOCIATIVITY_RIGHT     (2)
#define ASSOCIATIVITY_OPTIONAL  (4) // Meaning the associativity is optional

/**
 * A segment of an expression.
 */
struct expression_segment_t
{

    /**
     * The associativity of the operator.
     * This determines whether it is required for a variable (keyword)
     * to be on the left or right side of the operator, both or neither.
     */
    int associativity;

    /**
     * The precedence of the operator.
     * This is used to determine the order of operations.
     */
    int precedence;

    /**
     * Flags for the segment.
     * These are retrieved from `ast.h`, and
     * determine what kind of node will be created.
     */
    int flags;

    /**
     * The type of node that will be created.
     */
    int node_type;
};

const std::map<token_type_t, struct expression_segment_t> expressions = {
        /* Enclosing tokens  */
        { TOKEN_LPAREN,               { .associativity = ASSOCIATIVITY_NONE, .precedence = 100 }}, // (
        { TOKEN_RPAREN,               { .associativity = ASSOCIATIVITY_NONE, .precedence = 100 }}, // )
        { TOKEN_LSQUARE_BRACKET,      { .associativity = ASSOCIATIVITY_NONE, .precedence = 100 }}, // [
        { TOKEN_RSQUARE_BRACKET,      { .associativity = ASSOCIATIVITY_NONE, .precedence = 100 }}, // ]

        /* High precedence operators */
        { TOKEN_STAR,                 { .associativity = ASSOCIATIVITY_NONE, .precedence = 50, // *
                                              .flags = AST_OPERATION_MUL }},
        { TOKEN_SLASH,                { .associativity = ASSOCIATIVITY_NONE, .precedence = 50, // /
                                              .flags = AST_OPERATION_DIV }},
        { TOKEN_PLUS,                 { .associativity = ASSOCIATIVITY_RIGHT | ASSOCIATIVITY_OPTIONAL,
                                              .precedence = 45, .flags = AST_OPERATION_ADD }}, // + (unary and binary)
        { TOKEN_MINUS,                { .associativity = ASSOCIATIVITY_RIGHT | ASSOCIATIVITY_OPTIONAL,
                                              .precedence = 45, .flags = AST_OPERATION_SUB }}, // - (unary and binary)
        { TOKEN_PERCENT,              { .associativity = ASSOCIATIVITY_NONE, .precedence = 40, // %
                                              .flags = AST_OPERATION_MOD }},
        { TOKEN_DOUBLE_STAR,          { .associativity = ASSOCIATIVITY_NONE, .precedence = 40, // **
                                              .flags = AST_OPERATION_POW }},
        /* Bitwise operators */
        { TOKEN_DOUBLE_RARROW,        { .associativity = ASSOCIATIVITY_LEFT, .precedence = 30, // >>
                                              .flags = AST_OPERATION_RSF }},
        { TOKEN_DOUBLE_LARROW,        { .associativity = ASSOCIATIVITY_LEFT, .precedence = 30, // <<
                                              .flags = AST_OPERATION_RSF }},
        { TOKEN_DOUBLE_PIPE,          { .associativity = ASSOCIATIVITY_LEFT, .precedence = 30, // ||
                                              .flags = AST_CONDITIONAL_OR }},
        { TOKEN_DOUBLE_AMPERSAND,     { .associativity = ASSOCIATIVITY_LEFT, .precedence = 30, // &&
                                              .flags = AST_CONDITIONAL_AND }},
        { TOKEN_CARET,                { .associativity = ASSOCIATIVITY_LEFT, .precedence = 30, // ^
                                              .flags = AST_OPERATION_XOR }},
        { TOKEN_AMPERSAND,            { .associativity = ASSOCIATIVITY_LEFT, .precedence = 30, // &
                                              .flags = AST_OPERATION_AND }},
        { TOKEN_TILDE,                { .associativity = ASSOCIATIVITY_LEFT, .precedence = 30, // ~
                                              .flags = AST_OPERATION_NOT }},
        { TOKEN_PIPE,                 { .associativity = ASSOCIATIVITY_LEFT, .precedence = 30, // |
                                              .flags = AST_OPERATION_OR }},
        { TOKEN_BANG,                 { .associativity = ASSOCIATIVITY_LEFT, .precedence = 30, // !
                                              .flags = AST_CONDITIONAL_NOT }},
        { TOKEN_PERCENT,              { .associativity = ASSOCIATIVITY_LEFT, .precedence = 30, // %
                                              .flags = AST_OPERATION_MOD }},

        /* Left associative operators */
        { TOKEN_STAR_EQUALS,          { .associativity = ASSOCIATIVITY_LEFT, .precedence = 50,  // *=
                                              .flags = AST_OPERATION_MUL }},
        { TOKEN_SLASH_EQUALS,         { .associativity = ASSOCIATIVITY_LEFT, .precedence = 50,  // /=
                                              .flags = AST_OPERATION_MUL }},
        { TOKEN_PLUS_EQUALS,          { .associativity = ASSOCIATIVITY_LEFT, .precedence = 45,  // +=
                                              .flags = AST_OPERATION_MUL }},
        { TOKEN_MINUS_EQUALS,         { .associativity = ASSOCIATIVITY_LEFT, .precedence = 45,  // -=
                                              .flags = AST_OPERATION_MUL }},
        { TOKEN_AMPERSAND_EQUALS,     { .associativity = ASSOCIATIVITY_LEFT, .precedence = 45,  // &=
                                              .flags = AST_OPERATION_MUL }},
        { TOKEN_PERCENT_EQUALS,       { .associativity = ASSOCIATIVITY_LEFT, .precedence = 45,  // %=
                                              .flags = AST_OPERATION_MUL }},
        { TOKEN_BANG_EQUALS,          { .associativity = ASSOCIATIVITY_LEFT, .precedence = 45,  // !=
                                              .flags = AST_OPERATION_MUL }},
        { TOKEN_PIPE_EQUALS,          { .associativity = ASSOCIATIVITY_LEFT, .precedence = 45,  // |=
                                              .flags = AST_OPERATION_MUL }},
        { TOKEN_TILDE_EQUALS,         { .associativity = ASSOCIATIVITY_LEFT, .precedence = 45,  // ~=
                                              .flags = AST_OPERATION_MUL }},
        { TOKEN_DOUBLE_LARROW_EQUALS, { .associativity = ASSOCIATIVITY_LEFT, .precedence = 45,  // <<=
                                              .flags = AST_OPERATION_MUL }},
        { TOKEN_DOUBLE_RARROW_EQUALS, { .associativity = ASSOCIATIVITY_LEFT, .precedence = 45,  // >>=
                                              .flags = AST_OPERATION_MUL }},
        { TOKEN_DOUBLE_STAR_EQUALS,   { .associativity = ASSOCIATIVITY_LEFT, .precedence = 50,  // **=
                                              .flags = AST_OPERATION_MUL }},
        { TOKEN_CARET_EQUALS,  { .associativity = ASSOCIATIVITY_LEFT, .precedence = 45, //  ^=
                                              .flags = AST_OPERATION_MUL }},


        { TOKEN_DOUBLE_PLUS,   { .associativity = ASSOCIATIVITY_LEFT |
                                                         ASSOCIATIVITY_RIGHT, .precedence = 55 }}, // ++
        { TOKEN_DOUBLE_MINUS,  { .associativity = ASSOCIATIVITY_LEFT |
                                                         ASSOCIATIVITY_RIGHT, .precedence = 55 }}, // --

        /* Comparison operators */
        { TOKEN_LARROW,        { .associativity = ASSOCIATIVITY_NONE, .precedence = 10 }}, // <
        { TOKEN_RARROW,        { .associativity = ASSOCIATIVITY_NONE, .precedence = 10 }}, // >
        { TOKEN_LEQUALS,       { .associativity = ASSOCIATIVITY_NONE, .precedence = 10 }}, // <=
        { TOKEN_GEQUALS,       { .associativity = ASSOCIATIVITY_NONE, .precedence = 10 }}, // >=
        { TOKEN_DOUBLE_EQUALS, { .associativity = ASSOCIATIVITY_NONE, .precedence = 10 }}, // ==
        { TOKEN_NOT_EQUALS,    { .associativity = ASSOCIATIVITY_NONE, .precedence = 10 }}, // !=
        { TOKEN_EQUALS,        { .associativity = ASSOCIATIVITY_NONE, .precedence = 10 }}, // = (assignment)
};

/**
 * Parses an expression, which can be after variable declaration / assignment,
 * or providing function parameters.
 * An expression can be in the following format:
 * ` (5 + (3 * 4)) `
 * This function will decompose the expression into the required AST nodes.
 * @param token_set The token set to parse a segment from
 * @param cursor The cursor position in the token set
 * @param token_count The amount of tokens in the subset
 * @param parent_node The parent AST Node to put the result into.
 * @return How many tokens were skipped.
 */
int stride::ast::parse_expression(ast_token_set_t &token_set, int cursor, int token_count, Node &parent_node)
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
    
    bool is_string = token_set.tokens[ cursor ].type == TOKEN_STRING_LITERAL;

    auto *expression_node = new Node(NODE_TYPE_EXPRESSION);

    std::queue<Node *> operator_queue;
    std::queue<Node *> literal_queue;

    int i, j;
    
    for ( i = is_string ? 1 : 0; i < token_count; i++ )
    {
        j = cursor + i;
        if ( is_function_call(token_set,j ))
        {
            i += parse_function_call(token_set, j, *expression_node);
        }
        else
        {
            bool is_literal = types::is_valid_literal_value(token_set.tokens[ j ].type);
            bool has_key = expressions.contains(token_set.tokens[ j ].type);
            // Check whether the token is an operator or a value.
            if ( !has_key && !is_literal )
            {
                blame_token(token_set.tokens[ j], "Expected operator or value in expression.");
                return 0;
            }

            // If it's a literal, add it to the literal queue.
            if ( is_literal )
            {
                literal_queue.push(new Node(NODE_TYPE_VALUE, 0, token_set.tokens[ j ].value));
            }
            else if ( has_key )
            {
                // String expression types only accept appending,
                // or conditional expressions.
                if ( is_string && token_set.tokens[ j].type != TOKEN_PLUS )
                {
                    // Check if it's an opening parenthesis (conditional expression)
                    if ( token_set.tokens[ j ].type == TOKEN_LPAREN)
                    {
                        requires_token(TOKEN_PLUS, token_set, j - 1, "Expected appending operator or conditional expression after string literal.");
                        ast_token_set_t *sub_expression = capture_block(token_set, TOKEN_LPAREN, TOKEN_RPAREN, j);
                        if ( sub_expression == nullptr )
                        {
                            blame_token(token_set.tokens[ j ], "Expected closing parenthesis.");
                        }

                        int nested_expression_length = parse_expression(*sub_expression, 0, sub_expression->token_count, *expression_node);
                        i += nested_expression_length + 2;
                        skipped += nested_expression_length + 2;
                    } else blame_token(token_set.tokens[ j ], "Expected appending operator or conditional expression after string literal.");
                }
            }
        }
    }

    parent_node.add_branch(expression_node);

    // TODO: Implement.

    return skipped;
}