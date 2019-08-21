/**
 * @file   parser.c
 * @author Lucas Peixoto <lucaspeixotoac@gmail.com>
 *
 *
 */

#include "parser.h"

#include <stdio.h>

#include "scanner.h"

token_t tk = {0};

static int exp_booleana_handler(void);
static int exp_concat_handler(void);
static int lista_sent_handler(void);
static int lista_arg_handler(void);
static int exp_aritmetica_handler(void);

static int def_tipo_handler(void)
{
    int err = 0;

    switch (tk.cat) {
    case DEFINT:
        printf("          DefTipo = 'defint'\n");
        break;
    case DEFBOOL:
        printf("          DefTipo = 'defbool'\n");
        break;
    case DEFCHAR:
        printf("          DefTipo = 'defchar'\n");
        break;
    case DEFFLOAT:
        printf("          DefTipo = 'deffloat'\n");
        break;
    case DEFSTRING:
        printf("          DefTipo = 'defstring'\n");
        break;
    default:
        printf("Esperado uma declaração de variável, por exemplo, 'defint'\n");
        return -SYNTAX_ERROR;
    }
    PRINT_TOKEN(tk);
    NEXT_TOKEN(tk);

    return 0;
}

static int fator_aritmetico_F_handler(void)
{
    int err = 0;

    if (tk.cat == OPEN_PAREN) {
        printf("          FatorAritmeticoF = '(' ListaArg ')'\n");
        PRINT_TOKEN(tk);
        NEXT_TOKEN(tk);

        err = lista_arg_handler();
        if (err) {
            return err;
        }

        if (tk.cat == CLOSE_PAREN) {
            PRINT_TOKEN(tk);
            NEXT_TOKEN(tk);

        } else {
            printf("Esperado um ')'\n");
            return -SYNTAX_ERROR;
        }

    } else if (tk.cat == OPEN_BRACKETS) {
        printf("          FatorAritmeticoF = '[' ExpAritmetica ']'\n");
        PRINT_TOKEN(tk);
        NEXT_TOKEN(tk);

        err = exp_aritmetica_handler();
        if (err) {
            return err;
        }

        if (tk.cat == CLOSE_BRACKETS) {
            PRINT_TOKEN(tk);
            NEXT_TOKEN(tk);
        } else {
            printf("Esperado um ']'\n");
            return -SYNTAX_ERROR;
        }
    } else {
        printf("          FatorAritmeticoF = epsilon\n");
    }

    return 0;
}

static int fator_aritmetico_handler(void)
{
    int err = 0;

    switch (tk.cat) {
    case OP_ADI:
        printf("          FatorAritmetico = 'op_adi' FatorAritmetico\n");
        PRINT_TOKEN(tk);
        NEXT_TOKEN(tk);
        err = fator_aritmetico_handler();
        if (err) {
            return err;
        }
        break;
    case ID:
        printf("          FatorAritmetico = 'id' FatorAritmeticoF\n");
        PRINT_TOKEN(tk);
        NEXT_TOKEN(tk);

        err = fator_aritmetico_F_handler();
        if (err) {
            return err;
        }
        break;
    case CONST_FLOAT:
        printf("          FatorAritmetico = 'const_float'\n");
        PRINT_TOKEN(tk);
        NEXT_TOKEN(tk);
        break;
    case CONST_INT:
        printf("          FatorAritmetico = 'const_int'\n");
        PRINT_TOKEN(tk);
        NEXT_TOKEN(tk);
        break;
    case CONST_BOOL:
        printf("          FatorAritmetico = 'const_bool'\n");
        PRINT_TOKEN(tk);
        NEXT_TOKEN(tk);
        break;
    case CONST_CHAR:
        printf("          FatorAritmetico = 'const_char'\n");
        PRINT_TOKEN(tk);
        NEXT_TOKEN(tk);
        break;
    case CONST_STRING:
        printf("          FatorAritmetico = 'const_string'\n");
        PRINT_TOKEN(tk);
        NEXT_TOKEN(tk);
        break;
    case OPEN_PAREN:
        printf("          FatorAritmetico = '(' ExpConcat ')'\n");
        PRINT_TOKEN(tk);
        NEXT_TOKEN(tk);
        err = exp_concat_handler();
        if (err) {
            return err;
        }

        if (tk.cat == CLOSE_PAREN) {
            PRINT_TOKEN(tk);
            NEXT_TOKEN(tk);
        } else {
            printf("Esperado um ')\n");
            return -SYNTAX_ERROR;
        }
        break;
    default:
        printf("Esperado um ID, constantes, operador negativo ou abre parênteses\n");
        return -SYNTAX_ERROR;
    }

    return 0;
}

static int term_formatacao_R_handler(void)
{
    int err = 0;
    if (tk.cat == OP_FORMAT) {
        printf("          TermFormatacaoR = 'op_format' FatorAritmetico TermFormatacaoR\n");
        PRINT_TOKEN(tk);
        NEXT_TOKEN(tk);
        err = fator_aritmetico_handler();
        if (err) {
            return err;
        }

        err = term_formatacao_R_handler();
        if (err) {
            return err;
        }
    } else {
        printf("          TermFormatacaoR = epsilon\n");
    }

    return 0;
}

