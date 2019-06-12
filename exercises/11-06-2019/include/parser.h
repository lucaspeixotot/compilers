/**
 * @file parser.h
 * @brief 
 * @author Lucas Peixoto (lucaspeixotoac@gmail.com)
 * @version 
 * @date 2019-06-11
 */

#ifndef _PARSER_H_
#define _PARSER_H_

enum category {
    C_NONE,
    C_PT,
    C_EOF,
    C_PTV,
    C_SE,
    C_ENTAO,
    C_FIM,
    C_SENAO,
    C_PARA,
    C_ATE,
    C_REPITA,
    C_ID,
    C_EQUAL,
    C_OPL,
    C_NAO,
    C_VERD,
    C_FALSO,
    C_OPA,
    C_OPM,
    C_ABREPAREN,
    C_FECHAPAREN,
    C_CTE,
};

/**
 * @brief This function deals with the program.
 *
 * @return errno
 */
int program_solver(void);


/**
 * @brief This function delas with lsent non terminal
 *
 * @return errno
 */
int lsent_handler(void);


/**
 * @brief This function delas with lsentr non terminal
 *
 * @return errno
 */
int lsentr_handler(void);


/**
 * @brief This function delas with sent non terminal
 *
 * @return errno
 */
int sent_handler(void);

/**
 * @brief This function delas with atr non terminal
 *
 * @return errno
 */
int atr_handler(void);


/**
 * @brief This function delas with expr non terminal
 *
 * @return errno
 */
int expr_handler(void);


/**
 * @brief This function delas with eb non terminal
 *
 * @return errno
 */
int eb_handler(void);

/**
 * @brief This function delas with ebr non terminal
 *
 * @return errno
 */
int ebr_handler(void);

/**
 * @brief This function delas with tb non terminal
 *
 * @return errno
 */
int tb_handler(void);

/**
 * @brief This function delas with ea non terminal
 *
 * @return errno
 */
int ea_handler(void);

/**
 * @brief This function delas with ear non terminal
 *
 * @return errno
 */
int ear_handler(void);

/**
 * @brief This function delas with ta non terminal
 *
 * @return errno
 */
int ta_handler(void);

/**
 * @brief This function delas with tar non terminal
 *
 * @return errno
 */
int tar_handler(void);

/**
 * @brief This function delas with fa non terminal
 *
 * @return errno
 */
int fa_handler(void);

#endif
