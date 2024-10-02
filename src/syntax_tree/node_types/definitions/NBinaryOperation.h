#ifndef STRIDE_LANGUAGE_NBINARYOPERATION_H
#define STRIDE_LANGUAGE_NBINARYOPERATION_H

#include "../../ASTNodes.h"
#include "../../../tokens/TokenSet.h"
#include "../../../tokens/token.h"
#include "NExpression.h"
#include "../../Reducible.h"

/**
 * Represents a block.
 * Blocks are used to group statements.
 * For example, in the expression "{ let x = 1; let y = 2; }", the block contains the statements "let x = 1" and "let y = 2".
 */
enum EBinaryOperator
{
    ADD = TOKEN_PLUS,                                           // +
    ADD_ASSIGN = TOKEN_PLUS_EQUALS,                             // +=
    SUBTRACT = TOKEN_MINUS,                                     // -
    SUBTRACT_ASSIGN = TOKEN_MINUS_EQUALS,                       // -=
    MULTIPLY = TOKEN_STAR,                                      // *
    MULTIPLY_ASSIGN = TOKEN_STAR_EQUALS,                        // *=
    POWER = TOKEN_DOUBLE_STAR,                                  // **
    POWER_ASSIGN = TOKEN_DOUBLE_STAR_EQUALS,                    // **=
    DIVIDE = TOKEN_SLASH,                                       // /
    DIVIDE_ASSIGN = TOKEN_SLASH_EQUALS,                         // /=
    MODULO = TOKEN_PERCENT,                                     // %
    MODULO_ASSIGN = TOKEN_PERCENT_EQUALS,                       // %=
    ASSIGN = TOKEN_EQUALS,                                      // =
    EQUALS = TOKEN_DOUBLE_EQUALS,                               // ==
    NOT_EQUALS = TOKEN_NOT_EQUALS,                              // !=
    LESS_THAN = TOKEN_LARROW,                                   // <
    GREATER_THAN = TOKEN_GEQUALS,                               // >
    LESS_THAN_EQUALS = TOKEN_LEQUALS,                           // <=
    SHIFT_LEFT = TOKEN_DOUBLE_LARROW,                           // <<
    GREATER_THAN_EQUALS = TOKEN_GEQUALS,                        // >=
    SHIFT_RIGHT = TOKEN_DOUBLE_RARROW,                          // >>
    SHIFT_LEFT_ASSIGN = TOKEN_DOUBLE_LARROW_EQUALS,             // <<=
    SHIFT_RIGHT_ASSIGN = TOKEN_DOUBLE_RARROW_EQUALS,            // >>=
    BITWISE_AND = TOKEN_AMPERSAND,                              // &
    AND_ASSIGN = TOKEN_AMPERSAND_EQUALS,                        // &=
    AND = TOKEN_DOUBLE_AMPERSAND,                               // &&
    BITWISE_OR = TOKEN_PIPE,                                    // |
    OR_ASSIGN = TOKEN_PIPE_EQUALS,                              // |=
    OR = TOKEN_DOUBLE_PIPE,                                     // ||
    XOR = TOKEN_CARET,                                          // ^
    XOR_ASSIGN = TOKEN_CARET_EQUALS,                            // ^=
};

/**
     * Represents a binary operation.
     * Binary operations are used to evaluate two expressions.
     * For example, in the expression "1 + 2", the binary operation is the addition operation.
     */
class NBinaryOperation : public NExpression
{
public:
    enum EBinaryOperator operation;
    NExpression *left;
    NExpression *right;

    NBinaryOperation(NExpression *left, enum EBinaryOperator operation,
                     NExpression *right) :
            left(std::move(left)),
            right(std::move(right)),
            operation(operation)
    {}

    enum stride::ast::ENodeType getType() override
    {
        return stride::ast::BINARY_OPERATOR;
    }

    /**
     * Checks if the binary operation is reducible.
     * This is the case if both the left and right expressions are reducible,
     * or they're both literals.
     */
    bool isReducible() override;

    /**
     * Reduces the binary operation into a single literal.
     * This is done by reducing the left and right expressions, and then performing the operation.
     */
    std::variant<NExpression *, NLiteral *> reduce() override;

};

#endif