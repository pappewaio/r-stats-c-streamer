#include "stat_operators.h"

/**********************************************
STATISTICS functions section
***********************************************/
/*ARITHMETICS good to know
- fabs, the floating number version of abs
- log, natural logarithm
- log10, logarithm with base10
*/

/******************************
Generate header function
 Always generate a header, but depending on function and if inclusion of index
 it can look a bit different.
******************************/
int generate_header(char *operator_name, int indexcolumn) {

  if (errno != 0) {
    perror("[ERROR] Failed to parse character input");
    errno = 0;

    return 1;
  }

  // Add index column if specified
    if (indexcolumn != 0) {
      printf("%s\t", "0");
    }

  // Make new header based on function name
  if (strcmp(operator_name, "qnorm") == 0) {
      printf("%s\n", "QNORM");
  } else if (
      strcmp(operator_name, "beta_se_2_zscore") == 0 ||
      strcmp(operator_name, "pval_oddsratio_2_zscore") == 0 ||
      strcmp(operator_name, "pval_beta_2_zscore") == 0 ||
      strcmp(operator_name, "pval_beta_N_2_zscore") == 0 
    ) {
      printf("%s\n", "ZSCORE");
  } else if (
      strcmp(operator_name, "zscore_N_2_pvalue") == 0 ||
      strcmp(operator_name, "zscore_2_pvalue") == 0 
    ) {
      printf("%s\n", "PVALUE");
  } else if (
      strcmp(operator_name, "zscore_se_2_beta") == 0 ||
      strcmp(operator_name, "zscore_N_af_2_beta") == 0 
    ) {
      printf("%s\n", "BETA");
  } else {
    fprintf(stderr, "[ERROR] Cannot make new header, unknown function: %s\n", operator_name);
  }

  return 0;
}



/******************************
Compute per-SNP default Rmath R functions
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
Compute per-SNP P-values
******************************/

// pvalue from Z and N (linear regression)
/* 
  Use the test statistics and the sample size to reference the t-distribution
  Does not account for degrees of freedom lost to covariates in variance estimation
*/
int operator_zscore_N_2_pvalue(char **arrayvals, int arraypositions[]) {
  double Nindividuals = strtod(arrayvals[arraypositions[4]], NULL);
  double zscore = strtod(arrayvals[arraypositions[5]], NULL);

  if (errno != 0) {
    perror("[ERROR] Failed to parse floating point number");
    errno = 0;

    return 1;
  }

  printf("%lf\n", 2*pt(-fabs(zscore), Nindividuals-2, 1, 0));

  return 0;
}


// pvalue from Z (linear regression)
/* 
  Use the test statistics and the z-distribution
  Ignores that SE uses sample variance
  bigger errors that might result from floating point storage? seem to peak around 0.1?
*/
int operator_zscore_2_pvalue(char **arrayvals, int arraypositions[]) {
  double zscore = strtod(arrayvals[arraypositions[5]], NULL);

  if (errno != 0) {
    perror("[ERROR] Failed to parse floating point number");
    errno = 0;

    return 1;
  }

  printf("%lf\n", 2*pnorm( -abs( zscore ), 0, 1, 1, 0 ));

  return 0;
}


// beta from zscore and se (linear regression)
/* 
   Use the test statistics and the estimated SE to infer effect size
*/
int operator_zscore_se_2_beta(char **arrayvals, int arraypositions[]) {
  double stderror = strtod(arrayvals[arraypositions[3]], NULL);
  double zscore = strtod(arrayvals[arraypositions[5]], NULL);

  if (errno != 0) {
    perror("[ERROR] Failed to parse floating point number");
    errno = 0;

    return 1;
  }

  printf("%lf\n", zscore*stderror);

  return 0;
}

// beta from zscore, N and AF (linear regression)
/* 
   Use the test statistic, sample size, and allele frequency to infer effect sizes
   Note: does not recover the original effects sizes, but rather, a standardized effect size
		B_hat <- Beta / sd( pheno )
   Citation: Supplement of https://www.nature.com/articles/ng.3538
   Note: Ignores covariate effects and adjustment for degrees of freedom from estimation
*/
int operator_zscore_N_af_2_beta(char **arrayvals, int arraypositions[]) {
  double zscore = strtod(arrayvals[arraypositions[5]], NULL);
  double Nindividuals = strtod(arrayvals[arraypositions[4]], NULL);
  double af = strtod(arrayvals[arraypositions[6]], NULL);

  if (errno != 0) {
    perror("[ERROR] Failed to parse floating point number");
    errno = 0;

    return 1;
  }

  printf("%lf\n", zscore/sqrt(2*af*(1-af)*(Nindividuals+zscore*zscore)) );

  return 0;
}


