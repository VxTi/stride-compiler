//
// Created by Luca Warmenhoven on 16/08/2024.
//

#ifndef STRIDE_LANGUAGE_TOKEN_H
#define STRIDE_LANGUAGE_TOKEN_H

#include <vector>
#include <regex.h>
#include <map>

typedef enum
{
    TOKEN_COMMENT = 0,
    TOKEN_COMMENT_MULTILINE = 0,

    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_LSQUARE_BRACKET,
    TOKEN_RSQUARE_BRACKET,
    TOKEN_COMMA,
    TOKEN_LARROW_DASH,
    TOKEN_DASH_RARROW,
    TOKEN_RARROW,
    TOKEN_LARROW,
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
    TOKEN_TILDE_EQUALS,
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
    TOKEN_DOUBLE_LARROW_EQUALS,
    TOKEN_DOUBLE_RARROW_EQUALS,
    TOKEN_DOUBLE_STAR_EQUALS,
    TOKEN_DOUBLE_PIPE,
    TOKEN_DOUBLE_AMPERSAND,
    TOKEN_DOUBLE_PLUS,
    TOKEN_DOUBLE_MINUS,
    TOKEN_DOUBLE_STAR,
    TOKEN_DOUBLE_COLON,
    TOKEN_SEMICOLON,
    TOKEN_COLON,
    TOKEN_DOT,
    TOKEN_THREE_DOTS,

    /* Primitives */
    TOKEN_PRIMITIVE_UINT8,
    TOKEN_PRIMITIVE_UINT16,
    TOKEN_PRIMITIVE_UINT32,
    TOKEN_PRIMITIVE_UINT64,
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
    TOKEN_PRIMITIVE_UNKNOWN,

    /* Keywords */
    TOKEN_KEYWORD_HAS,
    TOKEN_KEYWORD_AND,
    TOKEN_KEYWORD_AS,
    TOKEN_KEYWORD_ASYNC,
    TOKEN_KEYWORD_DEFINE,
    TOKEN_KEYWORD_LET,
    TOKEN_KEYWORD_CONST,
    TOKEN_KEYWORD_DO,
    TOKEN_KEYWORD_WHILE,
    TOKEN_KEYWORD_FOR,
    TOKEN_KEYWORD_SWITCH,
    TOKEN_KEYWORD_TRY,
    TOKEN_KEYWORD_CATCH,
    TOKEN_KEYWORD_THROW,
    TOKEN_KEYWORD_NEW,
    TOKEN_KEYWORD_RETURN,
    TOKEN_KEYWORD_IF,
    TOKEN_KEYWORD_ELSE,
    TOKEN_KEYWORD_CLASS,
    TOKEN_KEYWORD_THIS,
    TOKEN_KEYWORD_STRUCT,
    TOKEN_KEYWORD_IMPORT,
    TOKEN_KEYWORD_PUBLIC,
    TOKEN_KEYWORD_MODULE,
    TOKEN_KEYWORD_EXTERNAL,
    TOKEN_KEYWORD_NULL,
    TOKEN_KEYWORD_OVERRIDE,
    TOKEN_KEYWORD_ENUM,
    TOKEN_KEYWORD_CASE,
    TOKEN_KEYWORD_DEFAULT,
    TOKEN_KEYWORD_BREAK,

    TOKEN_IDENTIFIER,
    TOKEN_STRING_LITERAL,
    TOKEN_CHAR_LITERAL,
    TOKEN_NUMBER_INTEGER,
    TOKEN_NUMBER_FLOAT,
    TOKEN_BOOLEAN_LITERAL,
} token_type_t;

/**
 * Definition of a required_token.
 * This includes the regular expression that has to match,
 * and the reference required_token enumerable.
 */
typedef struct
{
    token_type_t token;
    regex_t regex;
    bool keyword;
} token_def_t;

/**
 * Struct that represents a matched required_token.
 * This includes the properties of the required_token, the current of the required_token,
 * the length of the current, the line number and the column number.
 */
typedef struct
{
    token_type_t type;
    char *value;
    int index;
} token_t;

/**
 * Vector that contains all the required_token definitions.
 */
extern std::vector<token_def_t> token_definitions;

/**
 * Map that contains the byte sizes of the primitive types.
 * This is used for the code generation.
 */
extern std::map<token_type_t, unsigned int> primitive_byte_sizes;

#endif //STRIDE_LANGUAGE_TOKEN_H
