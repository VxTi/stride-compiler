#include "../../ASTNodes.h"

using namespace stride::ast;

/**
     * Represents a block.
     * A block is a sequence of statements and expressions.
     */
class NBlock : public Node
{
    enum ENodeType getType() override
    { return BLOCK; }
};