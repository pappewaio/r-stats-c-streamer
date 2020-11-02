#include "stat_operators.h"

/**********************************************
Functions section, should be moved to own file
***********************************************/
/*ARITHMETICS good to know
- fabs, the floating number version of abs
- log, natural logarithm
- log10, logarithm with base10
*/

/******************************
Compute per-SNP Rmath R functions
******************************/

//Compute qnorm directly from the Rmath library
int operator_qnorm(char **arrayvals, int arraypositions[]) {
  //take out element 1 which should be the pvalue
  double prob = strtod(arrayvals[arraypositions[0]], NULL);

  if (errno != 0) {
    perror("[ERROR] Failed to parse floating point number");
    errno = 0;

    return 1;
  } else if (prob == 0.0) {
    fprintf(stderr, "[ERROR] Given line was not a floating point number: %s", arrayvals[arraypositions[0]]);

    return 1;
  }

  printf("%lf\n", qnorm(prob, 0.0, 1.0, 1, 0));

  return 0;
}

/******************************
Compute per-SNP test statistics
******************************/

// z-score from beta and stderror (linear regression)
/* 
  Use the Beta and standard error to convert Beta/SE -> t
*/
int operator_beta_se_2_zscore(char **arrayvals, int arraypositions[]) {
  double beta = strtod(arrayvals[arraypositions[2]], NULL);
  double stderror = strtod(arrayvals[arraypositions[3]], NULL);

  if (errno != 0) {
    perror("[ERROR] Failed to parse floating point number");
    errno = 0;

    return 1;
  }

  printf("%lf\n", beta/stderror);

  return 0;
}

// z-score from pvalue and oddsratio (logistic regression only)
/* 
  Need to fill in details here
  Need to fill in details here
*/
int operator_pval_oddsratio_2_zscore(char **arrayvals, int arraypositions[]) {
  double or = strtod(arrayvals[arraypositions[1]], NULL);
  double prob = strtod(arrayvals[arraypositions[0]], NULL);

  if (errno != 0) {
    perror("[ERROR] Failed to parse floating point number");
    errno = 0;

    return 1;
  }

  //sign funciton to get -1,0,1
  int sign = (log(or) > 0) - (log(or) < 0);
  printf("%lf\n", sign*fabs(qnorm(prob, 0.0, 1.0, 1, 0)));

  return 0;
}
 
// Z-score from pvalue and beta (linear regression)
/* 
  Use a z-approximation to convert P -> t
  Will have slight error that decreases as a function of sample size.
  Does not account estimated variance of beta
  Shrunken towards 0 relative to truth as a function of absolute magnitude of truth 
*/
int operator_pval_beta_2_zscore(char **arrayvals, int arraypositions[]) {
  double prob = strtod(arrayvals[arraypositions[0]], NULL);
  double beta = strtod(arrayvals[arraypositions[2]], NULL);

  if (errno != 0) {
    perror("[ERROR] Failed to parse floating point number");
    errno = 0;

    return 1;
  }

  //sign funciton to get -1,0,1
  int sign = (log(beta) > 0) - (log(beta) < 0);
  printf("%lf\n", sign*fabs(qnorm(prob, 0.0, 1.0, 1, 0)));

  return 0;
}

// Z-score from pvalue, beta and N (linear regression)
/* 
  Use the N (must be per SNP N reported in sumstats file ) to convert P -> t
  Does not account for loss of degrees of freedom due to covariates
*/
int operator_pval_beta_N_2_zscore(char **arrayvals, int arraypositions[]) {
  double Nindividuals = strtod(arrayvals[arraypositions[4]], NULL);
  double prob = strtod(arrayvals[arraypositions[0]], NULL);
  double beta = strtod(arrayvals[arraypositions[2]], NULL);

  if (errno != 0) {
    perror("[ERROR] Failed to parse floating point number");
    errno = 0;

    return 1;
  }

  //sign funciton to get -1,0,1
  int sign = (log(beta) > 0) - (log(beta) < 0);
  printf("%lf\n", sign*fabs( qt( prob/2, Nindividuals-2, 1, 0)));

  return 0;
}

/******************************
Generate header function
******************************/
int generate_header(char *operator_name, int indexcolumn) {

  if (errno != 0) {
    perror("[ERROR] Failed to parse character input");
    errno = 0;

    return 1;
  }

  // Make new header based on function name
  if (strcmp(operator_name, "qnorm") == 0) {
    if (indexcolumn == 0) {
      printf("%s\n", "QNORM");
    } else {
      printf("%s\t%s\n", "0", "QNORM");
    }
  } else if (
      strcmp(operator_name, "beta_se_2_zscore") == 0 ||
      strcmp(operator_name, "pval_oddsratio_2_zscore") == 0 ||
      strcmp(operator_name, "pval_beta_2_zscore") == 0 ||
      strcmp(operator_name, "pval_beta_N_2_zscore") == 0 
    ) {
    if (indexcolumn == 0) {
      printf("%s\n", "ZSCORE");
    } else {
      printf("%s\t%s\n", "0", "ZSCORE");
    }
  } else {
    fprintf(stderr, "[ERROR] Cannot make new header, unknown function: %s\n", operator_name);
  }

  return 0;
}