static int term_formatacao_handler(void)
{
    int err = 0;
    printf("          TermFormatacao = FatorAritmetico TermFormatacaoR\n");
    err = fator_aritmetico_handler();
    if (err) {
        return err;
    }

    err = term_formatacao_R_handler();
    if (err) {
        return err;
    }
    return 0;
}

static int term_aritmetico_R_handler(void)
{
    int err = 0;
    if (tk.cat == OP_MULT_DIV) {
        printf("          TermAritmeticoR = 'op_mult_div' TermFormatacao TermFormatacaoR\n");
        PRINT_TOKEN(tk);
        NEXT_TOKEN(tk);
        err = term_formatacao_handler();
        if (err) {
            return err;
        }

        err = term_formatacao_R_handler();
        if (err) {
            return err;
        }
    } else {
        printf("          TermAritmeticoR = epsilon\n");
    }
    return 0;
}

static int term_aritmetico_handler(void)
{
    int err = 0;
    printf("          TermAritmetico = TermFormatacao TermAritmeticoR\n");
    err = term_formatacao_handler();
    if (err) {
        return err;
    }

    err = term_aritmetico_R_handler();
    if (err) {
        return err;
    }
    return 0;
}

static int exp_aritmetica_R_handler(void)
{
    int err = 0;

    if (tk.cat == OP_ADI) {
        printf("          ExpAritmeticaR = 'op_adi' TermAritmetico ExpAritmeticaR\n");
        PRINT_TOKEN(tk);
        NEXT_TOKEN(tk);

        err = term_aritmetico_handler();
        if (err) {
            return err;
        }

        err = exp_aritmetica_R_handler();
        if (err) {
            return err;
        }
    } else {
        printf("          ExpAritmeticaR = epsilon\n");
    }

    return 0;
}

static int exp_aritmetica_handler(void)
{
    int err = 0;
    printf("          ExpAritmetica = TermAritmetico ExpAritmeticaR\n");
    err = term_aritmetico_handler();
    if (err) {
        return err;
    }

    err = exp_aritmetica_R_handler();
    if (err) {
        return err;
    }
    return 0;
}

static int term_booleano_F_handler(void)
{
    int err = 0;

    if (tk.cat == OP_RELACIONAL) {
        printf("          TermBooleanoF = 'op_relacional' ExpAritmetica\n");
        PRINT_TOKEN(tk);
        NEXT_TOKEN(tk);
        err = exp_aritmetica_handler();
        if (err) {
            return err;
        }
    } else {
        printf("          TermBooleanoF = epsilon\n");
    }

    return 0;
}

static int term_booleano_handler(void)
{
    int err = 0;
    if (tk.cat == OP_NOT) {
        printf("          TermBooleano = '!' TermBooleano\n");
        PRINT_TOKEN(tk);
        NEXT_TOKEN(tk);
        err = term_booleano_handler();
        if (err) {
            return err;
        }
    } else {
        printf("          TermBooleano = ExpAritmetica TermBooleanoF\n");
        err = exp_aritmetica_handler();
        if (err) {
            return err;
        }

        err = term_booleano_F_handler();
        if (err) {
            return err;
        }
    }

    return 0;
}

static int exp_booleana_R_handler(void)
{
    int err = 0;

    if (tk.cat == OP_LOGIC) {
        printf("          ExpBooleanaR = 'op_logic' TermBooleano ExpBooleanaR\n");
        PRINT_TOKEN(tk);
        NEXT_TOKEN(tk);

        err = term_booleano_handler();
        if (err) {
            return err;
        }

        err = exp_booleana_R_handler();
        if (err) {
            return err;
        }

    } else {
        printf("          ExpBooleanaR = epsilon\n");
    }

    return 0;
}

static int exp_booleana_handler(void)
{
    int err = 0;
    printf("          ExpBooleana = TermBooleano ExpBooleanaR\n");
    err = term_booleano_handler();
    if (err) {
        return err;
    }

    err = exp_booleana_R_handler();
    if (err) {
        return err;
    }

    return 0;
}

static int exp_concat_R_handler(void)
{
    int err = 0;
    if (tk.cat == OP_CONCAT) {
        printf("          ExpConcatR = '<<' ExpBooleana ExpConcatR\n");
        PRINT_TOKEN(tk);
        NEXT_TOKEN(tk);

        err = exp_booleana_handler();
        if (err) {
            return err;
        }

        err = exp_concat_R_handler();
        if (err) {
            return err;
        }
    } else {
        printf("          ExpConcatR = epsilon\n");
    }
    return 0;
}

static int exp_concat_handler(void)
{
    int err = 0;
    printf("          ExpConcat = ExpBooleana ExpConcatR\n");
    err = exp_booleana_handler();
    if (err) {
        return err;
    }

    err = exp_concat_R_handler();
    if (err) {
        return err;
    }

    return 0;
}

