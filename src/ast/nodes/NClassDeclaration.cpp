//
// Created by Luca Warmenhoven on 27/08/2024.
//

#include "../ASTNodes.h"
#include "../Lookahead.h"
#include "../../error/ast_error_handling.h"
#include "../NodeProperties.h"

using namespace stride::ast;

stride::ast::NClassDeclaration::~NClassDeclaration()
{
    for (auto &parent : *parents)
    {
        delete parent;
    }
    for (auto &generic : *generics)
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
void stride::ast::NClassDeclaration::parse(TokenSet &tokens, Node &parent)
{
    // First inheritance class
    auto *nst_class = new NClassDeclaration();
    nst_class->class_name = tokens.consumeRequired(TOKEN_IDENTIFIER, "Class inheritance requires parent class name.").value;

    printf("Class name: %s\n", nst_class->class_name.c_str());

    parseGenerics(tokens, *nst_class->generics);

    if (tokens.consume(TOKEN_KEYWORD_HAS))
    {
        do {
            nst_class->addParent(parseIdentifier(tokens));
        } while (tokens.consume(TOKEN_KEYWORD_AND));
    }

    auto *subset = captureBlock(tokens, TOKEN_LBRACE, TOKEN_RBRACE);
    auto *nst_class_block = new NBlock();
    parser::parse(*subset, *nst_class_block);
    nst_class->body = nst_class_block;
}