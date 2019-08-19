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

    err = parser_run();
    if (err) {
        printf("Error parsing the file!! (err %d)\n", err);
    }

    printf("The file was analyzed!\n");

    err = scanner_finish();
    if (err) {
        printf("Error finishing the scanner!! (err %d)\n", err);
    }

    return err;
}
