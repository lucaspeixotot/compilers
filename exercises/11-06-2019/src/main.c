/**
 * @file main.c
 * @brief 
 * @author Lucas Peixoto (lucaspeixotoac@gmail.com)
 * @version 
 * @date 2019-06-11
 */

#include <stdio.h>

#include "errno.h"
#include "parser.h"
#include "lexical.h"
#include "globals.h"

// #TODO: finalizar struct, enums e implementar o módulo do analisador 
//        léxico.

int translate_error(enum category cat) {
    switch(cat) {
        C_EOF: 
            printf("[ERROR] EOF was expected!\n");
            return -EEOF;
            break;
        C_PT:
            printf("[ERROR] \".\" was expected!\n");
            return -EPT;
            break;
        C_PTV:
            printf("[ERROR] \";\" was expected!\n");
            return -EPTV;
            break;
        default:
            break;
    }

    return 0;
}

/************O INICIO DA SOLUÇÃO DA ATIVIDADE COMEÇA AQUI**************/

int program_solver(void) {
    int err = 0;

    err = lsent_handler();
    if (err) {
        return err;
    }

    if (tk.cat == C_PT) {
        tk.next();
        if (tk.cat == C_EOF) {
            err = 0;
        }
        else {
            err = translate_error(C_EOF);
        }
    }
    else {
        err = translate_error(C_PT);
    }

    return err;
}

int lsent_handler(void) {
    int err = 0;

    err = sent_handler();
    if (err) {
        return err;
    }

    err = lsentr_handler();
    if (err) {
        return err;
    }

    return 0;
}

int lsentr_handler(void) {
    int err = 0;

    if (tk.cat == C_PTV) {
        tk.next();

        err = sent_handler();
        if (err) {
            return err;
        }

        err = lsentr_handler();
        if (err) {
            return err;
        }
    }

    return err;
}

int sent_handler(void) {
    int err = 0;

    if (tk.cat == C_SE) {
        tk.next();

        err = eb_handler();
        if (err) {
            return err;
        }

        if (tk.cat == C_ENTAO) {
            tk.next();

            err = lsent_handler();
            if (err) {
                return err;
            }

            if (tk.cat == C_FIM) {
                tk.next();
            }
            else if (tk.cat == C_SENAO) {
                tk.next();

                err = lsent_handler();
                if (err) {
                    return err;
                }

                if (tk.cat == C_FIM) {
                    tk.next();
                }
                else {
                    err = translate_error(C_FIM);
                }
            }
            else {
                err = translate_error(C_FIM | C_SENAO);
            }
        }
        else {
            err = translate_error(C_ENTAO);
        }

    }
    else if (tk.cat == C_PARA) {
        tk.next();

        err = atr_handler();
        if (err) {
            return err;
        }

        if (tk.cat == C_ATE) {
            tk.next();

            err = ea_handler();
            if (err) {
                return err;
            }

            if (tk.cat == C_REPITA) {
                tk.next();

                err = lsent_handler();
                if (err) {
                    return err;
                }

                if (tk.cat == C_FIM) {
                    tk.next();
                }
                else {
                    err = translate_error(C_FIM);
                }
            }
            else {
                err = translate_error(C_REPITA);
            }
        }
        else {
            err = translate_error(C_ATE);
        }
    }
    else {
        err = atr_handler();
    }

    return err;
}

int atr_handler(void) {
    int err = 0;

    if (tk.cat == C_ID) {
        tk.next();

        if (tk.cat == C_EQUAL) {
            tk.next();

            err = expr_handler();
            if (err) {
                return err;
            }
        }
        else {
            err = translate_error(C_EQUAL);
        }
    }
    else {
        err = translate_error(C_ID);
    }

    return err;
} 

int expr_handler(void) {
    return eb_handler();
}

int eb_handler(void) {
    int err = 0;

    err = tb_handler();
    if (err) {
        return err;
    }

    err = ebr_handler();
    if (err) {
        return err;
    }

    return 0;
}

int ebr_handler(void) {
    int err = 0;

    if (tk.cat == C_OPL) {
        tk.next();

        err = tb_handler();
        if (err) {
            return err;
        }

        err = ebr_handler();
        if (err) {
            return err;
        }
    }

    return err;
}

int tb_handler(void) {
    int err = 0;

    if (tk.cat == C_NAO) {
        tk.next();

        err = tb_handler();
        if (err) {
            return err;
        }
    }
    else if (tk.cat == C_VERD) {
        tk.next();
    }
    else if (tk.cat == C_FALSO) {
        tk.next();
    }
    else {
        err = ea_handler();
    }

    return err;
}

int ea_handler(void) {
    int err = 0;

    err = ta_handler();
    if (err) {
        return err;
    }

    err = ear_handler();
    if (err) {
        return err;
    }

    return 0;
}

int ear_handler(void) {
    int err = 0;

    if (tk.cat == C_OPA) {
        tk.next();

        err = ta_handler();
        if (err) {
            return err;
        }

        err = ear_handler();
        if (err) {
            return err;
        }
    }

    return err;
}

int ta_handler(void) {
    int err = 0;

    err = fa_handler();
    if (err) {
        return err;
    }

    err = tar_handler();
    if (err) {
        return err;
    }

    return 0;
}

int tar_handler(void) {
    int err = 0;

    if (tk.cat == C_OPM) {
        tk.next();

        err = fa_handler();
        if (err) {
            return err;
        }

        err = tar_handler();
        if (err) {
            return err;
        }
    }

    return err;
}

int fa_handler(void) {
    int err = 0;

    if (tk.cat == C_ABREPAREN) {
        tk.next();

        err = expr_handler();
        if (err) {
            return err;
        }

        if (tk.cat == C_FECHAPAREN) {
            tk.next();
        }
        else {
            err = translate_error(C_FECHAPAREN);
        }

    }
    else if (tk.cat == C_ID || tk.cat == C_CTE) {
        tk.next();
    }
    else {
        err = translate_error(C_FECHAPAREN | C_ID | C_CTE);
    }

    return err;
}

int main(void) {
    return 0;
}