static int lista_uni_R_handler(void)
{
    int err = 0;

    if (tk.cat == COMMA) {
        printf("          ListaUniR = ',' ExpConcat ListaUniR\n");
        PRINT_TOKEN(tk);
        NEXT_TOKEN(tk);

        err = exp_concat_handler();
        if (err) {
            return err;
        }

        err = lista_uni_R_handler();
        if (err) {
            return err;
        }

    } else {
        printf("          ListaUniR = epsilon\n");
    }

    return 0;
}

static int lista_uni_handler(void)
{
    int err = 0;
    printf("          ListaUni = ExpConcat ListaUniR\n");

    err = exp_concat_handler();
    if (err) {
        return err;
    }

    err = lista_uni_R_handler();
    if (err) {
        return err;
    }

    return 0;
}

static int variavel_FF_handler(void)
{
    int err = 0;

    if (tk.cat == OPEN_KEYS) {
        printf("          VariavelFF = '{' ListaUni '}'\n");
        PRINT_TOKEN(tk);
        NEXT_TOKEN(tk);

        err = lista_uni_handler();
        if (err) {
            return err;
        }

        if (tk.cat == CLOSE_KEYS) {
            PRINT_TOKEN(tk);
            NEXT_TOKEN(tk);
        } else {
            printf("Esperado um '}'\n");
            return -SYNTAX_ERROR;
        }
    } else {
        printf("          VariavelFF = epsilon\n");
    }

    return 0;
}

static int variavel_F_handler(void)
{
    int err = 0;
    if (CHECK_FIRST_SET_VARIAVEL_F(tk.cat)) {
        if (tk.cat == OPEN_BRACKETS) {
            printf("          VariavelF = '[' ExpConcat ']' VariavelFF\n");
            PRINT_TOKEN(tk);
            NEXT_TOKEN(tk);

            err = exp_concat_handler();
            if (err) {
                return err;
            }

            if (tk.cat == CLOSE_BRACKETS) {
                PRINT_TOKEN(tk);
                NEXT_TOKEN(tk);

                err = variavel_FF_handler();
                if (err) {
                    return err;
                }
            } else {
                printf("Esperado um '['\n");
                return -SYNTAX_ERROR;
            }
        } else {
            printf("          VariavelF = ExpConcat\n");
            err = exp_concat_handler();
            if (err) {
                return err;
            }
        }
    } else {
        printf("          VariavelF = epsilon\n");
    }
    return 0;
}

static int variavel_handler(void)
{
    int err = 0;
    printf("          Variavel = 'id' VariavelF\n");
    if (tk.cat == ID) {
        PRINT_TOKEN(tk);
        NEXT_TOKEN(tk);
        err = variavel_F_handler();
        if (err) {
            return err;
        }
    } else {
        printf("Esperado um ID\n");
        return -SYNTAX_ERROR;
    }
    return 0;
}

static int lista_variaveis_R_handler(void)
{
    int err = 0;

    if (tk.cat == COMMA) {
        printf("          ListaVariaveisR = ',' Variavel ListaVariaveisR\n");
        PRINT_TOKEN(tk);
        NEXT_TOKEN(tk);

        err = variavel_handler();
        if (err) {
            return err;
        }

        err = lista_variaveis_R_handler();
        if (err) {
            return err;
        }
    } else {
        printf("          ListaVariaveisR = epsilon\n");
    }
    return 0;
}

static int lista_variaveis_handler(void)
{
    int err = 0;
    printf("          ListaVariaveis = Variável ListaVariaveisR\n");
    err = variavel_handler();
    if (err) {
        return err;
    }

    err = lista_variaveis_R_handler();
    if (err) {
        return err;
    }

    return 0;
}

static int decl_variaveis_handler(void)
{
    int err = 0;

    printf("          DeclVariaveis = DefTipo ListaVariaveis ';'\n");

    err = def_tipo_handler();
    if (err) {
        return err;
    }

    err = lista_variaveis_handler();
    if (err) {
        return err;
    }

    if (tk.cat == SEMICOLON) {
        PRINT_TOKEN(tk);
        NEXT_TOKEN(tk);
    } else {
        printf("Esperado um ponto e vírgula\n");
        return -SYNTAX_ERROR;
    }

    return 0;
}

static int tipo_handler(void)
{
    int err = 0;

    switch (tk.cat) {
    case INT_TYPE:
        printf("          Tipo = 'int'\n");
        break;
    case CHAR_TYPE:
        printf("          Tipo = 'char'\n");
        break;
    case STRING_TYPE:
        printf("          Tipo = 'string'\n");
        break;
    case FLOAT_TYPE:
        printf("          Tipo = 'float'\n");
        break;
    case BOOL_TYPE:
        printf("          Tipo = 'bool'\n");
        break;
    default:
        printf("Esperado um tipo de variável\n");
        return -SYNTAX_ERROR;
    }

    PRINT_TOKEN(tk);
    NEXT_TOKEN(tk);


    return 0;
}

