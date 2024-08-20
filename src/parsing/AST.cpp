//
// Created by Luca Warmenhoven on 19/08/2024.
//

#include "AST.h"

/**
 * Returns whether the AST Node is a leaf or not (no branches)
 * @return
 */
bool ASTNode::isLeaf() const
{
    return this->branch_count == 0;
}

/**
 * Checks whether the node has
 * @return
 */
bool ASTNode::hasEmptyLeaves() const
{
    if ( this->branch_count == 0 ) {
        return false;
    }

    for ( size_t i = 0; i < this->branch_count; i++ ) {
        if ( this->branches[ i ].branch_count > 0 ) {
            return false;
        }
    }
    return true;
}

/**
 * Ensures that the node has a minimum amount of branches.
 * @param minimum The minimum amount of branches.
 */
void ASTNode::ensureMinimumBranches()
{
    if ( this->branch_count + 1 >= this->branch_count_upper_limit ) {
        // prevent integer overflow
        if ( this->branch_count_upper_limit * BRANCH_GROWTH_FACTOR < 0 ) {
            fprintf(stderr, "Attempting to increase branch buffer size lead to integer overflow.");
            exit(1);
        }
        // Grows exponentially to reduce frequent resizing of buffer.
        auto *newBranches = (ASTNode *) malloc(sizeof(ASTNode) * this->branch_count_upper_limit * BRANCH_GROWTH_FACTOR);

        if (!newBranches) {
            fprintf(stderr, "Failed to allocate memory for new branches.");
            exit(1);
        }

        memcpy(newBranches, this->branches, sizeof(ASTNode) * this->branch_count);
        free(this->branches);
        this->branches = newBranches;
        this->branch_count_upper_limit *= BRANCH_GROWTH_FACTOR;
    }
}

/**
 * Adds a branch to the node.
 * @param node The node to add.
 */
void ASTNode::addBranch(ASTNode *node)
{
    this->ensureMinimumBranches();
    this->branches[ this->branch_count++ ] = *node;
}

/*
 * Constructor
 */
AST::AST(token_t &tokens, size_t token_count)
{
    this->base_token_set = (ast_token_set_t *) malloc(sizeof(ast_token_set_t));
    this->base_token_set->token_count = token_count;
    this->base_token_set->tokens = reinterpret_cast<token_t **>(&tokens);
}


/**
 * Creates a new comparative node.
 * @param left The left node of the comparison
 * @param right The right node of the comparison
 * @param comparison The comparative function.
 * @return
 */
ASTNode *AST::createComparison(ASTNode *left, ASTNode *right, token_type_t comparison)
{
    switch ( comparison ) {
        case TOKEN_LESS:
        case TOKEN_GREATER:
        case TOKEN_LEQUALS:
        case TOKEN_GEQUALS:
        case TOKEN_DOUBLE_EQUALS:
        case TOKEN_NOT_EQUALS: {
            auto node = new ASTNode((ast_node_type_t) comparison);
            node->addBranch(left);
            node->addBranch(right);
            return node;
        }
        default:
            fprintf(stderr, "Invalid comparison token\n");
            exit(1);
    }
}

/**
 * Checks if there is a next token in the token stream.
 * @return True if there is a next token, false otherwise.
 */
bool AST::hasNext(ast_token_set_t &token_set, size_t index) const
{
    return index + 1 < token_set.token_count;
}

/**
 * Whether there is a token before the current one
 * @return
 */
bool AST::hasPrevious(size_t index) const
{
    return index > 0;
}

/**
 * Returns the next token in the token stream, from
 * the current index.
 * @return
 */
token_t *AST::next(ast_token_set_t &token_set, size_t index)
{
    if ( !this->hasNext(token_set, index)) {
        return nullptr;
    }

    return token_set.tokens[ index + 1 ];
}

/**
 * Returns the previous token in the token stream, from the
 * current index.
 * @return
 */
token_t *AST::previous(ast_token_set_t &token_set, size_t index)
{
    if ( !this->hasPrevious(index)) {
        return nullptr;
    }

    return token_set.tokens[ index - 1 ];
}

