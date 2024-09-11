#include "../../ASTNodes.h"

#include "../../../tokens/TokenSet.h"
#include "../../../tokens/token.h"

using namespace stride::ast;

class NEnumerableDeclaration : public Node
{
public:

    std::map<std::string, long int> values;

    NEnumerableDeclaration() : values()
    {};

    void addValue(const char *key, long int value)
    {
        values [ key ] = value;
    }

    void addValue(std::string &key, long int value)
    {
        values[ key ] = value;
    }

    /**
     * Parses an enumerable
     * @param tokenSet
     * @param parent
     */
    static void parse(TokenSet &tokenSet, Node &parent);

    enum ENodeType getType() override
    { return ENUMERABLE_DECLARATION; }
};