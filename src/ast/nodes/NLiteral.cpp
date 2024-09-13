//
// Created by Luca Warmenhoven on 12/09/2024.
//

#include "definitions/NLiteral.h"

NLiteral::NLiteral(token_t token)
{
    switch ( token.type )
    {
        case TOKEN_NUMBER_FLOAT:
        {
            double numericalValue = strtod(token.value, nullptr);
            int64_t intBits = *(int64_t *) &numericalValue;

            this->byteCount = ( intBits & ~INT32_MASK ) ? 8 : 4;
            this->value = { (double_t) numericalValue };
        }
            break;
        case TOKEN_NUMBER_INTEGER:
        {
            int64_t intValue = strtoll(token.value, nullptr, 10);
            this->value = { value };
            this->byteCount = intValue & ~INT32_MASK ? 8 :
                              intValue & ~INT16_MASK ? 4 :
                              intValue & ~INT8_MASK ? 2 : 1;
        }
            break;
        case TOKEN_STRING_LITERAL:
            byteCount = strlen(token.value);
            this->value = { token.value };
            break;

        case TOKEN_CHAR_LITERAL:
            byteCount = 1;
            this->value = { (int8_t) token.value[ 0 ] };
            break;
        default:
            throw std::runtime_error("Invalid token type for literal.");
    }
}

int NLiteral::bytes()
{
    return this->byteCount;
}

bool NLiteral::isInteger()
{
    return value.index() == 0; // int8_t, int64_t
}

bool NLiteral::isNumeric()
{
    return value.index() <= 1; // int8_t, int64_t, double_t
}