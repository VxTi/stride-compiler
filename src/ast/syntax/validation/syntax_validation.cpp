//
// Created by Luca Warmenhoven on 27/08/2024.
//

#include "syntax_validation.h"

void stride::ast::validation::validate(stride::ast::Node &root)
{
    // Pointless of analysing child nodes that don't have child nodes themselves.
    // Better look at the nodes from the top and see whether they're in the right order,
    // or whatever.
    if ( root.hasEmptyLeaves())
    {
        return;
    }

    for ( int i = 0; i < root.branch_count; i++ )
    {
        switch ( root.node_type )
        {
            case NODE_TYPE_VARIABLE_DECLARATION:
            {
                check_variable_redeclaration(root);
            }
                break;
            case NODE_TYPE_VARIABLE_REFERENCE:
            {
                check_variable_scope_order_accessibility(root);
            }
                break;
            default:
                break;
        }
        validate(root.branches[ i ]);
    }
}