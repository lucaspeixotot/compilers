/**
 * @file   parser.c
 * @author Lucas Peixoto <lucaspeixotoac@gmail.com>
 *
 */

#include "parser.h"

#include <stdio.h>

#include "scanner.h"

token_t tk = {0};

static void res_handler(void);
static double ea_handler(void);
static double ear_handler(double earvh);
static double ta_handler(void);
static double tar_handler(double tarvh);
static double fa_handler(void);

static double pot(double x, double y)
{
    double ret = 1;
    for (int i = 0; i < y; ++i) {
        ret = ret * x;
    }

    return ret;
}

static double fa_handler(void)
{
    double faval, eaval;

    if (tk.cat == OPEN_PAREN) {
        if (scanner_next_token(&tk) == NOTOKEN) {
            printf("Token não identificado, finalizando a análise!\n");
            exit(1);
        }
        faval = ea_handler();

        if (tk.cat == CLOSE_PAREN) {
            if (scanner_next_token(&tk) == NOTOKEN) {
                printf("Token não identificado, finalizando a análise!\n");
                exit(1);
            }
        } else {
            printf("Erro sintático, esperado ')'\n");
            exit(1);
        }
    } else if (tk.cat == CONST_FLOAT) {
        faval = atoi(tk.value);
        if (scanner_next_token(&tk) == NOTOKEN) {
            printf("Token não identificado, finalizando a análise!\n");
            exit(1);
        }
    } else {
        printf("Erro sintático, esperado '(' ou 'Cater'\n");
        exit(1);
    }

    return faval;
}

static double tar_handler(double tarvh)
{
    double faval, tar1vh, tarvs, tar1vs;

    if (tk.cat == OP_MINUS) {
        if (scanner_next_token(&tk) == NOTOKEN) {
            printf("Token não identificado, finalizando a análise!\n");
            exit(1);
        }
        faval  = fa_handler();
        tar1vh = tarvh - faval;
    } else if (tk.cat == OP_MULT) {
        if (scanner_next_token(&tk) == NOTOKEN) {
            printf("Token não identificado, finalizando a análise!\n");
            exit(1);
        }
        faval  = fa_handler();
        tar1vh = tarvh * faval;
    } else if (tk.cat == OP_DIVIDE) {
        if (scanner_next_token(&tk) == NOTOKEN) {
            printf("Token não identificado, finalizando a análise!\n");
            exit(1);
        }
        faval  = fa_handler();
        tar1vh = tarvh / faval;
    } else if (tk.cat == OP_MOD) {
        if (scanner_next_token(&tk) == NOTOKEN) {
            printf("Token não identificado, finalizando a análise!\n");
            exit(1);
        }
        faval  = fa_handler();
        tar1vh = (double) ((int) tarvh % (int) faval);
    } else if (tk.cat == OP_POT) {
        if (scanner_next_token(&tk) == NOTOKEN) {
            printf("Token não identificado, finalizando a análise!\n");
            exit(1);
        }
        faval  = fa_handler();
        tar1vh = pot(tarvh, faval);
    } else {
        return tarvh;
    }

    tar1vs = tar_handler(tar1vh);
    tarvs  = tar1vs;

    return tarvs;
}

static double ta_handler(void)
{
    double faval, tarvh, taval, tarvs;
    faval = fa_handler();
    tarvh = faval;

    tarvs = tar_handler(tarvh);
    taval = tarvs;

    return taval;
}

static double ear_handler(double earvh)
{
    double taval, ear1vh, ear1vs, earvs;

    if (tk.cat == OP_PLUS) {
        if (scanner_next_token(&tk) == NOTOKEN) {
            printf("Token não identificado, finalizando a análise!\n");
            exit(1);
        }
        taval  = ta_handler();
        ear1vh = earvh + taval;

        ear1vs = ear_handler(ear1vh);
        earvs  = ear1vs;
    } else {
        return earvh;
    }
}

static double ea_handler(void)
{
    double taval, earvh, earvs, eaval;
    taval = ta_handler();
    earvh = taval;
    earvs = ear_handler(earvh);
    eaval = earvs;

    return eaval;
}

static void res_handler(void)
{
    double eaval;
    eaval = ea_handler();
    if (tk.cat == EQUAL) {
        if (scanner_next_token(&tk) == NOTOKEN) {
            printf("Token não identificado, finalizando a análise!\n");
            exit(1);
        }
        if (tk.cat == CAT_EOF) {
            printf("%f\n", eaval);
        } else {
            printf("Erro sintático, esperado EOF\n");
        }
    } else {
        printf("Erro sintático, esperado '='\n");
    }
}

int parser_run(void)
{
    if (scanner_next_token(&tk) == NOTOKEN) {
        printf("Token não identificado, finalizando a análise!\n");
        exit(1);
    }
    res_handler();
}
