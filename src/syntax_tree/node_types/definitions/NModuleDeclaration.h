#ifndef STRIDE_LANGUAGE_NMODULEDECLARATION_H
#define STRIDE_LANGUAGE_NMODULEDECLARATION_H

#include "../../../tokens/TokenSet.h"
#include "NBlock.h"

class NModuleDeclaration : public stride::ast::Node
{
private:
    std::string moduleName;
    NBlock *body;

public:

    explicit NModuleDeclaration(std::string module_name) :
            moduleName(std::move(module_name)), body(nullptr)
    {}

    enum stride::ast::ENodeType getType() override
    { return stride::ast::MODULE_DECLARATION; }

    static void parse(TokenSet &tokenSet, Node &parent);

};

#endif