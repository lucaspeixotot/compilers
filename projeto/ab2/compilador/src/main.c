#include <regex.h>
#include <scanner.h>
#include <stdio.h>

#include "parser.h"

int main(int argc, char **argv)
{
    int err = 0;
    int rc  = 0;

    if (argc != 2) {
        printf("Esperado apenas um argumento!!\n");
        return 1;
    }


    err = scanner_init(argv[1]);
    if (err) {
        return err;
    }

    err = parser_run();
    if (err) {
        printf("Erro ao fazer a análise sintática do arquivo!! (err %d)\n", err);
    } else {
        printf("Arquivo analisado sintaticamente com sucesso!!\n");
    }

    err = scanner_finish();
    if (err) {
        printf("Error finishing the scanner!! (err %d)\n", err);
    }

    return err;
}
