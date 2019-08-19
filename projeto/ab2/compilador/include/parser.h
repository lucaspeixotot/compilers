/**
 * @file   parser.h
 * @author Lucas Peixoto <lucaspeixotoac@gmail.com>
 * @brief
 *
 *
 */

#ifndef _PARSER_H_
#define _PARSER_H_

#include "scanner.h"

#define SYNTAX_ERROR 1

/* First sets */
#define CHECK_FIRST_SET_VARIAVEL_F(cat)                                                        \
    ((cat == OP_NOT) || (cat == ID) || (cat == CONST_FLOAT) || (cat == CONST_INT)              \
     || (cat == CONST_STRING) || (cat == CONST_CHAR) || (cat == CONST_BOOL) || (cat == OP_ADI) \
     || (cat == OPEN_PAREN) || (cat == OPEN_BRACKETS))

#define CHECK_FIRST_SET_DECL(cat)                                              \
    ((cat == DEFUN) || (cat == DEFINT) || (cat == DEFCHAR) || (cat == DEFBOOL) \
     || (cat == DEFSTRING) || (cat == DEFFLOAT))

#define CHECK_FIRST_SET_DECL_VARIAVEIS(cat)                                        \
    ((cat == DEFINT) || (cat == DEFCHAR) || (cat == DEFBOOL) || (cat == DEFSTRING) \
     || (cat == DEFFLOAT))

#define CHECK_FIRST_SET_COMANDO(cat)                                                       \
    ((cat == CONTINUE) || (cat == BREAK) || (cat == SET) || (cat == ID) || (cat == RETURN) \
     || (cat == READ) || (cat == ECHO) || (cat == IF) || (cat == FOR) || (cat == WHILE))

#define CHECK_FIRST_SET_CONTINUE(cat) (cat == CONTINUE)
#define CHECK_FIRST_SET_BREAK(cat) (cat == BREAK)
#define CHECK_FIRST_SET_ATRIBUICAO(cat) (cat == SET)
#define CHECK_FIRST_SET_CHAMADAFUNC(cat) (cat == ID)
#define CHECK_FIRST_SET_RETURN(cat) (cat == RETURN)
#define CHECK_FIRST_SET_READ(cat) (cat == READ)
#define CHECK_FIRST_SET_ECHO(cat) (cat == ECHO)
#define CHECK_FIRST_SET_IF(cat) (cat == IF)
#define CHECK_FIRST_SET_FOR(cat) (cat == FOR)
#define CHECK_FIRST_SET_WHILE(cat) (cat == WHILE)

int parser_run(void);

#endif  // _PARSER_H_
