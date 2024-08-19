//
// Created by Luca Warmenhoven on 16/08/2024.
//

#ifndef STRIDE_LANGUAGE_TOKEN_H
#define STRIDE_LANGUAGE_TOKEN_H

#include <vector>
#include <regex.h>

typedef enum
{
    TOKEN_COMMENT = 0,
    TOKEN_COMMENT_MULTILINE,

    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_LBRACKET,
    TOKEN_RBRACKET,
    TOKEN_COMMA,
    TOKEN_LARROW,
    TOKEN_RARROW,
    TOKEN_GREATER,
    TOKEN_LESS,
    TOKEN_EQUALS,
    TOKEN_DOUBLE_EQUALS,
    TOKEN_NOT_EQUALS,
    TOKEN_LEQUALS,
    TOKEN_GEQUALS,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_STAR,
    TOKEN_SLASH,
    TOKEN_PERCENT,
    TOKEN_AMPERSAND,
    TOKEN_PIPE,
    TOKEN_CARET,
    TOKEN_TILDE,
    TOKEN_BANG,
    TOKEN_QUESTION,
    TOKEN_STAR_EQUALS,
    TOKEN_SLASH_EQUALS,
    TOKEN_PERCENT_EQUALS,
    TOKEN_PLUS_EQUALS,
    TOKEN_MINUS_EQUALS,
    TOKEN_AMPERSAND_EQUALS,
    TOKEN_PIPE_EQUALS,
    TOKEN_CARET_EQUALS,
    TOKEN_BANG_EQUALS,
    TOKEN_DOUBLE_LARROW,
    TOKEN_DOUBLE_RARROW,
    TOKEN_DOUBLE_PIPE,
    TOKEN_DOUBLE_AMPERSAND,
    TOKEN_DOUBLE_PLUS,
    TOKEN_DOUBLE_MINUS,
    TOKEN_DOUBLE_STAR,
    TOKEN_DOUBLE_COLON,
    TOKEN_SEMICOLON,
    TOKEN_COLON,
    TOKEN_THREE_DOTS,

    /* Primitives */
    TOKEN_PRIMITIVE_INT8,
    TOKEN_PRIMITIVE_INT16,
    TOKEN_PRIMITIVE_INT32,
    TOKEN_PRIMITIVE_INT64,

    TOKEN_PRIMITIVE_FLOAT32,
    TOKEN_PRIMITIVE_FLOAT64,

    TOKEN_PRIMITIVE_BOOL,
    TOKEN_PRIMITIVE_STRING,
    TOKEN_PRIMITIVE_CHAR,
    TOKEN_PRIMITIVE_VOID,

    /* Keywords */
    TOKEN_KEYWORD_FN,
    TOKEN_KEYWORD_VAR,
    TOKEN_KEYWORD_DO,
    TOKEN_KEYWORD_WHILE,
    TOKEN_KEYWORD_FOR,
    TOKEN_KEYWORD_TRY,
    TOKEN_KEYWORD_CATCH,
    TOKEN_KEYWORD_THROW,
    TOKEN_KEYWORD_NEW,
    TOKEN_KEYWORD_RETURN,
    TOKEN_KEYWORD_IF,
    TOKEN_KEYWORD_ELSE,
    TOKEN_KEYWORD_ELIF,
    TOKEN_KEYWORD_TRUE,
    TOKEN_KEYWORD_FALSE,
    TOKEN_KEYWORD_CLASS,
    TOKEN_KEYWORD_THIS,
    TOKEN_KEYWORD_STRUCT,
    TOKEN_KEYWORD_IMPORT,
    TOKEN_KEYWORD_NAMESPACE,
    TOKEN_KEYWORD_EXT,
    TOKEN_KEYWORD_NULL,
    TOKEN_KEYWORD_OVERRIDE,
    TOKEN_KEYWORD_PUB,

    TOKEN_STRING_LITERAL,
    TOKEN_NUMBER_LITERAL,
    TOKEN_IDENTIFIER,
} token_type_t;

/**
 * Definition of a token.
 * This includes the regular expression that has to match,
 * and the reference token enumerable.
 */
typedef struct
{
    token_type_t token;
    regex_t regex;
    int column;
    int line;
} token_def_t;

/**
 * Struct that represents a matched token.
 * This includes the type of the token, the value of the token,
 * the length of the value, the line number and the column number.
 */
typedef struct
{
    token_type_t type;
    const char *value;
    int valueLength;
    int line;
    int column;
} token_t;

/**
 * Vector that contains all the token definitions.
 */
extern std::vector<token_def_t> token_definitions;

void define_token(const char *expression, token_type_t token);

#endif //STRIDE_LANGUAGE_TOKEN_H
