//
// Created by Luca Warmenhoven on 11/09/2024.
//

#include "../ASTNodes.h"
#include "../NodeProperties.h"

using namespace stride::ast;

void stride::ast::NVariableDeclaration::parse(TokenSet &tokens, stride::ast::Node &parent)
{
    if (!tokens.canConsume(TOKEN_KEYWORD_LET) && !tokens.canConsume(TOKEN_KEYWORD_CONST))
    {
        tokens.error("Variable declaration requires either 'let' or 'const' keyword.");
    }

    bool isConst = false;

    // Captures the current token and moves to the next one,
    // and checks whether the 'const' token is present.
    // If there's multiple variables declared after one another,
    // and the 'const' keyword is used for declaration,
    // all of them will be marked as const.
    if (tokens.next().type == TOKEN_KEYWORD_CONST)
    {
        isConst = true;
    }

    do
    {
        auto nstVariableDecl = new NVariableDeclaration();
        nstVariableDecl->isConst = isConst;

        nstVariableDecl->setVariableName(tokens.consumeRequired(TOKEN_IDENTIFIER, "Expected variable name").value);

        tokens.consumeRequired(TOKEN_COLON, "Expected colon after variable name, but received none.");

        // Check if the type is a valid variable type.
        // This can be either a primitive type or an identifier. After this,
        // we'll check whether it's a primitive of identifier (sequence)
        if ( !stride::ast::validateVariableType(tokens))
        {
            tokens.error("Variable declaration requires valid type after colon.");
            return;
        }

        // Parse variable type, identifier(sequence) or primitive
        // and continue parsing the variable declaration.
        if ( tokens.canConsume(TOKEN_IDENTIFIER))
        {
            nstVariableDecl->setVariableType(parseIdentifier(tokens));
        }
        else
        {
            nstVariableDecl->setVariableType(new NIdentifier(tokens.next().value));
        }

        // Check if the variable is an array.
        if ( tokens.consume(TOKEN_LSQUARE_BRACKET) && tokens.consume(TOKEN_RSQUARE_BRACKET))
        {
            nstVariableDecl->isArray = true;
        }

        // Check if there's assignment or the declaration ends
        if ( tokens.consume(TOKEN_EQUALS))
        {

        }
    } while (tokens.consume(TOKEN_COMMA));

    tokens.consumeRequired(TOKEN_SEMICOLON, "Expected semicolon after variable declaration.");

}