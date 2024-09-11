#include "../../ASTNodes.h"

#include "../../../tokens/TokenSet.h"
#include "../../../tokens/token.h"
#include "NVariableDeclaration.h"

using namespace stride::ast;
/**
 * Structure declaration.
 * Structures are defined using the following format: <br />
 * <code>
 * struct &lt;structure_name&gt; {
 *  &nbsp;&lt;field&gt; ...
 *  }
 */
class NStructureDeclaration : public Node
{
private:

    std::string structure_name;
    std::vector<NVariableDeclaration *> *fields;

public:

    explicit NStructureDeclaration() : structure_name(""), fields(new std::vector<NVariableDeclaration *>())
    {}

    /**
     * Add a field to the structure declaration.
     * @param field The field to add.
     */
    void addField(NVariableDeclaration *field)
    {
        fields->push_back(field);
    }

    void setName(const char *name) {
        structure_name = name;
    }


    enum ENodeType getType() override
    {
        return STRUCTURE_DECLARATION;
    }

    static void parse(TokenSet &tokenSet, Node &parent);

};