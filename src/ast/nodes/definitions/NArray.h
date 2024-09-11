#include "../../ASTNodes.h"
#include "../../../tokens/TokenSet.h"
#include "../../../tokens/token.h"
#include "NExpression.h"

/**
    * Represents an array.
    * Arrays are used to store multiple values.
    * For example, in the expression "[1, 2, 3]", the array contains the values "1", "2" and "3".
    */
class NArray : public NExpression
{
public:
    std::vector<NExpression *> elements;

    NArray() : NExpression()
    {}

    void addElement(NExpression *element)
    {
        elements.push_back(element);
    }

    enum NType getType() override
    { return ARRAY; }

    static void parse(TokenSet &tokenSet, Node &parent);

};