static int param_F_handler(void)
{
    int err = 0;

    if (tk.cat == OPEN_BRACKETS) {
        printf("          ParamF = '[' ']'\n");
        PRINT_TOKEN(tk);
        NEXT_TOKEN(tk);

        if (tk.cat == CLOSE_BRACKETS) {
            PRINT_TOKEN(tk);
            NEXT_TOKEN(tk);
        } else {
            printf("Esperado um '['\n");
            return -SYNTAX_ERROR;
        }
    } else {
        printf("          ParamF = epsilon\n");
    }

    return 0;
}

static int param_handler(void)
{
    int err = 0;
    printf("          Param = Tipo 'id' ParamF\n");
    err = tipo_handler();
    if (err) {
        return err;
    }

    if (tk.cat == ID) {
        PRINT_TOKEN(tk);
        NEXT_TOKEN(tk);

        err = param_F_handler();
        if (err) {
            return err;
        }
    } else {
        printf("Esperado um 'id'\n");
        return -SYNTAX_ERROR;
    }

    return 0;
}

static int lista_param_R_handler(void)
{
    int err = 0;

    if (tk.cat == COMMA) {
        printf("          ListaParamR = ',' Param ListaParamR");
        PRINT_TOKEN(tk);
        NEXT_TOKEN(tk);

        err = param_handler();
        if (err) {
            return err;
        }

        err = lista_param_R_handler();
        if (err) {
            return err;
        }
    } else {
        printf("          ListaParamR = epsilon\n");
    }

    return 0;
}

static int lista_param_handler(void)
{
    int err = 0;
    if (tk.cat == VOID) {
        printf("          ListaParam = 'void'\n");
        PRINT_TOKEN(tk);
        NEXT_TOKEN(tk);
    } else {
        printf("          ListaParam = Param ListaParamR\n");

        err = param_handler();
        if (err) {
            return err;
        }

        err = lista_param_R_handler();
        if (err) {
            return err;
        }
    }

    return 0;
}

static int tipo_return_handler(void)
{
    int err = 0;
    if (tk.cat == VOID) {
        printf("          TipoReturn = 'void'\n");
        PRINT_TOKEN(tk);
        NEXT_TOKEN(tk);
    } else {
        printf("          TipoReturn = Tipo\n");
        err = tipo_handler();
        if (err) {
            return err;
        }
    }

    return 0;
}

static int continue_handler(void)
{
    int err = 0;
    if (tk.cat == CONTINUE) {
        printf("          Continue = 'continue' ';'");
        PRINT_TOKEN(tk);
        NEXT_TOKEN(tk);

        if (tk.cat == SEMICOLON) {
            PRINT_TOKEN(tk);
            NEXT_TOKEN(tk);
        } else {
            printf("Esperado um ';'");
            return -SYNTAX_ERROR;
        }
    } else {
        printf("Esperado um 'continue'\n");
        return -SYNTAX_ERROR;
    }

    return 0;
}

static int break_handler(void)
{
    int err = 0;
    if (tk.cat == BREAK) {
        printf("          Break = 'break' ';'");
        PRINT_TOKEN(tk);
        NEXT_TOKEN(tk);

        if (tk.cat == SEMICOLON) {
            PRINT_TOKEN(tk);
            NEXT_TOKEN(tk);
        } else {
            printf("Esperado um ';'");
            return -SYNTAX_ERROR;
        }
    } else {
        printf("Esperado um 'break'\n");
        return -SYNTAX_ERROR;
    }

    return 0;
}

static int atribuicao_F_handler(void)
{
    int err = 0;

    if (tk.cat == OPEN_BRACKETS) {
        printf("          AtribuicaoF = '[' ExpConcat ']' ExpConcat ';'\n");
        PRINT_TOKEN(tk);
        NEXT_TOKEN(tk);

        err = exp_concat_handler();
        if (err) {
            return err;
        }

        if (tk.cat == CLOSE_BRACKETS) {
            PRINT_TOKEN(tk);
            NEXT_TOKEN(tk);

            err = exp_concat_handler();
            if (err) {
                return err;
            }

            if (tk.cat == SEMICOLON) {
                PRINT_TOKEN(tk);
                NEXT_TOKEN(tk);

            } else {
                printf("Esperado um ';'\n");
                return -SYNTAX_ERROR;
            }

        } else {
            printf("Esperado um ']'\n");
            return -SYNTAX_ERROR;
        }
    } else {
        printf("          AtribuicaoF = ExpConcat ';'\n");

        err = exp_concat_handler();
        if (err) {
            return err;
        }

        if (tk.cat == SEMICOLON) {
            PRINT_TOKEN(tk);
            NEXT_TOKEN(tk);
        } else {
            printf("Esperado um ';'\n");
            return -SYNTAX_ERROR;
        }
    }
    return 0;
}

