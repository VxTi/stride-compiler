//
// Created by Luca Warmenhoven on 11/09/2024.
//

#include "../ASTNodes.h"
#include "../Lookahead.h"
#include "definitions/NStructureDeclaration.h"

void NStructureDeclaration::parse(TokenSet &tokenSet, stride::ast::Node &parent)
{
    tokenSet.consumeRequired(TOKEN_KEYWORD_STRUCT, "Structure declaration requires 'struct' keyword.");

    auto *nstStructureDecl = new NStructureDeclaration();

    nstStructureDecl->setName(
            tokenSet.consumeRequired(TOKEN_IDENTIFIER, "Expected structure name after structure definition").value);

    auto *structureSubSet = stride::ast::captureBlock(tokenSet, TOKEN_LBRACE, TOKEN_RBRACE);

    if ( structureSubSet == nullptr )
    {
        tokenSet.error("Expected block after structure declaration");
        return;
    }

    do
    {
        nstStructureDecl->addField(NVariableDeclaration::parseSingular(*structureSubSet, false, false, false));
    } while ( structureSubSet->hasNext());

    parent.addChild(nstStructureDecl);
}