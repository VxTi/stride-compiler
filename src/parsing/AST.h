/**///
// Created by Luca Warmenhoven on 19/08/2024.
//

#ifndef STRIDE_LANGUAGE_AST_H
#define STRIDE_LANGUAGE_AST_H

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

    STRUCTURE,
    INCLUDE_EXTERNAL,
    DECLARATION,
    ASSIGN,
    CONDITIONAL,
    REPETITION,
    FUNCTION_CALL,
    FUNCTION_DEF,

    RETURN, THROW,

    BLOCK, INDEX,
} ast_node_type_t;

/**
 * Defines a node in the abstract syntax tree.
 * This node can have multiple branches, which are also nodes,
 * or have a value of a certain type.
 */
typedef struct ast_node_t
{
    ast_node_type_t type;
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
    bool immutable;
} ast_declaration_node_t;

/*
 * Abstract syntax tree node.
 */
class ASTNode {
public:
    ASTNode *branches;
    size_t branch_count;
    ast_node_type_t type;
    void *value;

    ASTNode(ast_node_type_t type, size_t branch_count, ASTNode *branches, void *value) {
        this->type = type;
        this->branch_count = branch_count;
        this->branches = branches;
        this->value = value;
    }
};

/**
 * Abstract syntax tree parser.
 */
class AST {
private:
    token_t **tokens;
    size_t token_count;
    size_t current_index;

    token_t* next();
    token_t* previous();
    token_t* peak(int offset);

    bool hasNext() const;
    bool hasPrevious() const;

    bool isPrevious(token_type_t type);
    bool isNext(token_type_t type);
    bool isInRange(token_type_t type, int range);

    bool isValidType(token_type_t type);

    void requiresAt(token_type_t type, int offset, char *errorMessage, ...);

    ast_node_t parseExpression();
    ast_node_t parsePrimary();
    ast_node_t parseUnary();
    ast_node_t parseBinary(ast_node_t left, int precedence);
    ast_node_t parseAssignment();
    ast_node_t parseDeclaration();
    ast_node_t parseFunction();
    ast_node_t parseBlock();
    ast_node_t parseStatement();
    ast_node_t parseConditional();
    ast_node_t parseRepetition();
    ast_node_t parseComparison();

    void traverseBlocksRecursively();

public:
    AST(token_t **tokens, size_t token_count);
    ASTNode parse();

    ASTNode* createConditional(ASTNode condition, ASTNode if_true, ASTNode if_false);

    /**
     * Create a repetition node.
     * @param condition The condition to check.
     * @param body The body of the repetition.
     * @return
     */
    ASTNode* createRepetition(ASTNode condition, ASTNode body);

    /**
     * Create a comparative node.
     * @param left The left side of the comparison.
     * @param right The right side to compare
     * @param comparison The comparator type.
     * @return
     */
    ASTNode* createComparison(ASTNode *left, ASTNode *right, token_type_t comparison);

    ASTNode* createFunction(ast_function_node_t *node);
};

void ast_parse(token_t *tokens, size_t token_count, ast_node_t **dst, size_t *dst_size);


#endif //STRIDE_LANGUAGE_AST_H
