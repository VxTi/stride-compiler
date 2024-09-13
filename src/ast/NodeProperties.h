//
// Created by Luca Warmenhoven on 11/09/2024.
//

#ifndef STRIDE_LANGUAGE_NODEPROPERTIES_H
#define STRIDE_LANGUAGE_NODEPROPERTIES_H

#include <string>
#include "../tokens/TokenSet.h"
#include "ASTNodes.h"
#include "nodes/definitions/NIdentifier.h"

namespace stride::ast
{

    /**
     * Parses a sequence that represents class/structure generics.
     * This will append the generated identifiers to the genericsDst vector.
     * @param tokenSet
     * @param genericsDst
     */
    void parseGenerics(TokenSet &tokenSet, std::vector<std::string *> &genericsDst);

    /**
     * Parses a sequence of identifiers and converts it to an internal name.
     * An example of this is as followed: <br /> <br />
     * <code>
     * let k: modulename::submodule::classname = value;
     * </code> <br /> <br />
     *
     * Here, the type of the variable <code>k</code> refers to a nested class in a module.
     * This type will be converted to the internal name <code>__modulename_submodule_classname</code>.
     * @param tokenSet
     * @param identifierDst
     */
    NIdentifier *parseIdentifier(TokenSet &tokenSet);


    /**
     * Validates whether the next token in the token set is a valid variable type.
     * These types can be stated after variable declaration, function parameters,
     * error capturing, structures and enumeration members.
     * @param tokenSet The token set to validate the type from.
     * @param identifier The identifier to validate the type for.
     * @return Whether the type is valid.
     */
    bool validateVariableType(TokenSet &tokenSet);

    /**
     * Validates whether the next token in the token set is a valid literal value.
     * These values can be used in variable declarations, function parameters, etc.
     * Literal values are values that are directly assigned to a variable,
     * such as numbers, strings, booleans, etc.
     * @param tokenSet The token set to validate the value from.
     * @return Whether the value is valid.
     */
    bool validateLiteralValue(TokenSet &tokenSet);

}


#endif //STRIDE_LANGUAGE_NODEPROPERTIES_H
