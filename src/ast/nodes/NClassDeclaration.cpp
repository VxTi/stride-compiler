//
// Created by Luca Warmenhoven on 27/08/2024.
//

#include "../Lookahead.h"
#include "../NodeProperties.h"
#include "definitions/NClassDeclaration.h"

NClassDeclaration::~NClassDeclaration()
{
    for ( auto &parent: *parents )
    {
        delete parent;
    }
    for ( auto &generic: *generics )
    {
        delete generic;
    }

    delete parents;
    delete generics;
    delete body;

    Node::~Node();
}

/**
 * Parses class inheritance.
 * Class intheritance is in the format of <br />
 * <code>
 * class MyClass has ParentClass and SecondaryClass and Generics<A, B>, ... { ... }
 * </code>  <br />
 * This function must be called after the colon required_token.
 * @return The number of tokens skipped.
 * @see parse_class
 */
void NClassDeclaration::parse(TokenSet &tokens, Node &parent)
{
    // First inheritance class
    auto *nstClassDecl = new NClassDeclaration();
    tokens.consumeRequired(TOKEN_KEYWORD_CLASS, "Class declaration requires 'class' keyword.");
    nstClassDecl->className = tokens.consumeRequired(TOKEN_IDENTIFIER,
                                                     "Class inheritance requires parent class name.").value;

    printf("Class name: %s\n", nstClassDecl->className.c_str());

    stride::ast::parseGenerics(tokens, *nstClassDecl->generics);

    if ( tokens.consume(TOKEN_KEYWORD_HAS))
    {
        do
        {
            nstClassDecl->addParent(stride::ast::parseIdentifier(tokens));
        } while ( tokens.consume(TOKEN_KEYWORD_AND));
    }

    nstClassDecl->body = NBlock::capture(tokens);

    parent.addChild(nstClassDecl);
}