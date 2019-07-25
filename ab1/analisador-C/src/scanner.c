/**
 * @file   lexical.c
 * @author Lucas Peixoto <lucaspeixotoac@gmail.com>
 *
 * @brief
 *
 */

#include "scanner.h"

#include <regex.h>

scanner_t scanner = {.line_number = 0, .column_number = -1};


const char ERs[][STR_REGEX_LENGTH] = {
    "int",
    "char",
    "string",
    "bool",
    "float",
    "\\%",
    "\\%\\%",
    "defun",
    "defint",
    "defchar",
    "deffloat",
    "defbool",
    "defstring",
    "if",
    "else",
    "for",
    "while",
    "echo",
    "return",
    "break",
    "continue",
    "nil",
    "set",
    "read",
    "start",
    "([0-9])+\\.([0-9])*",
    "[0-9]+",
    "\"([A-Za-z]|[0-9]|\\s|!|#|\\$|%|&|\\”|\\(|\\)|\\*|\\+|\\,|\\-|\\.|\\/"
    "|:|;|<|=|>|\\?|@|[|]|\\^|_|\\‘|\\{|\\||}|\\∼)*\"",
    "\"([A-Za-z]|[0-9]|\\s|!|#|\\$|%|&|\\”|\\(|\\)|\\*|\\+|\\,|\\-|\\.|\\/"
    "|:|;|<|=|>|\\?|@|[|]|\\^|_|\\‘|\\{|\\||}|\\∼)\"",
    "(true|false)",
    "\\-",
    "\\+",
    "\\/",
    "\\*",
    "\\=\\=",
    "\\!\\=",
    "<=",
    "<",
    ">=",
    ">",
    "\\|\\|",
    "\\&\\&",
    "\\!",
    "<<",
    "\\(",
    "\\)",
    "\\[",
    "\\]",
    "\\{",
    "\\}",
    "\\;",
    "\\,",
    "void",
    "([A-Za-z]|_)\\w*",
};

const char cat_to_value[][CATEGORY_LENGTH_VALUE] = {"INT_TYPE",
                                                    "CHAR_TYPE",
                                                    "STRING_TYPE",
                                                    "BOOL_TYPE",
                                                    "FLOAT_TYPE",
                                                    "FORMAT_FIELD",
                                                    "FORMAT_DECIMAL",
                                                    "DEFUN",
                                                    "DEFINT",
                                                    "DEFCHAR",
                                                    "DEFFLOAT",
                                                    "DEFBOOL",
                                                    "DEFSTRING",
                                                    "IF",
                                                    "ELSE",
                                                    "FOR",
                                                    "WHILE",
                                                    "ECHO",
                                                    "RETURN",
                                                    "BREAK",
                                                    "CONTINUE",
                                                    "NIL",
                                                    "SET",
                                                    "READ",
                                                    "START_FUNCTION",
                                                    "CONST_FLOAT",
                                                    "CONST_INT",
                                                    "CONST_STRING",
                                                    "CONST_CHAR",
                                                    "CONST_BOOL",
                                                    "OP_MINUS",
                                                    "OP_PLUS",
                                                    "OP_DIVIDE",
                                                    "OP_MULT",
                                                    "OP_EQUAL",
                                                    "OP_DIFF",
                                                    "OP_LEQ",
                                                    "OP_LESS",
                                                    "OP_GEQ",
                                                    "OP_GREATER",
                                                    "OP_OR",
                                                    "OP_AND",
                                                    "OP_NOT",
                                                    "OP_CONCAT",
                                                    "OPEN_PAREN",
                                                    "CLOSE_PAREN",
                                                    "OPEN_BRACKETS",
                                                    "CLOSE_BRACKETS",
                                                    "OPEN_KEYS",
                                                    "CLOSE_KEYS",
                                                    "SEMICOLON",
                                                    "COMMA",
                                                    "VOID",
                                                    "ID",
                                                    "CAT_EOF",
                                                    "CATEGORIES_SIZE"};

regex_t regs[CATEGORIES_SIZE] = {0};
regex_t reg_whitespace;

static int mount_token(token_t *tk, cat_t cat, int line, int column, int eo, char *str)
{
    tk->cat    = cat;
    tk->line   = line;
    tk->column = column;
    memset(tk->value, 0, sizeof(tk->value));
    for (int i = 0; i < eo; ++i) {
        tk->value[i] = str[scanner.column_number + i];
    }

    return 0;
}

static int blank_line(char *str, int len)
{
    regmatch_t m;
    int rc = regexec(&reg_whitespace, str, 1, &m, REG_NOTBOL);

    if (rc) {
        return 0;
    } else {
        if (m.rm_so == 0 && m.rm_eo == len) {
            return 1;
        } else {
            return 0;
        }
    }
}

int scanner_init(char *filename)
{
    scanner.filename = filename;
    scanner.fptr     = fopen(filename, "r");
    if (scanner.fptr == NULL) {
        return -EINVAL;
    }

    for (int i = 0; i < CATEGORIES_SIZE; ++i) {
        if (regcomp(&regs[i], ERs[i], REG_EXTENDED)) {
            printf("Error compiling regex (%d, %s)\n", i, ERs[i]);
            return -EIO;
        }
    }

    if (regcomp(&reg_whitespace, "([[:blank:]]|[[:space:]])+", REG_EXTENDED)) {
        printf("Error compiling whitespace regex\n");
        return -EIO;
    }

    return 0;
}

int scanner_next_token(token_t *tk)
{
    regmatch_t m;
    int err = 0;
    int rc  = 0;
    void *err_ptr;

    int line_length = strlen(scanner.line);

    if (scanner.column_number >= line_length || scanner.column_number < 0) {
        do {
            err_ptr = fgets(scanner.line, sizeof(scanner.line), scanner.fptr);
            if (err_ptr != NULL) {
                line_length = strlen(scanner.line);
                scanner.line_number++;
                if (!blank_line(scanner.line, line_length)) {
                    scanner.line[strlen(scanner.line) - 1] = 0;
                    scanner.column_number                  = 0;
                    printf("%4d  %s\n", scanner.line_number, scanner.line);
                }
            } else {
                tk->column = scanner.column_number + 1;
                tk->line   = scanner.line_number;
                tk->cat    = CAT_EOF;
                memset(tk->value, 0, sizeof(tk->value));
                return 0;
            }
        } while (blank_line(scanner.line, line_length));
    }


    while (!regexec(&reg_whitespace, scanner.line + scanner.column_number, 1, &m, REG_NOTBOL)) {
        if (m.rm_so != 0) {
            break;
        }
        scanner.column_number += m.rm_eo;
    }

    for (int i = 0; i < CATEGORIES_SIZE; ++i) {
        rc = regexec(&regs[i], scanner.line + scanner.column_number, 1, &m, REG_NOTBOL);
        if (!rc && m.rm_so == 0) {
            mount_token(tk, (enum cat) i, scanner.line_number, scanner.column_number + 1, m.rm_eo,
                        scanner.line);
            scanner.column_number += m.rm_eo;
            goto scanner_next_token_end;
        }
    }
    rc = NOTOKEN;

scanner_next_token_end:
    return rc;
}


const char *cat_to_string(cat_t cat)
{
    return cat_to_value[cat];
}

int scanner_finish(void)
{
    fclose(scanner.fptr);
    return 0;
}
