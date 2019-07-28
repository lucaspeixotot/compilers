#include <regex.h>
#include <scanner.h>
#include <stdio.h>

#include "parser.h"

int main(int argc, char **argv)
{
    int err = 0;
    int rc  = 0;

    if (argc != 2) {
        printf("Expected just one argument!!\n");
        return 1;
    }


    err = scanner_init(argv[1]);
    if (err) {
        return err;
    }

    parser_run();

    printf("The file was analyzed!\n");

    err = scanner_finish();

    return err;
}
