#include "../../ASTNodes.h"

#include "../../../tokens/TokenSet.h"
#include "../../../tokens/token.h"

using namespace stride::ast;

class NModuleDeclaration : public Node
{
public:
    std::string module_name;

    explicit NModuleDeclaration(std::string module_name) :
            module_name(std::move(module_name))
    {}

    enum ENodeType getType() override
    { return MODULE_DECLARATION; }

    static void parse(TokenSet &tokenSet, Node &parent);

};