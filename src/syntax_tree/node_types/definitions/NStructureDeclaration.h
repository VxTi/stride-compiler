#ifndef STRIDE_LANGUAGE_NSTRUCTUREDECLARATION_H
#define STRIDE_LANGUAGE_NSTRUCTUREDECLARATION_H

#include "../../ASTNodes.h"

#include "../../../tokens/TokenSet.h"
#include "../../../tokens/token.h"
#include "NVariableDeclaration.h"

/**
 * Structure declaration.
 * Structures are defined using the following format: <br />
 * <code>
 * struct &lt;name&gt; {
 *  &nbsp;&lt;field&gt; ...
 *  }
 */
class NStructureDeclaration : public stride::ast::Node
{
private:

    std::string name;
    std::vector<NVariableDeclaration *> fields = {};
    std::vector<std::string *> generics = {};

public:

    NStructureDeclaration() = default;

    /**
     * Add a field to the structure declaration.
     * @param field The field to add.
     */
    void addField(NVariableDeclaration *field)
    {
        fields.push_back(field);
    }

    void setName(const char *name)
    {
        name = name;
    }


    enum stride::ast::ENodeType getType() override
    {
        return stride::ast::STRUCTURE_DECLARATION;
    }

    static void parse(TokenSet &tokenSet, Node &parent);

};

#endif