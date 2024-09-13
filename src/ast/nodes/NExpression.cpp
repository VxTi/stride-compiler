//
// Created by Luca Warmenhoven on 11/09/2024.
//

#include "../ASTNodes.h"
#include "definitions/NExpression.h"
#include "../Lookahead.h"
#include "../NodeProperties.h"
#include "definitions/NFunctionCall.h"
#include <queue>

bool isValidExpressionToken(TokenSet &set)
{
    switch ( set.current().type )
    {
        case TOKEN_KEYWORD_NULL:
        case TOKEN_COMMA:
        case TOKEN_IDENTIFIER:
        case TOKEN_STRING_LITERAL:
        case TOKEN_NUMBER_INTEGER:
        case TOKEN_NUMBER_FLOAT:
        case TOKEN_BOOLEAN_LITERAL:
        case TOKEN_CHAR_LITERAL:
        case TOKEN_LPAREN:
        case TOKEN_RPAREN:
        case TOKEN_LSQUARE_BRACKET:
        case TOKEN_RSQUARE_BRACKET:
        case TOKEN_STAR:
        case TOKEN_SLASH:
        case TOKEN_PLUS:
        case TOKEN_MINUS:
        case TOKEN_DOUBLE_STAR:
        case TOKEN_DOUBLE_RARROW:
        case TOKEN_DOUBLE_LARROW:
        case TOKEN_DOUBLE_PIPE:
        case TOKEN_DOUBLE_AMPERSAND:
        case TOKEN_CARET:
        case TOKEN_AMPERSAND:
        case TOKEN_TILDE:
        case TOKEN_PIPE:
        case TOKEN_BANG:
        case TOKEN_PERCENT:
        case TOKEN_STAR_EQUALS:
        case TOKEN_SLASH_EQUALS:
        case TOKEN_PLUS_EQUALS:
        case TOKEN_MINUS_EQUALS:
        case TOKEN_AMPERSAND_EQUALS:
        case TOKEN_PERCENT_EQUALS:
        case TOKEN_BANG_EQUALS:
        case TOKEN_PIPE_EQUALS:
        case TOKEN_TILDE_EQUALS:
        case TOKEN_DOUBLE_LARROW_EQUALS:
        case TOKEN_DOUBLE_RARROW_EQUALS:
        case TOKEN_DOUBLE_STAR_EQUALS:
        case TOKEN_CARET_EQUALS:
        case TOKEN_DOUBLE_PLUS:
        case TOKEN_DOUBLE_MINUS:
        case TOKEN_LARROW:
        case TOKEN_RARROW:
        case TOKEN_LEQUALS:
        case TOKEN_GEQUALS:
        case TOKEN_DOUBLE_EQUALS:
        case TOKEN_NOT_EQUALS:
        case TOKEN_EQUALS:
            return true;
        default:
            return false;
    }
}


#define ASSOCIATIVITY_NONE      (0)
#define ASSOCIATIVITY_LEFT      (1)
#define ASSOCIATIVITY_RIGHT     (2)
#define ASSOCIATIVITY_OPTIONAL  (4) // Meaning the associativity is optional

/**
 * A segment of an expression.
 */
struct expression_segment_t
{

    /**
     * The associativity of the operator.
     * This determines whether it is required for a variable (keyword)
     * to be on the left or right side of the operator, both or neither.
     */
    int associativity;

    /**
     * The precedence of the operator.
     * This is used to determine the order of operations.
     */
    int precedence;
};

