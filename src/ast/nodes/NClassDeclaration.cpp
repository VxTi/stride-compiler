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
    auto *nst_class = new NClassDeclaration();
    nst_class->class_name = tokens.consumeRequired(TOKEN_IDENTIFIER,
                                                   "Class inheritance requires parent class name.").value;

    printf("Class name: %s\n", nst_class->class_name.c_str());

    stride::ast::parseGenerics(tokens, *nst_class->generics);

    if ( tokens.consume(TOKEN_KEYWORD_HAS))
    {
        do
        {
            nst_class->addParent(stride::ast::parseIdentifier(tokens));
        } while ( tokens.consume(TOKEN_KEYWORD_AND));
    }

    auto *subset = stride::ast::captureBlock(tokens, TOKEN_LBRACE, TOKEN_RBRACE);
    auto *nst_class_block = new NBlock();
    stride::ast::parser::parse(*subset, *nst_class_block);
    nst_class->body = nst_class_block;
}