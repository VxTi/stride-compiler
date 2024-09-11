#ifndef STRIDE_LANGUAGE_NCLASSDECLARATION_H
#define STRIDE_LANGUAGE_NCLASSDECLARATION_H

#include "../../ASTNodes.h"
#include "../../../tokens/TokenSet.h"
#include "../../../tokens/token.h"
#include "NIdentifier.h"
#include "NBlock.h"


/**
     * Class declaration.
     * Classes are defined using the following format: <br />
     * <code>
     * class &lt;className&gt; {
     *  ... <br />
     * } <br />
     * or <br />
     * class &lt;className&gt;&lt;&lt;GENERICS&gt;&gt; : &lt;parent_class&gt;, ... {
     * </code>
     */
class NClassDeclaration : public stride::ast::Node
{
private:
    std::string className;
    std::vector<NIdentifier *> *parents;
    std::vector<NIdentifier *> *generics;
    NBlock *body;

public:

    NClassDeclaration() : className(""), parents(new std::vector<NIdentifier *>()), generics(new std::vector<NIdentifier *>()), body(nullptr)
    {}

    ~NClassDeclaration();

    /**
     * Add a parent class to the class declaration.
     * @param parent The parent class.
     */
    void addParent(NIdentifier *parent)
    {
        parents->push_back(parent);
    }

    /**
     * Add a generic to the class declaration.
     * @param generic The generic to add.
     */
    void addClassGeneric(NIdentifier *generic)
    {
        generics->push_back(generic);
    }

    enum stride::ast::ENodeType getType() override
    {
        return stride::ast::CLASS_DECLARATION;
    }

    static void parse(TokenSet &tokenSet, Node &parent);
};

#endif