static int atribuicao_handler(void)
{
    int err = 0;
    printf("          Atribuicao = 'set' 'id' AtribuicaoF\n");
    if (tk.cat == SET) {
        PRINT_TOKEN(tk);
        NEXT_TOKEN(tk);

        if (tk.cat == ID) {
            PRINT_TOKEN(tk);
            NEXT_TOKEN(tk);

            err = atribuicao_F_handler();
            if (err) {
                return err;
            }

        } else {
            printf("Esperado um 'id'\n");
            return -SYNTAX_ERROR;
        }
    } else {
        printf("Esperado um 'set'\n");
        return -SYNTAX_ERROR;
    }


    return 0;
}

static int lista_arg_R_handler(void)
{
    int err = 0;
    if (tk.cat == COMMA) {
        printf("          ListaArgR = ',' 'id' ListaArgR\n");
        PRINT_TOKEN(tk);
        NEXT_TOKEN(tk);

        if (tk.cat == ID) {
            PRINT_TOKEN(tk);
            NEXT_TOKEN(tk);

            err = lista_arg_R_handler();
            if (err) {
                return err;
            }
        } else {
            printf("Esperado um 'id'\n");
            return -SYNTAX_ERROR;
        }
    } else {
        printf("          ListaArgR = epsilon\n");
    }

    return 0;
}

static int lista_arg_handler(void)
{
    int err = 0;
    printf("          ListaArg = 'id' ListaArgR\n");
    if (tk.cat == ID) {
        PRINT_TOKEN(tk);
        NEXT_TOKEN(tk);

        err = lista_arg_R_handler();
        if (err) {
            return err;
        }
    } else {
        printf("Esperado 'id'\n");
        return -SYNTAX_ERROR;
    }
    return 0;
}

static int chamadafunc_handler(void)
{
    int err = 0;
    if (tk.cat == ID) {
        printf("          ChamadaFunc = 'id' '(' ListaArg ')' ';'\n");
        PRINT_TOKEN(tk);
        NEXT_TOKEN(tk);

        if (tk.cat == OPEN_PAREN) {
            PRINT_TOKEN(tk);
            NEXT_TOKEN(tk);

            err = lista_arg_handler();
            if (err) {
                return err;
            }

            if (tk.cat == CLOSE_PAREN) {
                PRINT_TOKEN(tk);
                NEXT_TOKEN(tk);

                if (tk.cat == SEMICOLON) {
                    PRINT_TOKEN(tk);
                    NEXT_TOKEN(tk);
                } else {
                    printf("Esperado um ';'\n");
                }
            } else {
                printf("Esperado ')'\n");
                return -SYNTAX_ERROR;
            }

        } else {
            printf("Esperado um '('\n");
            return -SYNTAX_ERROR;
        }
    } else {
        printf("Esperado um 'id'\n");
        return -SYNTAX_ERROR;
    }

    return 0;
}

static int return_handler(void)
{
    int err = 0;

    if (tk.cat == RETURN) {
        printf("          Return = 'return' ExpConcat ';'\n");
        PRINT_TOKEN(tk);
        NEXT_TOKEN(tk);

        err = exp_concat_handler();
        if (err) {
            return err;
        }

        if (tk.cat == SEMICOLON) {
            PRINT_TOKEN(tk);
            NEXT_TOKEN(tk);
        } else {
            printf("Esperado um ';'\n");
            return -SYNTAX_ERROR;
        }
    } else {
        printf("Esperado um 'return'\n");
        return -SYNTAX_ERROR;
    }
    return 0;
}

static int read_handler(void)
{
    int err = 0;

    if (tk.cat == READ) {
        printf("          Read = 'read' '(' ListaArg ')' ';'\n");
        PRINT_TOKEN(tk);
        NEXT_TOKEN(tk);

        if (tk.cat == OPEN_PAREN) {
            PRINT_TOKEN(tk);
            NEXT_TOKEN(tk);

            err = lista_arg_handler();
            if (err) {
                return err;
            }

            if (tk.cat == CLOSE_PAREN) {
                PRINT_TOKEN(tk);
                NEXT_TOKEN(tk);

                if (tk.cat == SEMICOLON) {
                    PRINT_TOKEN(tk);
                    NEXT_TOKEN(tk);
                } else {
                    printf("Esperado um ';'\n");
                }
            } else {
                printf("Esperado ')'\n");
                return -SYNTAX_ERROR;
            }

        } else {
            printf("Esperado um '('\n");
            return -SYNTAX_ERROR;
        }
    } else {
        printf("Esperado um 'read'\n");
    }
    return 0;
}

static int echo_F_handler(void)
{
    int err = 0;

    if (tk.cat == OPEN_PAREN) {
        printf("          EchoF = '(' ListaArg ')' ';'");
        PRINT_TOKEN(tk);
        NEXT_TOKEN(tk);

        err = lista_arg_handler();
        if (err) {
            return err;
        }

        if (tk.cat == CLOSE_PAREN) {
            PRINT_TOKEN(tk);
            NEXT_TOKEN(tk);

            if (tk.cat == SEMICOLON) {
                PRINT_TOKEN(tk);
                NEXT_TOKEN(tk);
            } else {
                printf("Esperado um ';'");
                return -SYNTAX_ERROR;
            }
        } else {
            printf("Esperado um ')'\n");
            return -SYNTAX_ERROR;
        }
    } else if (tk.cat == SEMICOLON) {
        printf("          EchoF = ';'\n");
        PRINT_TOKEN(tk);
        NEXT_TOKEN(tk);
    } else {
        printf("Esperado um ';' ou uma lista de argumentos entre parênteses\n");
        return -SYNTAX_ERROR;
    }

    return 0;
}

