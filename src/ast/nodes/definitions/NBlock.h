#include "../../ASTNodes.h"

/**
     * Represents a block.
     * A block is a sequence of statements and expressions.
     */
class NBlock : public stride::ast::Node
{
    enum stride::ast::ENodeType getType() override
    { return stride::ast::BLOCK; }
};