//
// Created by Luca Warmenhoven on 11/09/2024.
//

#include "definitions/NTryCatchStatement.h"

void NTryCatchStatement::parse(TokenSet &tokenSet, stride::ast::Node &parent)
{
    tokenSet.consumeRequired(TOKEN_KEYWORD_TRY, "Try-catch statement requires 'try' keyword.");

    auto nstTryCatch = new NTryCatchStatement();

    nstTryCatch->tryBlock = NBlock::capture(tokenSet);

    tokenSet.consumeRequired(TOKEN_KEYWORD_CATCH, "Try-catch statement requires 'catch' keyword after try block.");
    nstTryCatch->exception = NVariableDeclaration::parseSingular(tokenSet, false, false, false);
    nstTryCatch->catchBlock = NBlock::capture(tokenSet);

    parent.addChild(nstTryCatch);
}