static int echo_handler(void)
{
    int err = 0;
    printf("          Echo = 'echo' '(' ExpConcat ')' EchoF\n");
    if (tk.cat == ECHO) {
        PRINT_TOKEN(tk);
        NEXT_TOKEN(tk);

        if (tk.cat == OPEN_PAREN) {
            PRINT_TOKEN(tk);
            NEXT_TOKEN(tk);

            err = exp_concat_handler();
            if (err) {
                return err;
            }

            if (tk.cat == CLOSE_PAREN) {
                PRINT_TOKEN(tk);
                NEXT_TOKEN(tk);

                err = echo_F_handler();
                if (err) {
                    return err;
                }
            } else {
                printf("Esperado um ')'\n");
                return -SYNTAX_ERROR;
            }
        } else {
            printf("Esperado um '('\n");
            return -SYNTAX_ERROR;
        }

    } else {
        printf("Esperado um 'echo'\n");
        return -SYNTAX_ERROR;
    }
    return 0;
}

static int else_handler(void)
{
    int err = 0;

    if (tk.cat == ELSE) {
        printf("          Else = 'else' '{' ListaSent '}'\n");
        PRINT_TOKEN(tk);
        NEXT_TOKEN(tk);

        if (tk.cat == OPEN_KEYS) {
            PRINT_TOKEN(tk);
            NEXT_TOKEN(tk);

            err = lista_sent_handler();
            if (err) {
                return err;
            }

            if (tk.cat == CLOSE_KEYS) {
                PRINT_TOKEN(tk);
                NEXT_TOKEN(tk);
            } else {
                printf("Esperado um '}'\n");
                return -SYNTAX_ERROR;
            }
        } else {
            printf("Esperado um '{'\n");
            return -SYNTAX_ERROR;
        }
    } else {
        printf("          Else = epsilon\n");
    }

    return 0;
}

static int if_handler(void)
{
    int err = 0;
    printf("          If = 'if' '(' ExpBooleana ')' '{' ListaSent '}' Else\n");
    if (tk.cat == IF) {
        PRINT_TOKEN(tk);
        NEXT_TOKEN(tk);

        if (tk.cat == OPEN_PAREN) {
            PRINT_TOKEN(tk);
            NEXT_TOKEN(tk);

            err = exp_booleana_handler();
            if (err) {
                return err;
            }

            if (tk.cat == CLOSE_PAREN) {
                PRINT_TOKEN(tk);
                NEXT_TOKEN(tk);

                if (tk.cat == OPEN_KEYS) {
                    PRINT_TOKEN(tk);
                    NEXT_TOKEN(tk);

                    err = lista_sent_handler();
                    if (err) {
                        return err;
                    }

                    if (tk.cat == CLOSE_KEYS) {
                        PRINT_TOKEN(tk);
                        NEXT_TOKEN(tk);

                        err = else_handler();
                        if (err) {
                            return err;
                        }
                    } else {
                        printf("Esperado um '}'\n");
                    }
                } else {
                    printf("Esperado um '{'\n");
                }
            } else {
                printf("Esperado um ')'\n");
                return -SYNTAX_ERROR;
            }
        } else {
            printf("Esperado um '('\n");
            return -SYNTAX_ERROR;
        }


    } else {
        printf("Esperado um 'if'\n");
        return -SYNTAX_ERROR;
    }
    return 0;
}

static int forpasso_handler(void)
{
    int err = 0;

    if (tk.cat == COMMA) {
        printf("          ForPasso = ',' ExpAritmetica \n");
        PRINT_TOKEN(tk);
        NEXT_TOKEN(tk);

        err = exp_aritmetica_handler();
        if (err) {
            return err;
        }
    } else {
        printf("          ForPasso = epsilon\n");
    }

    return 0;
}

