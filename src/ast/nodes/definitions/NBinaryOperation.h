#include "../../ASTNodes.h"

#include "../../../tokens/TokenSet.h"
#include "../../../tokens/token.h"
#include "NExpression.h"

using namespace stride::ast;


/**
 * Represents a block.
 * Blocks are used to group statements.
 * For example, in the expression "{ let x = 1; let y = 2; }", the block contains the statements "let x = 1" and "let y = 2".
 */
enum EBinaryOperator
{
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    MODULO,
    EQUALS,
    NOT_EQUALS,
    LESS_THAN,
    GREATER_THAN,
    LESS_THAN_EQUALS,
    GREATER_THAN_EQUALS,
    AND,
    OR,
    XOR,
    SHIFT_LEFT,
    SHIFT_RIGHT,
    ASSIGN,
    ADD_ASSIGN,
    SUBTRACT_ASSIGN,
    MULTIPLY_ASSIGN,
    DIVIDE_ASSIGN,
    MODULO_ASSIGN,
    AND_ASSIGN,
    OR_ASSIGN,
    XOR_ASSIGN,
    SHIFT_LEFT_ASSIGN,
    SHIFT_RIGHT_ASSIGN
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
    NExpression &left;
    NExpression &right;

    NBinaryOperation(NExpression &lhs, enum EBinaryOperator operation, NExpression &rhs) :
            left(lhs),
            right(rhs),
            operation(operation)
    {}

    enum ENodeType getType() override
    { return BINARY_OPERATOR; }

    static void parse(TokenSet &tokenSet, Node &parent);
};