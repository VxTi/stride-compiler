#include "../../ASTNodes.h"
#include "../../../tokens/TokenSet.h"
#include "../../../tokens/token.h"

/**
     * Represents an identifier.
     * Identifiers are used to represent names.
     * For example, in the expression "let x = 1", the identifier is "x".
     */
class NIdentifier : public stride::ast::Node
{
public:
    std::string name;

    explicit NIdentifier(std::string name) :
            name(std::move(name))
    {}

    enum stride::ast::ENodeType getType() override
    { return stride::ast::IDENTIFIER; }
};