static int for_handler(void)
{
    int err = 0;
    printf("          For = 'for' '(' 'id' ',' ExpAritmetica ',' Exparitmetica ForPasso ')' '{' "
           "ListaSent '}'\n");

    if (tk.cat == FOR) {
        PRINT_TOKEN(tk);
        NEXT_TOKEN(tk);

        if (tk.cat == OPEN_PAREN) {
            PRINT_TOKEN(tk);
            NEXT_TOKEN(tk);

            if (tk.cat == ID) {
                PRINT_TOKEN(tk);
                NEXT_TOKEN(tk);

                if (tk.cat == COMMA) {
                    PRINT_TOKEN(tk);
                    NEXT_TOKEN(tk);

                    err = exp_aritmetica_handler();
                    if (err) {
                        return err;
                    }
                    if (tk.cat == COMMA) {
                        PRINT_TOKEN(tk);
                        NEXT_TOKEN(tk);

                        err = exp_aritmetica_handler();
                        if (err) {
                            return err;
                        }

                        err = forpasso_handler();
                        if (err) {
                            return err;
                        }

                        if (tk.cat == CLOSE_PAREN) {
                            PRINT_TOKEN(tk);
                            NEXT_TOKEN(tk);

                            if (tk.cat == OPEN_KEYS) {
                                PRINT_TOKEN(tk);
                                NEXT_TOKEN(tk);

                                err = lista_sent_handler();
                                if (err) {
                                    return err;
                                }

                                if (tk.cat == CLOSE_KEYS) {
                                    PRINT_TOKEN(tk);
                                    NEXT_TOKEN(tk);
                                } else {
                                    printf("Esperado um '}'\n");
                                    return -SYNTAX_ERROR;
                                }
                            } else {
                                printf("Esperado um '}'\n");
                                return -SYNTAX_ERROR;
                            }
                        } else {
                            printf("Esperado um ')'\n");
                            return -SYNTAX_ERROR;
                        }

                    } else {
                        printf("Esperado um ','\n");
                        return -SYNTAX_ERROR;
                    }

                } else {
                    printf("Esperado um ','\n");
                    return -SYNTAX_ERROR;
                }
            } else {
                printf("Esperado um 'id'\n");
                return -SYNTAX_ERROR;
            }
        } else {
            printf("Esperado um '('\n");
            return -SYNTAX_ERROR;
        }
    } else {
        printf("Esperado um 'for'\n");
        return -SYNTAX_ERROR;
    }
    return 0;
}

static int while_handler(void)
{
    int err = 0;
    printf("          While = 'while' '(' ExpBooleana ')' '{' ListaSent '}'\n");
    if (tk.cat == WHILE) {
        PRINT_TOKEN(tk);
        NEXT_TOKEN(tk);

        if (tk.cat == OPEN_PAREN) {
            PRINT_TOKEN(tk);
            NEXT_TOKEN(tk);

            err = exp_booleana_handler();
            if (err) {
                return err;
            }

            if (tk.cat == CLOSE_PAREN) {
                PRINT_TOKEN(tk);
                NEXT_TOKEN(tk);

                if (tk.cat == OPEN_KEYS) {
                    PRINT_TOKEN(tk);
                    NEXT_TOKEN(tk);

                    err = lista_sent_handler();
                    if (err) {
                        return err;
                    }

                    if (tk.cat == CLOSE_KEYS) {
                        PRINT_TOKEN(tk);
                        NEXT_TOKEN(tk);

                    } else {
                        printf("Esperado um '}'\n");
                    }

                } else {
                    printf("Esperado um '{'\n");
                }

            } else {
                printf("Esperado um ')'\n");
                return -SYNTAX_ERROR;
            }
        } else {
            printf("Esperado um '('\n");
            return SYNTAX_ERROR;
        }
    } else {
        printf("Esperado um 'while'\n");
        return -SYNTAX_ERROR;
    }
    return 0;
}

static int comando_handler(void)
{
    int err = 0;

    if (CHECK_FIRST_SET_CONTINUE(tk.cat)) {
        printf("          Comando = Continue\n");
        err = continue_handler();
    } else if (CHECK_FIRST_SET_BREAK(tk.cat)) {
        printf("          Comando = Break\n");
        err = break_handler();
    } else if (CHECK_FIRST_SET_ATRIBUICAO(tk.cat)) {
        printf("          Comando = Atribuicao\n");
        err = atribuicao_handler();
    } else if (CHECK_FIRST_SET_CHAMADAFUNC(tk.cat)) {
        printf("          Comando = ChamadaFunc\n");
        err = chamadafunc_handler();
    } else if (CHECK_FIRST_SET_RETURN(tk.cat)) {
        printf("          Comando = Return\n");
        err = return_handler();
    } else if (CHECK_FIRST_SET_READ(tk.cat)) {
        printf("          Comando = Read\n");
        err = read_handler();
    } else if (CHECK_FIRST_SET_ECHO(tk.cat)) {
        printf("          Comando = Echo\n");
        err = echo_handler();
    } else if (CHECK_FIRST_SET_IF(tk.cat)) {
        printf("          Comando = If\n");
        err = if_handler();
    } else if (CHECK_FIRST_SET_FOR(tk.cat)) {
        printf("          Comando = For\n");
        err = for_handler();
    } else if (CHECK_FIRST_SET_WHILE(tk.cat)) {
        printf("          Comando = While\n");
        err = while_handler();
    } else {
        printf("Esperado um comando\n");
        return -SYNTAX_ERROR;
    }

    if (err) {
        return err;
    }

    return 0;
}

static int sent_handler(void)
{
    int err = 0;
    if (CHECK_FIRST_SET_DECL_VARIAVEIS(tk.cat)) {
        printf("          Sent = DeclVariaveis\n");
        err = decl_variaveis_handler();
        if (err) {
            return err;
        }
    } else if (CHECK_FIRST_SET_COMANDO(tk.cat)) {
        printf("          Sent = Comando\n");
        err = comando_handler();
        if (err) {
            return err;
        }
    } else {
        printf("Esperado uma sentença\n");
        return -SYNTAX_ERROR;
    }

    return 0;
}

