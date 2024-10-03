//
// Created by Luca Warmenhoven on 19/08/2024.
//

#include <regex.h>
#include "token.h"

/**
 * Generates a required_token definition from a regular expression and a required_token properties.
 * @param expression The regular expression to match.
 * @param token The required_token properties to assign to the matched string.
 * @return A required_token definition.
 */
token_def_t gen_token(const char *expression, token_type_t token)
{
    token_def_t token_def;
    regex_t regexp;
    if ( regcomp(&regexp, expression, REG_EXTENDED))
    {
        fprintf(stderr, "\nFailed to compile regular expression %s\n", expression);
        exit(1);
    }
    token_def.keyword = false;
    token_def.regex = regexp;
    token_def.token = token;
    return token_def;
}

/**
 * Generates a keyword required_token.
 * This slightly alters the regular expression to work with word boundaries.
 * @param expression 
 * @param token 
 * @return 
 */
token_def_t gen_token_kw(const char *expression, token_type_t token)
{
    token_def_t token_def = gen_token(expression, token);
    token_def.keyword = true;
    return token_def;
}

/**
 * Token definitions for the tokens.
 */
std::vector<token_def_t> token_definitions = {
        gen_token("//[^\n]*", TOKEN_COMMENT),
        gen_token("/\\*.*\\*/", TOKEN_COMMENT_MULTILINE),
        gen_token_kw("public", TOKEN_KEYWORD_PUBLIC),
        gen_token_kw("module", TOKEN_KEYWORD_MODULE),
        gen_token_kw("and", TOKEN_KEYWORD_AND),
        gen_token_kw("has", TOKEN_KEYWORD_HAS),
        gen_token_kw("as", TOKEN_KEYWORD_AS),
        gen_token_kw("let", TOKEN_KEYWORD_LET),
        gen_token_kw("return", TOKEN_KEYWORD_RETURN),
        gen_token_kw("if", TOKEN_KEYWORD_IF),
        gen_token_kw("else", TOKEN_KEYWORD_ELSE),
        gen_token_kw("class", TOKEN_KEYWORD_CLASS),
        gen_token_kw("const", TOKEN_KEYWORD_CONST),
        gen_token_kw("define", TOKEN_KEYWORD_DEFINE),
        gen_token_kw("async", TOKEN_KEYWORD_ASYNC),
        gen_token_kw("this", TOKEN_KEYWORD_THIS),
        gen_token_kw("enum", TOKEN_KEYWORD_ENUM),
        gen_token_kw("switch", TOKEN_KEYWORD_SWITCH),
        gen_token_kw("case", TOKEN_KEYWORD_CASE),
        gen_token_kw("default", TOKEN_KEYWORD_DEFAULT),
        gen_token_kw("break", TOKEN_KEYWORD_BREAK),
        gen_token_kw("struct", TOKEN_KEYWORD_STRUCT),
        gen_token_kw("import", TOKEN_KEYWORD_IMPORT),
        gen_token_kw("external", TOKEN_KEYWORD_EXTERNAL),
        gen_token_kw("null", TOKEN_KEYWORD_NULL),
        gen_token_kw("override", TOKEN_KEYWORD_OVERRIDE),
        gen_token_kw("do", TOKEN_KEYWORD_DO),
        gen_token_kw("while", TOKEN_KEYWORD_WHILE),
        gen_token_kw("for", TOKEN_KEYWORD_FOR),
        gen_token_kw("try", TOKEN_KEYWORD_TRY),
        gen_token_kw("catch", TOKEN_KEYWORD_CATCH),
        gen_token_kw("throw", TOKEN_KEYWORD_THROW),
        gen_token_kw("new", TOKEN_KEYWORD_NEW),
        gen_token_kw("bool", TOKEN_PRIMITIVE_BOOL),
        gen_token_kw("string", TOKEN_PRIMITIVE_STRING),
        gen_token_kw("char", TOKEN_PRIMITIVE_CHAR),
        gen_token_kw("void", TOKEN_PRIMITIVE_VOID),
        gen_token_kw("auto", TOKEN_PRIMITIVE_AUTO),
        gen_token("i8", TOKEN_PRIMITIVE_INT8),
        gen_token("i16", TOKEN_PRIMITIVE_INT16),
        gen_token("i32", TOKEN_PRIMITIVE_INT32),
        gen_token("i64", TOKEN_PRIMITIVE_INT64),
        gen_token("f32", TOKEN_PRIMITIVE_FLOAT32),
        gen_token("f64", TOKEN_PRIMITIVE_FLOAT64),
        gen_token_kw("[a-zA-Z_$][a-zA-Z0-9_$]*", TOKEN_IDENTIFIER),
        gen_token(R"([\+\-]?[0-9]+([eE][0-9]+)?)", TOKEN_NUMBER_INTEGER),
        gen_token("true|false", TOKEN_BOOLEAN_LITERAL),
        gen_token(R"([\+\-]?([0-9]+\.[eE][-+]?[0-9]+|[0-9]*\.?[0-9]+[eE][\+\-]?[0-9]+|[0-9]*\.[0-9]+|[0-9]+))",
                  TOKEN_NUMBER_FLOAT),
        gen_token(R"("[^"]*")", TOKEN_STRING_LITERAL),
        gen_token(R"('(\\[^']|\\'|[^'])')", TOKEN_CHAR_LITERAL),
        gen_token("\\.{3}", TOKEN_THREE_DOTS),
        gen_token("\\[", TOKEN_LSQUARE_BRACKET),
        gen_token("]", TOKEN_RSQUARE_BRACKET),
        gen_token("\\{", TOKEN_LBRACE),
        gen_token("}", TOKEN_RBRACE),
        gen_token("\\(", TOKEN_LPAREN),
        gen_token("\\)", TOKEN_RPAREN),
        gen_token(",", TOKEN_COMMA),
        gen_token("->", TOKEN_DASH_RARROW),
        gen_token("<-", TOKEN_LARROW_DASH),
        gen_token("<=", TOKEN_LEQUALS),
        gen_token(">=", TOKEN_GEQUALS),
        gen_token("<<=", TOKEN_DOUBLE_LARROW_EQUALS),
        gen_token(">>=", TOKEN_DOUBLE_RARROW_EQUALS),
        gen_token("\\*\\*=", TOKEN_DOUBLE_STAR_EQUALS),
        gen_token("<<", TOKEN_DOUBLE_LARROW),
        gen_token(">>", TOKEN_DOUBLE_RARROW),
        gen_token("\\|\\|", TOKEN_DOUBLE_PIPE),
        gen_token("\\+\\+", TOKEN_DOUBLE_PLUS),
        gen_token("--", TOKEN_DOUBLE_MINUS),
        gen_token("\\*\\*", TOKEN_DOUBLE_STAR),
        gen_token("::", TOKEN_DOUBLE_COLON),
        gen_token("==", TOKEN_DOUBLE_EQUALS),
        gen_token("&&", TOKEN_DOUBLE_AMPERSAND),
        gen_token("\\*=", TOKEN_STAR_EQUALS),
        gen_token("/=", TOKEN_SLASH_EQUALS),
        gen_token("%=", TOKEN_PERCENT_EQUALS),
        gen_token("&=", TOKEN_AMPERSAND_EQUALS),
        gen_token("\\|=", TOKEN_PIPE_EQUALS),
        gen_token("\\^=", TOKEN_CARET_EQUALS),
        gen_token("!=", TOKEN_BANG_EQUALS),
        gen_token("\\+=", TOKEN_PLUS_EQUALS),
        gen_token("~=", TOKEN_TILDE_EQUALS),
        gen_token("-=", TOKEN_MINUS_EQUALS),
        gen_token(":", TOKEN_COLON),
        gen_token(";", TOKEN_SEMICOLON),
        gen_token("\\?", TOKEN_QUESTION),
        gen_token("\\*", TOKEN_STAR),
        gen_token("/", TOKEN_SLASH),
        gen_token("%", TOKEN_PERCENT),
        gen_token("&", TOKEN_AMPERSAND),
        gen_token("\\|", TOKEN_PIPE),
        gen_token("\\^", TOKEN_CARET),
        gen_token("~", TOKEN_TILDE),
        gen_token("!", TOKEN_BANG),
        gen_token("\\+", TOKEN_PLUS),
        gen_token("-", TOKEN_MINUS),
        gen_token(">", TOKEN_RARROW),
        gen_token("<", TOKEN_LARROW),
        gen_token("!=", TOKEN_NOT_EQUALS),
        gen_token("=", TOKEN_EQUALS),
        gen_token("\\.", TOKEN_DOT)
};