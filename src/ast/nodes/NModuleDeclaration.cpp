//
// Created by Luca Warmenhoven on 11/09/2024.
//

#include "definitions/NModuleDeclaration.h"
#include "../Lookahead.h"
#include "definitions/NBlock.h"

void NModuleDeclaration::parse(TokenSet &tokenSet, stride::ast::Node &parent)
{
    tokenSet.consumeRequired(TOKEN_KEYWORD_MODULE, "Module declaration requires 'module' keyword.");

    auto nstModuleDecl = new NModuleDeclaration(
            tokenSet.consumeRequired(TOKEN_IDENTIFIER, "Module requires identifier after declaration.").value
            );

    auto moduleSubSet = stride::ast::captureBlock(tokenSet, TOKEN_LBRACE, TOKEN_RBRACE);

    if (moduleSubSet == nullptr)
    {
        tokenSet.error("Module requires body after declaration.");
        return;
    }

    auto moduleBlock = new NBlock();
    stride::ast::parser::parse(*moduleSubSet, *moduleBlock);

    nstModuleDecl->addChild(moduleBlock);
    parent.addChild(nstModuleDecl);
}