const std::map<token_type_t, struct expression_segment_t> tokenPrecedenceMap = {
        /* Enclosing tokens  */
        { TOKEN_LPAREN,               { .associativity = ASSOCIATIVITY_NONE, .precedence = 100 }}, // (
        { TOKEN_RPAREN,               { .associativity = ASSOCIATIVITY_NONE, .precedence = 100 }}, // )
        { TOKEN_LSQUARE_BRACKET,      { .associativity = ASSOCIATIVITY_NONE, .precedence = 100 }}, // [
        { TOKEN_RSQUARE_BRACKET,      { .associativity = ASSOCIATIVITY_NONE, .precedence = 100 }}, // ]

        /* High precedence operators */
        { TOKEN_STAR,                 { .associativity = ASSOCIATIVITY_NONE, .precedence = 50 }}, // *
        { TOKEN_SLASH,                { .associativity = ASSOCIATIVITY_NONE, .precedence = 50 }}, // /
        { TOKEN_PLUS,                 { .associativity = ASSOCIATIVITY_RIGHT | ASSOCIATIVITY_OPTIONAL, .precedence = 45,  }}, // + (unary and binary)
        { TOKEN_MINUS,                { .associativity = ASSOCIATIVITY_RIGHT | ASSOCIATIVITY_OPTIONAL, .precedence = 45,  }}, // - (unary and binary)
        { TOKEN_PERCENT,              { .associativity = ASSOCIATIVITY_NONE, .precedence = 40 }}, // %
        { TOKEN_DOUBLE_STAR,          { .associativity = ASSOCIATIVITY_NONE, .precedence = 40 }}, // **
        /* Bitwise operators */
        { TOKEN_DOUBLE_RARROW,        { .associativity = ASSOCIATIVITY_LEFT, .precedence = 30 }}, // >>
        { TOKEN_DOUBLE_LARROW,        { .associativity = ASSOCIATIVITY_LEFT, .precedence = 30 }}, // <<
        { TOKEN_DOUBLE_PIPE,          { .associativity = ASSOCIATIVITY_LEFT, .precedence = 30 }}, // ||
        { TOKEN_DOUBLE_AMPERSAND,     { .associativity = ASSOCIATIVITY_LEFT, .precedence = 30 }}, // &&
        { TOKEN_CARET,                { .associativity = ASSOCIATIVITY_LEFT, .precedence = 30 }}, // ^
        { TOKEN_AMPERSAND,            { .associativity = ASSOCIATIVITY_LEFT, .precedence = 30 }}, // &
        { TOKEN_TILDE,                { .associativity = ASSOCIATIVITY_LEFT, .precedence = 30 }}, // ~
        { TOKEN_PIPE,                 { .associativity = ASSOCIATIVITY_LEFT, .precedence = 30 }},// |
        { TOKEN_BANG,                 { .associativity = ASSOCIATIVITY_LEFT, .precedence = 30 }}, // !
        { TOKEN_PERCENT,              { .associativity = ASSOCIATIVITY_LEFT, .precedence = 30 }}, // %

        /* Left associative operators */
        { TOKEN_STAR_EQUALS,          { .associativity = ASSOCIATIVITY_LEFT, .precedence = 50 }},  // *=
        { TOKEN_SLASH_EQUALS,         { .associativity = ASSOCIATIVITY_LEFT, .precedence = 50 }},  // /=
        { TOKEN_PLUS_EQUALS,          { .associativity = ASSOCIATIVITY_LEFT, .precedence = 45 }},  // +=
        { TOKEN_MINUS_EQUALS,         { .associativity = ASSOCIATIVITY_LEFT, .precedence = 45 }},  // -=
        { TOKEN_AMPERSAND_EQUALS,     { .associativity = ASSOCIATIVITY_LEFT, .precedence = 45 }},  // &=
        { TOKEN_PERCENT_EQUALS,       { .associativity = ASSOCIATIVITY_LEFT, .precedence = 45 }},  // %=
        { TOKEN_BANG_EQUALS,          { .associativity = ASSOCIATIVITY_LEFT, .precedence = 45 }},  // !=
        { TOKEN_PIPE_EQUALS,          { .associativity = ASSOCIATIVITY_LEFT, .precedence = 45 }},  // |=
        { TOKEN_TILDE_EQUALS,         { .associativity = ASSOCIATIVITY_LEFT, .precedence = 45 }},  // ~=
        { TOKEN_DOUBLE_LARROW_EQUALS, { .associativity = ASSOCIATIVITY_LEFT, .precedence = 45 }},  // <<=
        { TOKEN_DOUBLE_RARROW_EQUALS, { .associativity = ASSOCIATIVITY_LEFT, .precedence = 45 }},  // >>=
        { TOKEN_DOUBLE_STAR_EQUALS,   { .associativity = ASSOCIATIVITY_LEFT, .precedence = 50 }},  // **=
        { TOKEN_CARET_EQUALS,         { .associativity = ASSOCIATIVITY_LEFT, .precedence = 45 }}, //  ^=


        { TOKEN_DOUBLE_PLUS,   { .associativity = ASSOCIATIVITY_LEFT |
                                                  ASSOCIATIVITY_RIGHT, .precedence = 55 }}, // ++
        { TOKEN_DOUBLE_MINUS,  { .associativity = ASSOCIATIVITY_LEFT |
                                                  ASSOCIATIVITY_RIGHT, .precedence = 55 }}, // --

        /* Comparison operators */
        { TOKEN_LARROW,        { .associativity = ASSOCIATIVITY_NONE, .precedence = 10 }}, // <
        { TOKEN_RARROW,        { .associativity = ASSOCIATIVITY_NONE, .precedence = 10 }}, // >
        { TOKEN_LEQUALS,       { .associativity = ASSOCIATIVITY_NONE, .precedence = 10 }}, // <=
        { TOKEN_GEQUALS,       { .associativity = ASSOCIATIVITY_NONE, .precedence = 10 }}, // >=
        { TOKEN_DOUBLE_EQUALS, { .associativity = ASSOCIATIVITY_NONE, .precedence = 10 }}, // ==
        { TOKEN_NOT_EQUALS,    { .associativity = ASSOCIATIVITY_NONE, .precedence = 10 }}, // !=
        { TOKEN_EQUALS,        { .associativity = ASSOCIATIVITY_NONE, .precedence = 10 }}, // = (assignment)
};

