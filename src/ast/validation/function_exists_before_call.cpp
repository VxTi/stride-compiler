//
// Created by Luca Warmenhoven on 04/09/2024.
//

#include "syntax_validation.h"
#include <iostream>

std::string get_internal_identifier_name(stride::ast::Node &node)
{
    if ( node.node_type == NODE_TYPE_IDENTIFIER)
    {
        return { (char *) node.value };
    }
    else if ( node.node_type == NODE_TYPE_IDENTIFIER_REFERENCE)
    {
        // Append all children to the string
        std::string identifier_name;
        for ( int i = 0; i < node.branch_count; i++ )
        {
            identifier_name.append("__").append(get_internal_identifier_name(node.branches[ i ]));
        }
        return identifier_name;
    }
    return "";
}

void check_parent_recursively(const std::string &internal_name, stride::ast::Node *parent, int child_index)
{
    if ( child_index == -1 || parent == nullptr )
    {
        std::cerr << "Function call (" << internal_name << ") is not declared before call" << std::endl;
        return;
    }

    for ( int i = 0; i < child_index; i++ )
    {
        if ( parent->branches[ i ].node_type == NODE_TYPE_FUNCTION_DEFINITION)
        {
            if ( parent->branches[ i ].branches[ 0 ].node_type == NODE_TYPE_IDENTIFIER_REFERENCE)
                return;
            if ( parent->branches[ i ].branches[0].node_type == NODE_TYPE_IDENTIFIER)
            {
                std::string function_name = get_internal_identifier_name(parent->branches[ i ].branches[0]);
                if ( function_name == internal_name )
                {
                    return;
                }
            }
        }
    }
    check_parent_recursively(internal_name, parent->parent, parent->parent_index);
}

void stride::validation::function_exists_before_call(stride::ast::Node &root)
{
    if ( root.node_type != NODE_TYPE_FUNCTION_CALL)
    {
        return;
    }

    std::string internal_name = get_internal_identifier_name(root.branches[0]);
    std::cout << "Checking if function \"" << internal_name << "\" is declared before call" << std::endl;

    // Check if the function is declared before the call
    check_parent_recursively(internal_name, root.parent, root.parent_index);
}
