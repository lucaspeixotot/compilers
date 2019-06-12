/**
 * @file lexical.h
 * @brief 
 * @author Lucas Peixoto (lucaspeixotoac@gmail.com)
 * @version 
 * @date 2019-06-11
 */

#ifndef _LEXICAL_H_
#define _LEXICAL_H_

typedef struct {
    int row;
    int column;
    char *content;
    enum category cat;
    void (*next)(void);
} tk_t;

tk_t tk = {
    .row = -1,
    .column = -1,
    .content = NULL,
    .cat = C_NONE,
    .next = NULL
};

#endif
