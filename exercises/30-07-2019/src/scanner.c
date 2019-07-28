/**
 * @file   lexical.c
 * @author Lucas Peixoto <lucaspeixotoac@gmail.com>
 *
 * @brief
 *
 */

#include "scanner.h"

#include <regex.h>

scanner_t scanner = {.line_number = 0, .column_number = 0};


const char ERs[][STR_REGEX_LENGTH] = {
    "([+-]?[0-9]+[.]?[0-9]*)", "\\%", "\\-", "\\+", "\\/", "\\*\\*", "\\*", "\\=", "\\(", "\\)",
};

const char cat_to_value[][CATEGORY_LENGTH_VALUE] = {
    "CONST_DOUBLE", "OP_MOD", "OP_MINUS",   "OP_PLUS",     "OP_DIVIDE", "OP_POT",
    "OP_MULT",      "EQUAL",  "OPEN_PAREN", "CLOSE_PAREN", "CAT_EOF",   "CATEGORIES_SIZE"};

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

int scanner_init(char *line)
{
    strcpy(scanner.line, line);

    printf("%s\n", scanner.line);

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

    if (scanner.column_number >= line_length) {
        tk->column = scanner.column_number + 1;
        tk->line   = scanner.line_number;
        tk->cat    = CAT_EOF;
        memset(tk->value, 0, sizeof(tk->value));
        return 0;
    }

    while (!regexec(&reg_whitespace, scanner.line + scanner.column_number, 1, &m, REG_NOTBOL)) {
        if (m.rm_so != 0) {
            break;
        }
        scanner.column_number += m.rm_eo;
    }

    for (int i = 0; i < CATEGORIES_SIZE; ++i) {
        if (i == CAT_EOF) {
            break;
        }

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
    return 0;
}
