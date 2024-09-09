//
// Created by Luca Warmenhoven on 27/08/2024.
//

#include "syntax_validation.h"

void stride::validation::validate(stride::ast::Node &root)
{
    // Pointless of analysing child nodes that don't have child nodes themselves.
    // Better look at the nodes from the top and see whether they're in the right order,
    // or whatever.
    if ( root.branch_count == 0)
    {
        return;
    }

    for ( int i = 0; i < root.branch_count; i++ )
    {
        switch ( root.node_type )
        {
            case NODE_TYPE_VARIABLE_REFERENCE:
            case NODE_TYPE_MODULE:
            case NODE_TYPE_CLASS:
            case NODE_TYPE_IMPORT:
            case NODE_TYPE_FUNCTION_DEFINITION:
            case NODE_TYPE_FUNCTION_CALL:
            {
                check_symbol_existence_at_access(root);
            }
            break;
            default:
                break;
        }
        validate(root.branches[ i ]);
    }
}