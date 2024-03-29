#include <regex.h>
#include <scanner.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    int err = 0;
    int rc  = 0;

    if (argc != 2) {
        printf("Wrong number of arguments!!\n");
        return -1;
    }

    err = scanner_init(argv[1]);
    if (err) {
        return err;
    }

    token_t tk = {0};
    rc         = scanner_next_token(&tk);

    while (rc >= 0) {
        printf("          [%04d, %04d] (%04d, %20s) {%s}\n", tk.line, tk.column, tk.cat,
               cat_to_string(tk.cat), tk.value);

        if (tk.cat == CAT_EOF) {
            break;
        }

        rc = scanner_next_token(&tk);

        if (rc == NOTOKEN) {
            printf("Unexpected token, finishing analyze\n");
            break;
        }
    }

    printf("The file was analyzed!\n");

    err = scanner_finish();

    return err;
}
