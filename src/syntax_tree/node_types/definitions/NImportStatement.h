#ifndef STRIDE_LANGUAGE_NIMPORTSTATEMENT_H
#define STRIDE_LANGUAGE_NIMPORTSTATEMENT_H

#include "../../ASTNodes.h"

#include "../../../tokens/TokenSet.h"
#include "../../../tokens/token.h"

/**
 * Import statement.
 * Import statements are used to import modules.
 * They are defined using the following format:
 * <code>
 * import &lt;moduleName&gt;;
 * </code>
 */
class NImportStatement : public stride::ast::Node
{
private:
    std::string importedModuleName;

public:

    explicit NImportStatement(std::string module_name) :
            importedModuleName(std::move(module_name))
    {}

    enum stride::ast::ENodeType getType() override
    {
        return stride::ast::IMPORT_STATEMENT;
    }

    static void parse(TokenSet &tokenSet, Node &parent);
};

#endif