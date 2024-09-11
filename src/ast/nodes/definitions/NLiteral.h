#include "../../ASTNodes.h"

#include "../../../tokens/TokenSet.h"
#include "../../../tokens/token.h"


/*
     * Represents a literal.
     * Literals are used to represent fixed values.
     * For example, in the expression "1 + 2", the literals are "1" and "2", respectively.
     */
class NLiteral : public stride::ast::Node
{
public:

    void *value;

    /**
     * Create a new literal node.
     * @param val The current of the literal.
     */
    explicit NLiteral(void *val) : value(val)
    {}

    enum stride::ast::ENodeType getType() override
    { return stride::ast::LITERAL; }

};