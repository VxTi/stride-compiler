//
// Created by Luca Warmenhoven on 12/09/2024.
//

#include "definitions/NBinaryOperation.h"
#include <cmath>

bool NBinaryOperation::isReducible()
{
    // If both nodes of this binary operation are literals,
    // then this operation is reducible into a single literal.
    if ( this->left->getType() == stride::ast::LITERAL &&
         this->right->getType() == stride::ast::LITERAL )
    {
        return true;
    }
    return this->left->isReducible() || this->right->isReducible();
}

/**
 * Converts the provided LiteralValue to a double.
 * This is required to perform operations on literals.
 * @param value The value to convert.
 * @return
 */
double toDouble(const LiteralValue &value)
{
    return value.index() == 0 ? static_cast<double>(std::get<int64_t>(value)) : std::get<double_t>(value);
}

/**
 * Performs an operation on two literals.
 * @tparam Operation The operation to perform.
 * @param leftLiteral The left literal.
 * @param rightLiteral The right literal.
 * @param operation The operation to perform.
 * @return The result of the operation.
 */
template<typename Operation>
NLiteral *performOperation(const NLiteral *leftLiteral, const NLiteral *rightLiteral, Operation operation)
{
    double left = toDouble(leftLiteral->value);
    double right = toDouble(rightLiteral->value);
    return new NLiteral(operation(left, right));
}


NExpression::ExpressionReducible NBinaryOperation::reduce()
{
    if ( !isReducible())
    {
        return this;
    }

    // If both left and right are literals,
    // we can reduce this operation.
    if ( this->left->getType() == stride::ast::LITERAL &&
         this->right->getType() == stride::ast::LITERAL )
    {
        auto leftLiteral = dynamic_cast<NLiteral *>(this->left);
        auto rightLiteral = dynamic_cast<NLiteral *>(this->right);

        // This may never happen. This can only happen if a node was artificially
        // created and given the wrong type.
        if ( !leftLiteral || !rightLiteral )
        {
            throw std::runtime_error("Failed to cast left or right literal to NLiteral.");
        }

        // If either is not a number, it must be a string.
        if ( !leftLiteral->isNumeric() || !rightLiteral->isNumeric())
        {
            // If one is a number and the other is a string, we cannot perform a binary operation.
            if ( leftLiteral->isNumeric() || rightLiteral->isNumeric())
            {
                throw std::runtime_error("Cannot perform binary operation on a string and a number.");
            }

            // Append both strings
            return { new NLiteral(std::string(std::get<const char *>(leftLiteral->value)).append(
                    std::get<const char *>(rightLiteral->value)).c_str()) };
        }

        // Now we're sure they're both numbers

        switch ( this->operation )
        {
            case ADD:
                return performOperation(leftLiteral, rightLiteral, std::plus<>());
            case SUBTRACT:
                return performOperation(leftLiteral, rightLiteral, std::minus<>());
            case MULTIPLY:
                return performOperation(leftLiteral, rightLiteral, std::multiplies<>());
            case POWER:
                return performOperation(leftLiteral, rightLiteral, [](double a, double b) { return std::pow(a, b); });
            case DIVIDE:
                return performOperation(leftLiteral, rightLiteral, std::divides<>());
            case MODULO:
                return performOperation(leftLiteral, rightLiteral, std::modulus<int64_t>());
            case EQUALS:
                return performOperation(leftLiteral, rightLiteral, std::equal_to<>());
            case NOT_EQUALS:
                return performOperation(leftLiteral, rightLiteral, std::not_equal_to<>());
            case LESS_THAN:
                return performOperation(leftLiteral, rightLiteral, std::less<>());
            case GREATER_THAN:
                return performOperation(leftLiteral, rightLiteral, std::greater<>());
            case LESS_THAN_EQUALS:
                return performOperation(leftLiteral, rightLiteral, std::less_equal<>());
            case AND:
                return performOperation(leftLiteral, rightLiteral, std::logical_and<>());
            case OR:
                return performOperation(leftLiteral, rightLiteral, std::logical_or<>());
            case SHIFT_LEFT:
            case SHIFT_RIGHT:
            case BITWISE_AND:
            case BITWISE_OR:
            case XOR:
                if ( leftLiteral->value.index() == 0 && rightLiteral->value.index() == 0 )
                {
                    int64_t left = std::get<int64_t>(leftLiteral->value);
                    int64_t right = std::get<int64_t>(rightLiteral->value);
                    switch ( this->operation )
                    {
                        case SHIFT_LEFT:
                            return new NLiteral(left << right);
                        case SHIFT_RIGHT:
                            return new NLiteral(left >> right);
                        case BITWISE_AND:
                            return new NLiteral(left & right);
                        case BITWISE_OR:
                            return new NLiteral(left | right);
                        case XOR:
                            return new NLiteral(left ^ right);
                        default:
                            break;
                    }
                }
                throw std::runtime_error("Cannot perform bitwise operations on non-integers.");

            default:
                throw std::runtime_error("Invalid binary operation.");
        }
    }

    // If the left node is reducible, reduce it.
    if ( this->left->isReducible())
    {
        auto leftReducible = dynamic_cast<NExpression *>(this->left);
        if ( !leftReducible )
        {
            throw std::runtime_error("Failed to cast left node to NExpression.");
        }

        auto reducedLeft = leftReducible->reduce();
        this->left = std::holds_alternative<NLiteral *>(reducedLeft) ?
                     dynamic_cast<NExpression *>(std::get<NLiteral *>(reducedLeft)) :
                     std::get<NExpression *>(reducedLeft);
    }

    if ( this->right->isReducible())
    {
        auto rightReducible = dynamic_cast<NExpression *>(this->right);
        if ( !rightReducible )
        {
            throw std::runtime_error("Failed to cast right node to NExpression.");
        }

        auto reducedRight = rightReducible->reduce();
        auto rightExpr = std::holds_alternative<NLiteral *>(reducedRight) ?
                         dynamic_cast<NExpression *>(std::get<NLiteral *>(reducedRight)) :
                         std::get<NExpression *>(reducedRight);

    }

    if ( this->isReducible())
    {
        return this->reduce();
    }

    return this;
}