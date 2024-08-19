//
// Created by Luca Warmenhoven on 19/08/2024.
//

#include <regex.h>
#include "token.h"

/**
 * Generates a token definition from a regular expression and a token type.
 * @param expression The regular expression to match.
 * @param token The token type to assign to the matched string.
 * @return A token definition.
 */
token_def_t gen_token(const char *expression, token_type_t token)
{
    token_def_t token_def;
    regex_t regexp;
    regcomp(&regexp, expression, REG_EXTENDED);
    token_def.regex = regexp;
    token_def.token = token;
    return token_def;
}

/**
 * Token definitions for the lexer.
 */
std::vector<token_def_t> token_definitions = {
        gen_token("//.*", TOKEN_COMMENT),
        gen_token("/\\*.*\\*/", TOKEN_COMMENT_MULTILINE),
        gen_token("var", TOKEN_KEYWORD_VAR),
        gen_token("return", TOKEN_KEYWORD_RETURN),
        gen_token("if", TOKEN_KEYWORD_IF),
        gen_token("else", TOKEN_KEYWORD_ELSE),
        gen_token("elif", TOKEN_KEYWORD_ELIF),
        gen_token("true", TOKEN_KEYWORD_TRUE),
        gen_token("false", TOKEN_KEYWORD_FALSE),
        gen_token("class", TOKEN_KEYWORD_CLASS),
        gen_token("const", TOKEN_KEYWORD_CONST),
        gen_token("fn", TOKEN_KEYWORD_FN),
        gen_token("this", TOKEN_KEYWORD_THIS),
        gen_token("struct", TOKEN_KEYWORD_STRUCT),
        gen_token("import", TOKEN_KEYWORD_IMPORT),
        gen_token("namespace", TOKEN_KEYWORD_NAMESPACE),
        gen_token("ext", TOKEN_KEYWORD_EXT),
        gen_token("null", TOKEN_KEYWORD_NULL),
        gen_token("pub", TOKEN_KEYWORD_PUB),
        gen_token("override", TOKEN_KEYWORD_OVERRIDE),
        gen_token("do", TOKEN_KEYWORD_DO),
        gen_token("while", TOKEN_KEYWORD_WHILE),
        gen_token("for", TOKEN_KEYWORD_FOR),
        gen_token("try", TOKEN_KEYWORD_TRY),
        gen_token("catch", TOKEN_KEYWORD_CATCH),
        gen_token("throw", TOKEN_KEYWORD_THROW),
        gen_token("new", TOKEN_KEYWORD_NEW),
        gen_token("int8", TOKEN_PRIMITIVE_INT8),
        gen_token("int16", TOKEN_PRIMITIVE_INT16),
        gen_token("int32", TOKEN_PRIMITIVE_INT32),
        gen_token("int64", TOKEN_PRIMITIVE_INT64),
        gen_token("bool", TOKEN_PRIMITIVE_BOOL),
        gen_token("float32", TOKEN_PRIMITIVE_FLOAT32),
        gen_token("float64", TOKEN_PRIMITIVE_FLOAT64),
        gen_token("string", TOKEN_PRIMITIVE_STRING),
        gen_token("char", TOKEN_PRIMITIVE_CHAR),
        gen_token("void", TOKEN_PRIMITIVE_VOID),
        gen_token(R"([\+\-]?([0-9]+\.[eE][-+]?[0-9]+|[0-9]*\.?[0-9]+[eE][\+\-]?[0-9]+|[0-9]*\.[0-9]+|[0-9]+))",
                  TOKEN_NUMBER_LITERAL),
        gen_token(R"("[^"]*")", TOKEN_STRING_LITERAL),
        gen_token("[a-zA-Z_$][a-zA-Z0-9_$]*", TOKEN_IDENTIFIER),
        gen_token("\\.{3}", TOKEN_THREE_DOTS),
        gen_token("[", TOKEN_LBRACKET),
        gen_token("]", TOKEN_RBRACKET),
        gen_token("{", TOKEN_LBRACE),
        gen_token("}", TOKEN_RBRACE),
        gen_token("\\(", TOKEN_LPAREN),
        gen_token("\\)", TOKEN_RPAREN),
        gen_token(",", TOKEN_COMMA),
        gen_token("->", TOKEN_RARROW),
        gen_token("<-", TOKEN_LARROW),
        gen_token("<=", TOKEN_LEQUALS),
        gen_token(">=", TOKEN_GEQUALS),
        gen_token("<<", TOKEN_DOUBLE_LARROW),
        gen_token(">>", TOKEN_DOUBLE_RARROW),
        gen_token("\\|\\|", TOKEN_DOUBLE_PIPE),
        gen_token("++", TOKEN_DOUBLE_PLUS),
        gen_token("--", TOKEN_DOUBLE_MINUS),
        gen_token("**", TOKEN_DOUBLE_STAR),
        gen_token("::", TOKEN_DOUBLE_COLON),
        gen_token("==", TOKEN_DOUBLE_EQUALS),
        gen_token("&&", TOKEN_DOUBLE_AMPERSAND),
        gen_token("*=", TOKEN_STAR_EQUALS),
        gen_token("/=", TOKEN_SLASH_EQUALS),
        gen_token("%=", TOKEN_PERCENT_EQUALS),
        gen_token("&=", TOKEN_AMPERSAND_EQUALS),
        gen_token("\\|=", TOKEN_PIPE_EQUALS),
        gen_token("^=", TOKEN_CARET_EQUALS),
        gen_token("!=", TOKEN_BANG_EQUALS),
        gen_token("+=", TOKEN_PLUS_EQUALS),
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
        gen_token(">", TOKEN_GREATER),
        gen_token("<", TOKEN_LESS),
        gen_token("!=", TOKEN_NOT_EQUALS),
        gen_token("=", TOKEN_EQUALS),
        gen_token("[a-zA-Z_$][a-zA-Z0-9_$]*", TOKEN_IDENTIFIER)
};