/**
 * Parses an identifier.
 * This function can parse both regular identifiers and function calls.
 * @param tokenSet The token set to parse.
 */
stride::ast::Node *parseIdentifier(TokenSet &tokenSet)
{
    printf("[NExpression] Parsing identifier.\n");
    // Parses an identifier; accepts nested::identifiers
    auto identifier = stride::ast::parseIdentifier(tokenSet);

    if ( tokenSet.canConsume(TOKEN_LPAREN))
    {
        auto argumentsSubset = stride::ast::captureBlock(tokenSet, TOKEN_LPAREN, TOKEN_RPAREN);

        printf("[NExpression] Parsing function call.\n");
        auto *functionCall = new NFunctionCall();
        functionCall->functionName = &identifier->name;

        if ( argumentsSubset->size() > 0 )
        {
            printf("[NExpression] Parsing function call argument, next token: %s.\n", tokenSet.current().value);
            do
            {
                printf("[NExpression] Parsing function call argument.\n");
                functionCall->addChild(NExpression::parse(*argumentsSubset, false));

            } while ( tokenSet.canConsume(TOKEN_COMMA));
        }
        return functionCall;
    }
    return identifier;
}

NExpression *NExpression::parse(TokenSet &tokenSet, bool explicitExpression)
{
    auto nstExpression = new NExpression();

    printf("[NExpression] Parsing expression.\n");

    if ( explicitExpression && !tokenSet.canConsume(TOKEN_IDENTIFIER))
    {
        tokenSet.error("Explicit expression must either be a function call or variable modification.");
        return nullptr;
    }

    int sqBrace = 0,
            brackets = 0,
            parens = 0;

    // Queues for shunting yard.
    std::queue<token_t> operatorQueue;
    std::queue<token_t> literalQueue;

    while ( !tokenSet.end())
    {
        printf("[NExpression] Parsing expression token: %s.\n", tokenSet.current().value);
        if ( !isValidExpressionToken(tokenSet))
        {
            tokenSet.error("Invalid token in expression.");
            return nullptr;
        }

        if (( tokenSet.consume(TOKEN_SEMICOLON) ||
              ( tokenSet.consume(TOKEN_COMMA)) && !parens && !sqBrace && !brackets ))
        {
            break;
        }


        switch ( tokenSet.current().type )
        {
            case TOKEN_IDENTIFIER:
                nstExpression->addChild(parseIdentifier(tokenSet));
                break;
            case TOKEN_LPAREN:
                parens++;
                break;
            case TOKEN_RPAREN:
                parens--;
                break;
            case TOKEN_LSQUARE_BRACKET:
                sqBrace++;
                break;
            case TOKEN_RSQUARE_BRACKET:
                sqBrace--;
                break;
            case TOKEN_LBRACE:
                brackets++;
                break;
            case TOKEN_RBRACE:
                brackets--;
                break;
            case TOKEN_BOOLEAN_LITERAL:
            case TOKEN_STRING_LITERAL:
            case TOKEN_CHAR_LITERAL:
            case TOKEN_NUMBER_INTEGER:
            case TOKEN_NUMBER_FLOAT:
            default:
                break;
        }

        tokenSet.next();
    }

    tokenSet.consume(TOKEN_SEMICOLON); // If there's a trailing token remaining, eat it up.

    return nstExpression;
}

