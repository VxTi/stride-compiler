/**///
// Created by Luca Warmenhoven on 19/08/2024.
//

#ifndef STRIDE_LANGUAGE_AST_PARSER_H
#define STRIDE_LANGUAGE_AST_PARSER_H

#include <cstdlib>
#include "../analysis/token.h"

typedef enum
{
    /* Arithmetic operators */
    ADD = TOKEN_PLUS,
    SUB = TOKEN_MINUS,
    MUL = TOKEN_STAR,
    DIV = TOKEN_SLASH,
    MOD = TOKEN_PERCENT,
    AND = TOKEN_AMPERSAND,
    OR = TOKEN_PIPE,
    XOR = TOKEN_CARET,
    NOT = TOKEN_BANG,

    LESS_THAN = TOKEN_LESS,
    GREATER_THAN = TOKEN_GREATER,
    LESS_EQUAL = TOKEN_LEQUALS,
    GREATER_EQUAL = TOKEN_GEQUALS,
    EQUAL = TOKEN_DOUBLE_EQUALS,
    NOT_EQUAL = TOKEN_NOT_EQUALS,

    /* Literals */
    NUMBER = TOKEN_NUMBER_LITERAL,
    STRING = TOKEN_STRING_LITERAL,
    IDENTIFIER = TOKEN_IDENTIFIER,

    DECLARATION,
    ASSIGN,
    CONDITIONAL,
    REPETITION,
    FUNCTION_CALL,
    FUNCTION_DEF,

    RETURN, THROW,

    BLOCK, INDEX,
} node_type_t;

/**
 * Defines a node in the abstract syntax tree.
 * This node can have multiple branches, which are also nodes,
 * or have a value of a certain type.
 */
typedef struct ast_node_t
{
    node_type_t type;
    size_t branch_count;
    ast_node_t *branches;
    void *value;
} ast_node_t;

/*
 * Defines a function node in the abstract syntax tree.
 */
typedef struct {
    const char *function_name;
    size_t parameter_count;
    const char **parameters;
    const char *return_type;
    bool publicly_visible;
    bool external;
} ast_function_node_t;


/*
 * Defines a declaration node in the abstract syntax tree.
 */
typedef struct {
    const char *variable_name;
    const char *variable_type;
    ast_node_t *value;
} ast_declaration_node_t;

void ast_parse(token_t *tokens, size_t token_count, ast_node_t **dst, size_t *dst_size);


#endif //STRIDE_LANGUAGE_AST_PARSER_H
