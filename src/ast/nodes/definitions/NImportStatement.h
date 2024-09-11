#include "../../ASTNodes.h"

#include "../../../tokens/TokenSet.h"
#include "../../../tokens/token.h"

using namespace stride::ast;


/**
 * Import statement.
 * Import statements are used to import modules.
 * They are defined using the following format:
 * <code>
 * import &lt;module_name&gt;;
 * </code>
 */
class NImportStatement : public Node
{
public:
    std::string module_name;

    explicit NImportStatement(std::string module_name) :
            module_name(std::move(module_name))
    {}

    enum ENodeType getType() override
    { return IMPORT_STATEMENT; }

    static void parse(TokenSet &tokenSet, Node &parent);
};