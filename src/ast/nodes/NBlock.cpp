//
// Created by Luca Warmenhoven on 11/09/2024.
//

#include "definitions/NBlock.h"
#include "../Lookahead.h"

TokenSet *NBlock::captureRaw(TokenSet &set)
{
    auto subset = stride::ast::captureBlock(set, TOKEN_LBRACE, TOKEN_RBRACE);

    if (subset == nullptr)
    {
        set.error("Block requires opening and closing brackets.");
        return nullptr;
    }

    return subset;
}

NBlock *NBlock::capture(TokenSet &set)
{
    auto block = new NBlock();
    auto subset = NBlock::captureRaw(set);
    stride::ast::parser::parse(*subset, *block);
    return block;
}