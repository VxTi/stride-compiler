#ifndef STRIDE_LANGUAGE_NENUMERABLEDECLARATION_H
#define STRIDE_LANGUAGE_NENUMERABLEDECLARATION_H

#include "../../ASTNodes.h"
#include "../../../tokens/TokenSet.h"
#include "../../../tokens/token.h"

class NEnumerableDeclaration : public stride::ast::Node
{
public:

    std::map<std::string, long int> values;

    NEnumerableDeclaration() : values()
    {};

    void addValue(const char *key, long int value)
    {
        values[ key ] = value;
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

    enum stride::ast::ENodeType getType() override
    { return stride::ast::ENUMERABLE_DECLARATION; }
};

#endif