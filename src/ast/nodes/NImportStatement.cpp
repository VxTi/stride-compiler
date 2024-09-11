//
// Created by Luca Warmenhoven on 11/09/2024.
//

#include "../ASTNodes.h"
#include "definitions/NImportStatement.h"

void NImportStatement::parse(TokenSet &tokenSet, stride::ast::Node &parent)
{
    tokenSet.consumeRequired(TOKEN_KEYWORD_IMPORT, "Expected 'import' keyword in import statement.");

    parent.addChild(new NImportStatement(
            tokenSet.consumeRequired(TOKEN_STRING_LITERAL,
                                     "Expected string literal after 'import' keyword in import statement.").value
    ));
    tokenSet.consumeRequired(TOKEN_SEMICOLON, "Expected semicolon after import statement.");
}

