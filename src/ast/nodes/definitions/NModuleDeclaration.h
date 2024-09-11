#include "../../ASTNodes.h"

#include "../../../tokens/TokenSet.h"
#include "../../../tokens/token.h"

class NModuleDeclaration : public stride::ast::Node
{
public:
    std::string module_name;

    explicit NModuleDeclaration(std::string module_name) :
            module_name(std::move(module_name))
    {}

    enum stride::ast::ENodeType getType() override
    { return stride::ast::MODULE_DECLARATION; }

    static void parse(TokenSet &tokenSet, Node &parent);

};