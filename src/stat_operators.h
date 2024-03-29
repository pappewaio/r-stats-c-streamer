#ifndef STAT_OPERATORS
#define STAT_OPERATORS
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "Rmath.h"

//utils
void *populate_array(int (**p)(char**, int*, int*), char *operator_inname, int inx, char *statmodel);

//basic R
int lin_operator_qnorm(char **arrayvals, int arraypositions[], int valmodifier[]);
int none_pval_from_neglog10p(char **arrayvals, int arraypositions[], int valmodifier[]);
int none_pval_from_log10p(char **arrayvals, int arraypositions[], int valmodifier[]);


//Zscore 
int lin_operator_zscore_from_beta_se(char **arrayvals, int arraypositions[], int valmodifier[]);
int lin_operator_zscore_from_pval_beta(char **arrayvals, int arraypositions[], int valmodifier[]);
int lin_operator_zscore_from_pval_beta_N(char **arrayvals, int arraypositions[], int valmodifier[]);
int log_operator_zscore_from_pval_oddsratio(char **arrayvals, int arraypositions[], int valmodifier[]);
int log_operator_zscore_from_beta_se(char **arrayvals, int arraypositions[], int valmodifier[]);

//Pvalue
int lin_operator_pval_from_zscore_N(char **arrayvals, int arraypositions[], int valmodifier[]);
int lin_operator_pval_from_zscore(char **arrayvals, int arraypositions[], int valmodifier[]);
int log_operator_pval_from_zscore(char **arrayvals, int arraypositions[], int valmodifier[]);

//Beta
int lin_operator_beta_from_zscore_se(char **arrayvals, int arraypositions[], int valmodifier[]);
int lin_operator_beta_from_zscore_N_af(char **arrayvals, int arraypositions[], int valmodifier[]);
int log_operator_beta_from_oddsratio(char **arrayvals, int arraypositions[], int valmodifier[]);
int log_operator_beta_from_zscore_se(char **arrayvals, int arraypositions[], int valmodifier[]);

//SE
int lin_operator_se_from_zscore_beta(char **arrayvals, int arraypositions[], int valmodifier[]);
int lin_operator_se_from_zscore_N_af(char **arrayvals, int arraypositions[], int valmodifier[]);
int log_operator_se_from_beta_zscore(char **arrayvals, int arraypositions[], int valmodifier[]); 
int log_operator_se_from_OR_u95_OR_l95(char **arrayvals, int arraypositions[], int valmodifier[]);

//N
int lin_operator_N_from_zscore_beta_af(char **arrayvals, int arraypositions[], int valmodifier[]);

//Neff
int log_operator_Neff_from_Nca_Nco(char **arrayvals, int arraypositions[], int valmodifier[]);

//AF
int log_operator_AF_from_CaseAF_ControlAF(char **arrayvals, int arraypositions[], int valmodifier[]);

#endif

