//
// Created by Luca Warmenhoven on 11/09/2024.
//

#include "definitions/NModuleDeclaration.h"
#include "definitions/NBlock.h"

void NModuleDeclaration::parse(TokenSet &tokenSet, stride::ast::Node &parent)
{
    tokenSet.consumeRequired(TOKEN_KEYWORD_MODULE, "Module declaration requires 'module' keyword.");

    auto nstModuleDecl = new NModuleDeclaration(
            tokenSet.consumeRequired(TOKEN_IDENTIFIER, "Module requires identifier after declaration.").value
            );

    nstModuleDecl->body = NBlock::capture(tokenSet);
    parent.addChild(nstModuleDecl);
}