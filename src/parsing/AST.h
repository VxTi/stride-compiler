/**///
// Created by Luca Warmenhoven on 19/08/2024.
//

#ifndef STRIDE_LANGUAGE_AST_H
#define STRIDE_LANGUAGE_AST_H

#include <cstdlib>
#include "../analysis/token.h"

#define BRANCH_GROWTH_FACTOR 2

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
    ARRAY,

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

typedef struct {
    token_t **tokens;
    size_t token_count;
} ast_token_set_t;

typedef struct {
    char *name;
    token_type_t type;
    bool array;
    bool immutable;
} ast_function_param_t;

/*
 * Defines a function node in the abstract syntax tree.
 */
typedef struct
{
    const char *function_name;
    size_t parameter_count;
    ast_function_param_t *parameters;
    const char *return_type;
    bool publicly_visible;
    bool external;
} ast_function_node_t;


/*
 * Defines a declaration node in the abstract syntax tree.
 */
typedef struct
{
    const char *variable_name;
    const char *variable_type;
    ast_node_t *value;
    bool immutable;
} ast_declaration_node_t;

/*
 * Abstract syntax tree node.
 */
class ASTNode
{

public:

    bool isLeaf() const;

    bool hasEmptyLeaves() const;

    void ensureMinimumBranches();


    size_t branch_count_upper_limit;
    void *value;
    ast_node_type_t type;
    ASTNode *branches;
    size_t branch_count;

    void addBranch(ASTNode *node);

    void setValue(void * newVal) {
        this->value = newVal;
    }

    ASTNode(ast_node_type_t type) {
        this->type = type;
        this->branch_count = 0;
        this->value = nullptr;
        this->branch_count_upper_limit = 4;
        this->branches = (ASTNode *) malloc(sizeof(ASTNode) * this->branch_count_upper_limit);
    }
    ASTNode(ast_node_type_t type, size_t branch_count, ASTNode *branches, void *value)
    {
        this->type = type;
        this->branch_count = branch_count;
        this->value = value;
        this->branch_count_upper_limit = 4;
        if ( branches != nullptr )
        {
            this->branches = branches;
        }
        else
        {
            this->branches = (ASTNode *) malloc(sizeof(ASTNode) * this->branch_count_upper_limit);
        }
    }
};

/**
 * Abstract syntax tree parser.
 */
class AST
{
private:
    ast_token_set_t *base_token_set;

    token_t *next(ast_token_set_t &token_set, size_t index);

    token_t *previous(ast_token_set_t &token_set, size_t index);

    token_t *peak(ast_token_set_t &token_set, size_t index, int offset);

    bool hasNext(ast_token_set_t &token_set, size_t index) const;

    bool hasPrevious(size_t index) const;

    bool isPrevious(ast_token_set_t &token_set, token_type_t type, size_t index);

    bool isNext(ast_token_set_t &token_set, token_type_t type, size_t index);

    bool isInRange(ast_token_set_t &token_set, token_type_t type, size_t index, int range);

    bool isValidType(token_type_t type);

    void error(char *error_message, ...);

    void requiresAt(token_type_t type, ast_token_set_t &token_set, size_t index, char *error_message, ...);

    ASTNode *parsePartial(ASTNode *root, ast_token_set_t &token_set);

public:
    AST(token_t &tokens, size_t token_count);

    ASTNode *parse();

    /**
     * Creates a new conditional node, with two children,
     * the true branch and the false branch.
     * @param condition The condition to check.
     * @param if_true The branch to execute if the condition is true.
     * @param if_false The branch to execute if the condition is false.
     * @return
     */
    ASTNode *createConditional(ASTNode condition, ASTNode if_true, ASTNode if_false);

    /**
     * Create a repetition node.
     * @param condition The condition to check.
     * @param body The body of the repetition.
     * @return
     */
    ASTNode *createRepetition(ASTNode condition, ASTNode body);

    /**
     * Create a comparative node.
     * @param left The left side of the comparison.
     * @param right The right side to compare
     * @param comparison The comparator type.
     * @return
     */
    ASTNode *createComparison(ASTNode *left, ASTNode *right, token_type_t comparison);

    /**
     * Create a function call node.
     * @param function_name The name of the function to call.
     * @param parameters The parameters to pass to the function.
     * @return
     */
    ASTNode *createFunction(ast_function_node_t *node);
};

void ast_parse(token_t *tokens, size_t token_count, ast_node_t **dst, size_t *dst_size);


#endif //STRIDE_LANGUAGE_AST_H
