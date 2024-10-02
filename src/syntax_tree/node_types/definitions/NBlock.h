#ifndef STRIDE_LANGUAGE_NBLOCK_H
#define STRIDE_LANGUAGE_NBLOCK_H

#include "../../ASTNodes.h"

/**
     * Represents a block.
     * A block is a sequence of statements and expressions.
     */
class NBlock : public stride::ast::Node
{

public:
    NBlock() = default;

    enum stride::ast::ENodeType getType() override
    { return stride::ast::BLOCK; }

    /**
     * Captures a block from a set of tokens.
     * @param tokenSet The set of tokens to parse.
     * @param parent The parent node.
     */
    static NBlock *capture(TokenSet &set);

    /**
     * Captures a block from a set of tokens.
     * This function is a little different than the other capture function;
     * this one does not parse the content of the block, it just captures the raw block.
     * @param tokenSet The set of tokens to parse.
     */
    static TokenSet *captureRaw(TokenSet &set);
};

#endif