/**
 * Returns the token at the nth offset from the cursor position,
 * e.g. cursor = 0, peak(1) will return the token at position 1. (cursor + n)
 * @param offset
 * @return
 */
token_t *AST::peak(ast_token_set_t &token_set, size_t index, int offset)
{
    int i = index + offset;
    if ( i < 0 || i >= token_set.token_count ) {
        return nullptr;
    }

    return token_set.tokens[ i ];
}

/**
 * Whether the previous token is of the provided type.
 * If there are no tokens left, the function will return false.
 * @param type The type to check for
 * @return
 */
bool AST::isPrevious(ast_token_set_t &token_set, token_type_t type, size_t index)
{
    if ( !hasPrevious(index)) {
        return false;
    }

    return this->previous(token_set, index)->type == type;
}

/**
 * Whether the next token is of the provided type.
 * If there are no tokens left, the function will return false.
 * @param type The type to check for
 * @return
 */
bool AST::isNext(ast_token_set_t &token_set, token_type_t type, size_t index)
{
    if ( !hasNext(token_set, index)) {
        return false;
    }

    return this->next(token_set, index)->type == type;
}

/**
 * Checks whether the provided type is within the provided range,
 * from the current index. If one wants to check if there's a token of a specific type
 * within the next -3n || +3n of the cursor, this function is yours.
 * If range = 0, the function will check whether the current token is of the provided type.
 * @param type The type to check
 * @param range The offset to provide
 * @return
 */
bool AST::isInRange(ast_token_set_t &token_set, token_type_t type, size_t index, int range)
{
    if ( range == 0 ) {
        return token_set.tokens[ index ]->type == type;
    }

    long start = index;
    long end = index + range;


    // Swap range if range is negative
    if ( start > end ) {
        std::swap(start, end);
    }

    // Check whether token type is in range
    for ( size_t i = start; i < end; i++ ) {
        if ( token_set.tokens[ i ]->type == type ) {
            return true;
        }
    }
    return false;
}

/**
 * Whether the provided token type is an acceptable
 * variable type. This is only true for primitives and identifiers.
 * @param type The type to check.
 * @return
 */
bool AST::isValidType(token_type_t type)
{
    switch ( type ) {
        case TOKEN_IDENTIFIER:
        case TOKEN_PRIMITIVE_INT8:
        case TOKEN_PRIMITIVE_INT16:
        case TOKEN_PRIMITIVE_INT32:
        case TOKEN_PRIMITIVE_INT64:
        case TOKEN_PRIMITIVE_FLOAT32:
        case TOKEN_PRIMITIVE_FLOAT64:
        case TOKEN_PRIMITIVE_BOOL:
        case TOKEN_PRIMITIVE_STRING:
        case TOKEN_PRIMITIVE_CHAR:
            return true;
        default:
            return false;
    }
}

/**
 * Exits the program with an error message.
 * @param errorMessage The error message to display.
 */
void AST::error(char *errorMessage, ...)
{
    va_list args;
    va_start(args, errorMessage);
    vfprintf(stderr, errorMessage, args);
    va_end(args);
    exit(1);
}

/**
 * Exits the program if the next token is not of a certain type.
 * @param type The type to check for.
 * @param offset The offset to check.p
 * @param errorMessage The error message to display.
 */
void AST::requiresAt(token_type_t type, ast_token_set_t &token_set, size_t index, char *error_message, ...)
{
    if ( index >= token_set.token_count || token_set.tokens[ index ]->type != type ) {

        va_list args;
        va_start(args, error_message);

        fprintf(stderr, "Error at line %d column %d\n", token_set.tokens[ index ]->line,
                token_set.tokens[ index ]->column);
        vfprintf(stderr, error_message, args);
        va_end(args);
        exit(1);
    }
}

/**
 * Parses a partial block of code into an abstract syntax tree.
 * This function will recursively parse the block and its children.
 * @param partial_tokens The token stream.
 * @param partial_token_count The number of partial_tokens in the stream.
 * @param index The current index.
 * @return The parsed bracket node.
 */
