#ifndef STAT_OPERATORS
#define STAT_OPERATORS
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "Rmath.h"

//utils
void *populate_array(int (**p)(char**, int*), char *operator_inname, int inx);

//basic R
int operator_qnorm(char **arrayvals, int arraypositions[]);

//Zscore 
int operator_zscore_from_beta_se(char **arrayvals, int arraypositions[]);
int operator_zscore_from_pval_oddsratio(char **arrayvals, int arraypositions[]);
int operator_zscore_from_pval_beta(char **arrayvals, int arraypositions[]);
int operator_zscore_from_pval_beta_N(char **arrayvals, int arraypositions[]);

//Pvalue
int operator_pval_from_zscore_N(char **arrayvals, int arraypositions[]);
int operator_pval_from_zscore(char **arrayvals, int arraypositions[]);

//Beta (not ready)
int operator_beta_from_zscore_se(char **arrayvals, int arraypositions[]);
int operator_beta_from_zscore_N_af(char **arrayvals, int arraypositions[]);

//SE (not ready)
int operator_se_from_zscore_beta(char **arrayvals, int arraypositions[]);
int operator_se_from_zscore_N_af(char **arrayvals, int arraypositions[]);

//N (not ready)
int operator_N_from_zscore_beta_af(char **arrayvals, int arraypositions[]);


#endif
