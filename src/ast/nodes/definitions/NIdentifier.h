#include "../../ASTNodes.h"

#include "../../../tokens/TokenSet.h"
#include "../../../tokens/token.h"

using namespace stride::ast;

/**
     * Represents an identifier.
     * Identifiers are used to represent names.
     * For example, in the expression "let x = 1", the identifier is "x".
     */
class NIdentifier : public Node
{
public:
    std::string name;

    explicit NIdentifier(std::string name) :
            name(std::move(name))
    {}

    enum ENodeType getType() override
    { return IDENTIFIER; }
};