static int lista_sent_R_handler(void)
{
    int err = 0;

    if (CHECK_FIRST_SET_DECL_VARIAVEIS(tk.cat) || CHECK_FIRST_SET_COMANDO(tk.cat)) {
        err = sent_handler();
        if (err) {
            return err;
        }

        err = lista_sent_R_handler();
        if (err) {
            return err;
        }
    } else {
        printf("          ListaSentR = epsilon\n");
    }
    return 0;
}

static int lista_sent_handler(void)
{
    int err = 0;
    printf("          ListaSent = Sent ListaSentR\n");

    err = sent_handler();
    if (err) {
        return err;
    }

    err = lista_sent_R_handler();
    if (err) {
        return err;
    }

    return 0;
}

static int decl_funcao_handler(void)
{
    int err = 0;
    printf("          DeclFuncao = 'defun' 'id' '(' ListaParam ')' '(' TipoReturn ')' '{' "
           "ListaSent '}'\n");
    if (tk.cat == DEFUN) {
        PRINT_TOKEN(tk);
        NEXT_TOKEN(tk);
        if (tk.cat == ID) {
            PRINT_TOKEN(tk);
            NEXT_TOKEN(tk);
            if (tk.cat == OPEN_PAREN) {
                PRINT_TOKEN(tk);
                NEXT_TOKEN(tk);

                err = lista_param_handler();
                if (err) {
                    return err;
                }

                if (tk.cat == CLOSE_PAREN) {
                    PRINT_TOKEN(tk);
                    NEXT_TOKEN(tk);
                    if (tk.cat == OPEN_PAREN) {
                        PRINT_TOKEN(tk);
                        NEXT_TOKEN(tk);
                        err = tipo_return_handler();
                        if (err) {
                            return err;
                        }

                        if (tk.cat == CLOSE_PAREN) {
                            PRINT_TOKEN(tk);
                            NEXT_TOKEN(tk);

                            if (tk.cat == OPEN_KEYS) {
                                PRINT_TOKEN(tk);
                                NEXT_TOKEN(tk);

                                err = lista_sent_handler();
                                if (err) {
                                    return err;
                                }

                                if (tk.cat == CLOSE_KEYS) {
                                    PRINT_TOKEN(tk);
                                    NEXT_TOKEN(tk);
                                } else {
                                    printf("Esperado um '}'\n");
                                }
                            } else {
                                printf("Esperado um '{'\n");
                                return -SYNTAX_ERROR;
                            }
                        } else {
                            printf("Esperado um ')'\n");
                            return -SYNTAX_ERROR;
                        }
                    } else {
                        printf("Esperado um '('\n");
                        return -SYNTAX_ERROR;
                    }
                } else {
                    printf("Esperado um ')'\n");
                    return -SYNTAX_ERROR;
                }
            } else {
                printf("Esperado um '('\n");
                return -SYNTAX_ERROR;
            }
        } else {
            printf("Esperado um 'id'\n");
            return -SYNTAX_ERROR;
        }
    } else {
        printf("Esperado um 'defun'\n");
        return -SYNTAX_ERROR;
    }
    return 0;
}

static int decl_handler(void)
{
    int err = 0;
    if (CHECK_FIRST_SET_DECL_VARIAVEIS(tk.cat)) {
        printf("          Decl = DeclVariaveis\n");
        err = decl_variaveis_handler();
        if (err) {
            return err;
        }
    } else if (tk.cat == DEFUN) {
        printf("          Decl = DeclFuncao\n");
        err = decl_funcao_handler();
        if (err) {
            return err;
        }
    } else {
        printf("Esperado uma declaração de função ou de variável.\n");
        return -SYNTAX_ERROR;
    }

    return 0;
}

static int lista_decl_R_handler(void)
{
    int err = 0;
    if (CHECK_FIRST_SET_DECL(tk.cat)) {
        printf("          ListaDeclR = Decl ListaDeclR\n");
        err = decl_handler();
        if (err) {
            return err;
        }

        err = lista_decl_R_handler();
        if (err) {
            return err;
        }
    } else {
        PRINT_TOKEN(tk);
        printf("          ListaDeclR = epsilon\n");
    }
    return 0;
}

static int lista_decl_handler(void)
{
    int err = 0;
    printf("          ListaDecl = Decl ListaDeclR\n");

    err = decl_handler();
    if (err) {
        return err;
    }

    err = lista_decl_R_handler();
    if (err) {
        return err;
    }

    return 0;
}

static int programa_handler(void)
{
    printf("          Programa = ListaDecl\n");
    return lista_decl_handler();
}

int parser_run(void)
{
    NEXT_TOKEN(tk);
    int err = programa_handler();
    if (tk.cat != CAT_EOF) {
        printf("A análise terminou pela falta de uma declaração\n");
    }

    return err;
}
