//
// Created by Luca Warmenhoven on 04/09/2024.
//

#include "syntax_validation.h"
#include <iostream>

#define EXIT_IF_EXISTS 1
#define EXIT_IF_NOT_EXISTS 2

/**
 * Returns the name of the provided node.
 * This function only works with nodes of type IDENTIFIER
 * or IDENTIFIER_REFERENCE, exists otherwise.
 */
std::string get_symbol_name(stride::ast::Node &node)
{
    if ( node.node_type == NODE_TYPE_IDENTIFIER)
    {
        std::string prefix("__");
        prefix.append((char *) node.value);
        return prefix;
    }
    else if ( node.node_type == NODE_TYPE_IDENTIFIER_REFERENCE)
    {
        std::string identifier_name;
        for ( int i = 0; i < node.branch_count; i++ )
        {
            identifier_name.append("__").append((char *) node.branches[ i ].value);
        }
        return identifier_name;
    }
    else
    {
        std::cout << "Unable to get symbol name from node" << std::endl;
        exit(1);
    }
}

void symbol_exists(const std::string &symbol_name, stride::ast::Node *node, int child_index, int action, int symbol_type)
{
    bool exists = false;
    if ( node != nullptr && node->parent != nullptr && node->parent->branch_count > 0 )
    {
        printf("Checking for symbol existence '%s'\n", get_symbol_name(node->branches[ 0 ]).c_str());
        for ( int i = 0; i < child_index && !exists; i++ )
        {
            switch ( node->branches[ i ].node_type )
            {
                case NODE_TYPE_VARIABLE_REFERENCE:
                case NODE_TYPE_MODULE:
                case NODE_TYPE_CLASS:
                case NODE_TYPE_IMPORT:
                case NODE_TYPE_FUNCTION_DEFINITION:
                case NODE_TYPE_FUNCTION_CALL:
                    if ( get_symbol_name(node->branches[ 0 ]) == symbol_name )
                    {
                        printf("Found symbol name: %s\n", symbol_name.c_str());
                        exists = true;
                    }
                    printf("Symbol name: %s\n", get_symbol_name(node->branches[ 0 ]).c_str());
                    break;
                default:
                    break;
            }
            if ( node->parent == nullptr )
            {
                break;
            }
            else
            { symbol_exists(symbol_name, node->parent, node->parent_index, action, symbol_type); }
        }
    } else {
        printf("No branches found\n");
    }
    switch ( action )
    {
        case EXIT_IF_NOT_EXISTS:
        {
            if ( !exists )
            {
                printf("\e[31mLabel '%s' was not previously declared.\e[0m\n", symbol_name.c_str());
                //exit(1);
            }
            else
            {
                printf("\e[38;5;7mLabel '%s' was previously declared.\e[0m", symbol_name.c_str());
            }
            stride::ast::Node::print_static(symbol_type);
            break;
        }
        case EXIT_IF_EXISTS:
        {
            if ( exists )
            {
                printf("Label '%s' was already previously defined.\n", symbol_name.c_str());
                stride::ast::Node::print_static(symbol_type);
                //exit(1);
            }
        }
            break;
        default:
            break;
    }
}

void stride::validation::check_symbol_existence_at_access(stride::ast::Node &root)
{
    if ( root.branch_count <= 0 )
    {
        return;
    }
    switch ( root.node_type )
    {
        // Search for previous definitions, exit if exists
        case NODE_TYPE_FUNCTION_DEFINITION:
        case NODE_TYPE_IMPORT:
        {
            symbol_exists(get_symbol_name(root.branches[ 0 ]), root.parent, root.parent_index, EXIT_IF_EXISTS, root.node_type);
        }
            break;
            // Search for definition, exit if not exists
        case NODE_TYPE_FUNCTION_CALL:
        {
            symbol_exists(get_symbol_name(root.branches[ 0 ]), root.parent, root.parent_index, EXIT_IF_NOT_EXISTS, root.node_type);
        }
            break;
        default:
            return;
    }
}