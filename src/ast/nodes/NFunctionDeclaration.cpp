//
// Created by Luca Warmenhoven on 24/08/2024.
//

#include "../ASTNodes.h"
#include "../Lookahead.h"
#include "../NodeProperties.h"
#include "definitions/NFunctionDeclaration.h"

/**
 * Parses a function declaration.
 * This function will parse a function declaration in the following format:
 * ```
 * define (external?) (isPublic?) name(param1: type1, param2: type2) -> returnType {
 *     // code
 * }
 * ```
 * The function has to be called AFTER the declaration keyword, e.g. the input stream tokens
 * must look like:
 * define < 'keyword keyword identifier(...)'
 * @param tokenSet The required_token set to parse the function declaration from.
 * @param index The index of the required_token set to start parsing from.
 * @param root The root Node to append the function declaration to.
 */

void NFunctionDeclaration::parse(TokenSet &tokenSet, Node &parent)
{

    if ( !tokenSet.hasNext())
    {
        tokenSet.error("Expected function name or identifiers, but received none.");
    }

    auto *nstFunctionDecl = new NFunctionDeclaration();

    /** Consume function flag tokens */
    for ( ; !tokenSet.canConsume(TOKEN_IDENTIFIER); )
    {
        if ( tokenSet.consume(TOKEN_KEYWORD_PUBLIC))
        {
            if ( nstFunctionDecl->isPublic )
            {
                tokenSet.error("Double 'public' keyword.");
            }
            nstFunctionDecl->isPublic = true;
        }
        else if ( tokenSet.consume(TOKEN_KEYWORD_EXTERNAL))
        {
            if ( nstFunctionDecl->external )
            {
                tokenSet.error("Double 'external' keyword.");
            }
            nstFunctionDecl->external = false;
        }
        else if ( tokenSet.consume(TOKEN_KEYWORD_ASYNC))
        {
            if ( nstFunctionDecl->async )
            {
                tokenSet.error("Double 'async' keyword.");
            }
            nstFunctionDecl->async = true;
        }
        else
        {
            tokenSet.error("Received illegal function keyword.");
        }
    }

    nstFunctionDecl->setFunctionName(
            tokenSet.consumeRequired(TOKEN_IDENTIFIER, "Expected function name after function declaration.").value);

    // Capture block for function parameter body, aka the part after the function name between the parenthesis
    auto *fnParameterSet = stride::ast::captureBlock(tokenSet, TOKEN_LPAREN, TOKEN_RPAREN);

    if ( fnParameterSet == nullptr )
    {
        tokenSet.error("Function declaration requires parameters, but received none.");
        return;
    }

    // If the function declaration has parameters,
    // add the 'parameters' node
    if ( fnParameterSet->size() > 0 )
    {
        bool hasVariadic = false;
        do
        {
            if ( hasVariadic )
            {
                tokenSet.error("Cannot have parameter declaration after variadic function parameter.");
                break;
            }
            auto *nstFnParameter = new NVariableDeclaration();

            if ( fnParameterSet->consume(TOKEN_KEYWORD_CONST))
            {
                nstFnParameter->setConst(true);
            }

            nstFnParameter->setVariableName((char *) fnParameterSet->consumeRequired(TOKEN_IDENTIFIER,
                                                                                     "Expected parameter name after function declaration.").value);
            fnParameterSet->consumeRequired(TOKEN_COLON,
                                            "Expected colon after parameter name in function definition.\nThis is required to denote the parameter_type_token of the parameter.");

            // Validate parameter type
            if ( !stride::ast::validateVariableType(tokenSet))
            {
                fnParameterSet->error("Expected type after colon in function definition.");
            }

            // If the variable type is a reference to a class within a module, use identifier as type.
            // Otherwise, we'll use the token value as the type.
            nstFnParameter->setVariableType(
                    fnParameterSet->canConsume(TOKEN_IDENTIFIER) ? stride::ast::parseIdentifier(tokenSet)
                                                                 : new NIdentifier(
                            tokenSet.next().value)
            );

            // Check if function parameter is of array type.
            if ( tokenSet.consume(TOKEN_LSQUARE_BRACKET) && tokenSet.consume(TOKEN_LSQUARE_BRACKET))
            {
                nstFnParameter->setIsArray(true);
            }
            else if ( tokenSet.consume(TOKEN_THREE_DOTS))
            {
                nstFnParameter->setIsArray(true);
                hasVariadic = true;
            }

        } while ( fnParameterSet->consume(TOKEN_COMMA));

        // The parameter parsing ends when no comma is found.
        // If there's still tokens remaining in the set, that means there's an illegal one out there.
        if ( fnParameterSet->hasNext())
        {
            fnParameterSet->error("Function parameter requires comma or closing parenthesis after declaration.");
        }
    }

    if ( !nstFunctionDecl->external )
    {
        auto *fnBodySet = stride::ast::captureBlock(tokenSet, TOKEN_LBRACE, TOKEN_RBRACE);
        if ( fnBodySet == nullptr )
        {
            tokenSet.error("Function declaration requires a function body, but received none.");
            return;
        }

        auto *nstBlock = new NBlock();
        stride::ast::parser::parse(*fnBodySet, *nstBlock);
    }
    else
    {
        tokenSet.consumeRequired(TOKEN_SEMICOLON,
                                 "External functions are not allowed to have a function body, and must end with a semicolon.");
    }

    parent.addChild(nstFunctionDecl);
}