ASTNode *AST::parsePartial(ASTNode *root, ast_token_set_t &token_set)
{
    size_t i, j, k;

    printf("Parsing partial %zu\n", token_set.token_count);

    for ( i = 0; i < token_set.token_count; i++ ) {
        switch ( token_set.tokens[ i ]->type ) {

            // Function definitions must follow the format 'fn <name>( ... )' where ... = param: (...)type([]), ...
            case TOKEN_KEYWORD_FN: {
                if ( !hasNext(token_set, i)) {
                    this->error("Expected function name, but received none.");
                }

                this->requiresAt(TOKEN_IDENTIFIER, token_set, i + 1,"Expected function name");
                this->requiresAt(TOKEN_LPAREN, token_set, i + 2, "Expected opening parenthesis");

                std::vector<ast_function_param_t> parameters;

                printf("Found function declaration");

                // Move forward until we find a closing parenthesis
                for ( j = i + 3; j < token_set.token_count; j++ ) {
                    if ( token_set.tokens[ j ]->type == TOKEN_RPAREN ) {
                        break;
                    }

                    ast_function_param_t parameter;

                    // If there's a 'const' keyword before the parameter declaration,
                    // we'll move the cursor and declare it immutable.
                    if ( peak(token_set, j, 0)->type == TOKEN_KEYWORD_CONST ) {
                        parameter.immutable = true;
                        j++;
                    }

                    // Check whether the identifier is present at the required location
                    this->requiresAt(TOKEN_IDENTIFIER, token_set, j,
                                     "Expected parameter name, but received %s",
                                     token_set.tokens[ j ]->value);

                    // Check whether the colon is also present
                    this->requiresAt(TOKEN_COLON, token_set, j + 1,
                                     "Expected colon after parameter, but received %s",
                                     token_set.tokens[ j + 1 ]->value);

                    parameter.name = (char *) token_set.tokens[ j ]->value;

                    if ( this->peak(token_set, j, 2)->type == TOKEN_THREE_DOTS &&
                         this->isValidType(this->peak(token_set, j, 3)->type)) {
                        parameter.array = true;
                        parameter.name = (char *) this->peak(token_set, j, 3)->value;
                    }
                    else if ( this->isValidType(this->peak(token_set, j, 2)->type)) {
                        parameter.type = this->peak(token_set, j, 2)->type;
                    }
                    else {
                        this->error("Received invalid type after token declaration at line %d column %d: %s",
                                    token_set.tokens[ j + 2 ]->line, token_set.tokens[ j + 2 ]->column,
                                    token_set.tokens[ j + 2 ]->value);
                    }
                    parameters.push_back(parameter);
                }

                // Create a function node with the required information
                auto *function_node = (ast_function_node_t *) malloc(sizeof(ast_function_node_t));

                function_node->function_name = token_set.tokens[ i + 1 ]->value;
                function_node->parameter_count = parameters.size();
                function_node->parameters = parameters.data();
                function_node->publicly_visible = ( i > 0 && token_set.tokens[ i - 1 ]->type == TOKEN_KEYWORD_PUB ) ||
                                                  ( i > 1 && token_set.tokens[ i - 2 ]->type == TOKEN_KEYWORD_PUB );

                function_node->external = ( i > 0 && token_set.tokens[ i - 1 ]->type == TOKEN_KEYWORD_EXT ) ||
                                          ( i > 1 && token_set.tokens[ i - 2 ]->type == TOKEN_KEYWORD_EXT );

                auto *returnNode = new ASTNode(RETURN);
                auto *contentNode = new ASTNode(BLOCK);
                auto *fnRoot = new ASTNode(FUNCTION_DEF);
                fnRoot->addBranch(contentNode);
                fnRoot->addBranch(returnNode);
                fnRoot->setValue(function_node);
                root->addBranch(fnRoot);
            }
                break;
            case TOKEN_KEYWORD_VAR: {
                this->requiresAt(TOKEN_IDENTIFIER, token_set, i + 1,
                                 "Expected variable name, but received %s",
                                 token_set.tokens[ i + 1 ]->value);

                auto declaration = (ast_declaration_node_t *) malloc(sizeof(ast_declaration_node_t));
                declaration->variable_name = token_set.tokens[ i + 1 ]->value;
                declaration->immutable = this->isPrevious(token_set, TOKEN_KEYWORD_CONST, i);

                // Acquire variable type
                // We require a colon and a type after variable declaration.
                if ( this->isNext(token_set, TOKEN_COLON, i + 1)
                     && this->isValidType(token_set.tokens[ i + 2 ]->type)) {
                    declaration->variable_type = token_set.tokens[ i + 2 ]->value;
                    i += 2;
                }
                else {
                    this->error("\nError at line %d column %d -- Expected type name for variable declaration\n",
                                token_set.tokens[ i + 3 ]->line, token_set.tokens[ i + 3 ]->column);
                }

                auto *declNode = new ASTNode(DECLARATION);

                // Find position of the semicolon
                for ( j = 0, k = 0; j < token_set.token_count; j++ ) {
                    if ( token_set.tokens[ j ]->type == TOKEN_SEMICOLON ) {
                        k = 1;
                        break;
                    }
                }
                // If no semicolon found, throw error.
                if ( !k ) {
                    this->error("Expected semicolon after variable declaration at line %d column %d\n",
                                token_set.tokens[ i ]->line, token_set.tokens[ i ]->column);
                }

                // The first node is the one containing information about the variable
                auto nameNode = new ASTNode(IDENTIFIER);
                nameNode->setValue((void *) declaration);
                declNode->addBranch(nameNode);

                // Check whether there's variable assignment
                if ( this->isNext(token_set, TOKEN_EQUALS, i)) {
                    // we'll have to parse the value of the expression as a sub-expression,
                    // ... recursion
                    // Now we'll have to call this function with a sub
                    // array from i to j
                    auto *subset = (ast_token_set_t *) malloc(sizeof(ast_token_set_t));
                    subset->token_count = j - i;
                    subset->tokens = token_set.tokens + i;
                    parsePartial(declNode, *subset);
                }
                root->addBranch(declNode);
            }
                break;
            case TOKEN_IDENTIFIER: {
                auto *identifierNode = new ASTNode(IDENTIFIER);
                identifierNode->setValue((void *) token_set.tokens[ i ]->value);
                root->addBranch(identifierNode);
            }
                break;
            case TOKEN_KEYWORD_NAMESPACE: {
                this->requiresAt(TOKEN_IDENTIFIER, token_set, i + 1,"Expected namespace name");

                auto *namespaceNode = new ASTNode(STRUCTURE);
                namespaceNode->setValue((void *) token_set.tokens[ i + 1 ]->value);
                root->addBranch(namespaceNode);
            }
                break;
        }
    }
}

void print_type(ASTNode &node) {
    switch (node.type) {
        case BLOCK:
            printf("BLOCK\n");
            break;
        case FUNCTION_DEF:
            printf("FUNCTION_DEF\n");
            break;
        case DECLARATION:
            printf("DECLARATION\n");
            break;
        case IDENTIFIER:
            printf("IDENTIFIER\n");
            break;
        case RETURN:
            printf("RETURN\n");
            break;
        case STRUCTURE:
            printf("STRUCTURE\n");
            break;
        default:
            printf("UNKNOWN\n");
            break;
    }
}

void recursive_print(ASTNode &node, int depth) {
    if ( depth > 0 ) printf("|");
    for (int i = 0; i < depth; i++) {
        printf("_");
    }
    print_type(node);

    for ( size_t i = 0; i < node.branch_count; i++ ) {
        recursive_print(node.branches[i], depth + 1);
    }

}

/**
 * Parses the token stream into an abstract syntax tree.
 * @return An AST Node.
 */
ASTNode *AST::parse()
{
    auto *root = new ASTNode(BLOCK);
    this->parsePartial(root, reinterpret_cast<ast_token_set_t &>(this->base_token_set));

    recursive_print(*root, 0);

    return root;
}