#ifndef STAT_OPERATORS
#define STAT_OPERATORS
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "Rmath.h"

//basic R
int operator_qnorm(char **arrayvals, int arraypositions[]);

//Zscore 
int operator_beta_se_2_zscore(char **arrayvals, int arraypositions[]);
int operator_pval_oddsratio_2_zscore(char **arrayvals, int arraypositions[]);
int operator_pval_beta_2_zscore(char **arrayvals, int arraypositions[]);
int operator_pval_beta_N_2_zscore(char **arrayvals, int arraypositions[]);
int generate_header(char *operator_name, int indexcolumn);

//Pvalue
int operator_zscore_N_2_pvalue(char **arrayvals, int arraypositions[]);
int operator_zscore_2_pvalue(char **arrayvals, int arraypositions[]);


#endif
