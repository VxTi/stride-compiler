//
// Created by Luca Warmenhoven on 11/09/2024.
//

#include "definitions/NVariableDeclaration.h"
#include "../NodeProperties.h"

using namespace stride::ast;

/*void parse(TokenSet &tokenSet, )*/

NVariableDeclaration *
NVariableDeclaration::parseSingular(TokenSet &tokenSet, bool allowAssignment, bool implicitDeclaration,
                                    bool allowVariadic)
{
    auto nstVariableDecl = new NVariableDeclaration();

    printf("Next token: %s\n", tokenSet.current().value);

    // If the variable is explicitly declared (let x: type = ... ),
    // we'll have to check for declaration symbols, e.g. let and const.
    // Otherwise, 'implicit' declarations mean they can only contain 'const' before.
    if ( implicitDeclaration)
    {
        if ( !( tokenSet.canConsume(TOKEN_KEYWORD_LET) || tokenSet.canConsume(TOKEN_KEYWORD_CONST)))
        {
            tokenSet.error("Variable declaration requires either 'let' or 'const' keyword.");
            return nullptr;
        }
        else
        {
            bool isConst = tokenSet.canConsume(TOKEN_KEYWORD_CONST);
            nstVariableDecl->setConst(isConst);
            if (isConst)
            {
                tokenSet.next();
            }
        }
    }

    nstVariableDecl->setVariableName(
            tokenSet.consumeRequired(TOKEN_IDENTIFIER, "Expected variable name in implicit declaration.").value);

    tokenSet.consumeRequired(TOKEN_COLON, "Expected colon after variable name, but received none.");

    // Check if the type is a valid variable type.
    // This can be either a primitive type or an identifier. After this,
    // we'll check whether it's a primitive of identifier (sequence)
    if ( !stride::ast::validateVariableType(tokenSet))
    {
        tokenSet.error("Variable declaration requires valid type after colon.");
        return nullptr;
    }

    // Parse variable type, identifier(sequence) or primitive
    // and continue parsing the variable declaration.
    nstVariableDecl->setVariableType(
            tokenSet.canConsume(TOKEN_IDENTIFIER) ?
            std::variant<std::string *, token_type_t>(&stride::ast::parseIdentifier(tokenSet)->name) :
            std::variant<std::string *, token_type_t>(tokenSet.next().type)
    );

    // Check if the variable is an array.
    // If this is the case, we'll have to check if the assignment is of an array type,
    // if there's assignment at all.
    nstVariableDecl->setIsArray(tokenSet.consume(TOKEN_LSQUARE_BRACKET) && tokenSet.consume(TOKEN_RSQUARE_BRACKET));

    // Variadic checks
    if ( tokenSet.consume(TOKEN_THREE_DOTS))
    {
        if ( !allowVariadic )
        {
            tokenSet.error("Variadic variable declarations are not allowed here.");
            return nullptr;
        }

        if ( nstVariableDecl->isArray )
        {
            tokenSet.error("Variable is already declared as array.");
            return nullptr;
        }

        nstVariableDecl->setIsArray(true);
    }

    // Check if there's assignment or the declaration ends
    if ( tokenSet.consume(TOKEN_EQUALS))
    {
        if ( !allowAssignment )
        {
            tokenSet.error("Variable declaration does not allow assignment.");
            return nullptr;
        }

        nstVariableDecl->setValue(NExpression::parse(tokenSet, false));
    }

    tokenSet.consumeRequired(TOKEN_SEMICOLON, "Expected semicolon after variable declaration.");

    return nstVariableDecl;
}

void NVariableDeclaration::parse(TokenSet &tokens, stride::ast::Node &parent)
{
    if ( !tokens.canConsume(TOKEN_KEYWORD_LET) && !tokens.canConsume(TOKEN_KEYWORD_CONST))
    {
        tokens.error("Variable declaration requires either 'let' or 'const' keyword.");
    }

    // If there's multiple variables declared after one another,
    // and the 'const' keyword is used for declaration,
    // all of them will be marked as const.
    bool isConst = tokens.next().type == ( TOKEN_KEYWORD_CONST );

    do
    {
        printf("[NVariableDeclaration] Parsing variable declaration\n");
        auto nstVariableDecl = new NVariableDeclaration();
        nstVariableDecl->setConst(isConst);

        nstVariableDecl->setVariableName(
                tokens.consumeRequired(TOKEN_IDENTIFIER, "Expected variable name after 'const' or 'let'.").value);

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
        nstVariableDecl->setVariableType(
                tokens.canConsume(TOKEN_IDENTIFIER) ?
                std::variant<std::string *, token_type_t>(&stride::ast::parseIdentifier(tokens)->name) :
                std::variant<std::string *, token_type_t>(tokens.next().type)
        );

        printf("[NVariableDeclaration] Variable name: %s\n", nstVariableDecl->varName->c_str());
        printf("[NVariableDeclaration] Variable type: %s\n",
               std::holds_alternative<std::string *>(nstVariableDecl->varType) ?
               std::get<std::string *>(nstVariableDecl->varType)->c_str() : "Primitive");

        // Check if the variable is an array.
        // If this is the case, we'll have to check if the assignment is of an array type,
        // if there's assignment at all.
        nstVariableDecl->setIsArray(tokens.consume(TOKEN_LSQUARE_BRACKET) && tokens.consume(TOKEN_RSQUARE_BRACKET));

        // Check if there's assignment or the declaration ends
        if ( tokens.consume(TOKEN_EQUALS))
        {
            printf("Assignment found\n");
            nstVariableDecl->setValue(NExpression::parse(tokens, false));
        }

        parent.addChild(nstVariableDecl);
    } while ( tokens.consume(TOKEN_COMMA));
}