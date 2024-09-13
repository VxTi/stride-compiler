//
// Created by Luca Warmenhoven on 11/09/2024.
//

#include "definitions/NBlock.h"
#include "../Lookahead.h"

TokenSet *NBlock::captureRaw(TokenSet &set)
{
    return stride::ast::captureBlock(set, TOKEN_LBRACE, TOKEN_RBRACE);
}

NBlock *NBlock::capture(TokenSet &set)
{
    auto block = new NBlock();
    auto subset = NBlock::captureRaw(set);
    printf("Captured block with length %lu\n", subset->size());
    if (subset->size() == 0)
    {
        delete block;
        return nullptr;
    }
    stride::ast::parser::parse(*subset, *block);
    return block;
}