void NExpression::parse(TokenSet &tokenSet, Node &parent)
{
    parent.addChild(NExpression::parse(tokenSet, true));
}

NExpression *NExpression::captureParenthesis(TokenSet &tokenSet)
{
    auto expressionSubset = stride::ast::captureBlock(tokenSet, TOKEN_LPAREN, TOKEN_RPAREN);

    if ( expressionSubset == nullptr )
    {
        tokenSet.error("Expected opening and closing brackets after 'while' keyword.");
        return nullptr;
    }

    return NExpression::parse(*expressionSubset, false);
}

bool NExpression::isReducible()
{
    // The expression is reducible if it contains a reducible child,
    // which has to be an expression.
    for ( auto child: this->getChildren())
    {
        if ( child->getType() == stride::ast::EXPRESSION )
        {
            if ( dynamic_cast<NExpression *>(child)->isReducible())
            {
                return true;
            }
        }
    }

    // The expression is reducible if it has one child that's a literal (value)
    // or an identifier (variable reference)
    return this->getChildren().size() == 1 &&
           ( this->getChildren().at(0)->getType() == stride::ast::LITERAL ||
             this->getChildren().at(0)->getType() == stride::ast::IDENTIFIER );
}

/**
 * Reduction of expressions.
 *
 * A visual example of node reduction is given below: <br />
 *
 *        (reducible)                     (reducible)                 (irreducible)
 *       EXPRESSION (+)                  EXPRESSION (+)
 *      /         \              ->       /       \              ->   LITERAL(num)
 * LITERAL(num) EXPRESSION (*)      LITERAL(num)  LITERAL(num)
 *               /      \
 *       LITERAL(num)   LITERAL(num)
 *
 * Expressions are reducible if one if their children is reducible.
 * In the example given above, we start with an expression composed of another expression and a literal
 * This literal can be reduced, therefore the parent can too. This will recursively continue until the parent node
 * is irreducible, e.g. when the parent node is returned as a literal, or the children are irreducible.
 */
std::variant<NExpression *, NLiteral *> NExpression::reduce()
{
    // If the expression is not reducible, return itself.
    if ( !isReducible())
    {
        return this;
    }

    // If the expression is reducible and has one child that's a literal, convert
    // the expression to a literal.
    if ( this->getChildren().size() == 1 )
    {
        switch ( this->getChildren().at(0)->getType())
        {
            // If the expression is composed of a single literal child,
            // we return the child as reduced node.
            case stride::ast::LITERAL:
                return dynamic_cast<NLiteral *>(this->getChildren().at(0));

                // If the expression is composed of a single expression child,
                // we return the reduced child, if it can be reduced.
            case stride::ast::EXPRESSION:
                return dynamic_cast<NExpression *>(this->getChildren().at(0))->reduce();

                // If the expression is composed of a single non-literal, non-expression child,
                // we return the expression itself. This can be the case if the expression is
                // composed of a single identifier, for example.
            default:
                return this;
        }
    }

    // If the expression is reducible and has a reducible child, reduce the child.

    auto children = this->getChildren();
    for ( int i = 0; i < children.size(); i++ )
    {
        // Skip non-expression entries that aren't reducible.
        if ( children.at(i)->getType() != stride::ast::EXPRESSION ||
             !dynamic_cast<NExpression *>(children.at(i))->isReducible())
        {
            continue;
        }

        // If the expression is reducible, we replace it
        // with its reduced version.
        auto expression = dynamic_cast<NExpression *>(children.at(i));
        if ( expression->isReducible())
        {
            auto reduced = expression->reduce();
            children[ i ] = std::get<NLiteral *>(reduced);
        }
    }
    if ( this->isReducible())
    {
        return reduce();
    }
    return this;
}