/**
 * @file   scanner.h
 * @author Lucas Peixoto <lucaspeixotoac@gmail.com>
 *
 * @brief This header file defines the lexical part of the
 * LisC compiler and token stuffs.
 */

#ifndef _SCANNER_H_
#define _SCANNER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "errno.h"

/* Variables */
#define NOTOKEN 100
#define CATEGORY_LENGTH_VALUE 25
#define STR_REGEX_LENGTH 200
#define MAX_LINE_LENGTH 256
#define TOKEN_VALUE_LENGTH 256

/**
 * Enum for token category
 *
 */
enum cat {
    INT_TYPE = 0,
    CHAR_TYPE,
    STRING_TYPE,
    BOOL_TYPE,
    FLOAT_TYPE,
    FORMAT_FIELD,
    FORMAT_DECIMAL,
    DEFUN,
    DEFINT,
    DEFCHAR,
    DEFFLOAT,
    DEFBOOL,
    DEFSTRING,
    IF,
    ELSE,
    FOR,
    WHILE,
    ECHO,
    RETURN,
    BREAK,
    CONTINUE,
    NIL,
    SET,
    READ,
    START,
    CONST_FLOAT,
    CONST_INT,
    CONST_STRING,
    CONST_CHAR,
    CONST_BOOL,
    OP_MINUS,
    OP_PLUS,
    OP_DIVIDE,
    OP_MULT,
    OP_EQUAL,
    OP_DIFF,
    OP_LEQ,
    OP_LESS,
    OP_GEQ,
    OP_GREATER,
    OP_OR,
    OP_AND,
    OP_NOT,
    OP_CONCAT,
    OPEN_PAREN,
    CLOSE_PAREN,
    OPEN_BRACKETS,
    CLOSE_BRACKETS,
    OPEN_KEYS,
    CLOSE_KEYS,
    SEMICOLON,
    COMMA,
    ID,
    CAT_EOF,
    CATEGORIES_SIZE
};
typedef enum cat cat_t;

/**
 * This function will be used to convert enum cat to string
 *
 * @param cat
 * @param cat_value
 */
const char *cat_to_string(cat_t cat);

/**
 * Struct that represents the token
 *
 */
struct token {
    cat_t cat;
    char value[TOKEN_VALUE_LENGTH];
    int line;
    int column;
};
typedef struct token token_t;

/**
 * Struct that represents the scanner attributes
 *
 */
struct scanner {
    char *filename;
    FILE *fptr;
    char line[MAX_LINE_LENGTH];
    int line_number;
    int column_number;
};
typedef struct scanner scanner_t;

/**
 * This function will be used to init the scanner struct
 *
 * @param filename The file name that to be scanned
 *
 * @return errno
 */
int scanner_init(char *filename);

/**
 * This function will return the next token
 *
 * @param tk pointer to token that to be filled
 *
 * @return errno
 */
int scanner_next_token(token_t *tk);


/**
 * This function will be used to finish the scanner operation
 *
 *
 * @return errno
 */
int scanner_finish(void);

#endif
