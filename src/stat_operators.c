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
Check if operator is available
******************************/

void *populate_array(int (**p)(char**, int*), char *operator_inname, int inx, char *statmodel) {

  // Declare the different functions we can use as operators
  if(strcmp(statmodel, "lin") == 0 ){
    if (strcmp(operator_inname, "qnorm") == 0) {
      p[inx] = lin_operator_qnorm;
    } else if (strcmp(operator_inname, "zscore_from_pval_oddsratio") == 0) {
      p[inx] = lin_operator_zscore_from_pval_oddsratio;
    } else if (strcmp(operator_inname, "zscore_from_pval_beta") == 0) {
      p[inx] = lin_operator_zscore_from_pval_beta;
    } else if (strcmp(operator_inname, "zscore_from_pval_beta_N") == 0) {
      p[inx] = lin_operator_zscore_from_pval_beta_N;
    } else if (strcmp(operator_inname, "zscore_from_beta_se") == 0) {
      p[inx] = lin_operator_zscore_from_beta_se;
    } else if (strcmp(operator_inname, "pval_from_zscore_N") == 0) {
      p[inx] = lin_operator_pval_from_zscore_N;
    } else if (strcmp(operator_inname, "pval_from_zscore") == 0) {
      p[inx] = lin_operator_pval_from_zscore;
    } else if (strcmp(operator_inname, "beta_from_zscore_se") == 0) {
      p[inx] = lin_operator_beta_from_zscore_se;
    } else if (strcmp(operator_inname, "beta_from_zscore_N_af") == 0) {
      p[inx] = lin_operator_beta_from_zscore_N_af;
    } else if (strcmp(operator_inname, "se_from_zscore_beta") == 0) {
      p[inx] = lin_operator_se_from_zscore_beta;
    } else if (strcmp(operator_inname, "se_from_zscore_N_af") == 0) {
      p[inx] = lin_operator_se_from_zscore_N_af;
    } else if (strcmp(operator_inname, "N_from_zscore_beta_af") == 0) {
      p[inx] = lin_operator_N_from_zscore_beta_af;
    } else {
      fprintf(stderr, "[ERROR] Unknown function: %s", operator_inname);
    }
  }

  return p;

}

/******************************
Compute per-SNP default Rmath R functions
******************************/

//Compute qnorm directly from the Rmath library
int lin_operator_qnorm(char **arrayvals, int arraypositions[]) {
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

  printf("%lf", qnorm(prob, 0.0, 1.0, 1, 0));

  return 0;
}

/******************************
Compute per-SNP test statistics
******************************/

// z-score from beta and stderror (linear regression)
/* 
  Use the Beta and standard error to convert Beta/SE -> t
*/
int lin_operator_zscore_from_beta_se(char **arrayvals, int arraypositions[]) {
  double beta = strtod(arrayvals[arraypositions[2]], NULL);
  double stderror = strtod(arrayvals[arraypositions[3]], NULL);

  if (errno != 0) {
    perror("[ERROR] Failed to parse floating point number");
    errno = 0;

    return 1;
  }

  printf("%lf", beta/stderror);

  return 0;
}

// z-score from pvalue and oddsratio (logistic regression only)
/* 
  Need to fill in details here
  Need to fill in details here
*/
int lin_operator_zscore_from_pval_oddsratio(char **arrayvals, int arraypositions[]) {
  double or = strtod(arrayvals[arraypositions[1]], NULL);
  double prob = strtod(arrayvals[arraypositions[0]], NULL);

  if (errno != 0) {
    perror("[ERROR] Failed to parse floating point number");
    errno = 0;

    return 1;
  }

  //sign funciton to get -1,0,1
  int sign = (log(or) > 0) - (log(or) < 0);
  printf("%lf", sign*fabs(qnorm(prob, 0.0, 1.0, 1, 0)));

  return 0;
}
 
// Z-score from pvalue and beta (linear regression)
/* 
  Use a z-approximation to convert P -> t
  Will have slight error that decreases as a function of sample size.
  Does not account estimated variance of beta
  Shrunken towards 0 relative to truth as a function of absolute magnitude of truth 
*/
int lin_operator_zscore_from_pval_beta(char **arrayvals, int arraypositions[]) {
  double prob = strtod(arrayvals[arraypositions[0]], NULL);
  double beta = strtod(arrayvals[arraypositions[2]], NULL);
  //long double beta = strtold(arrayvals[arraypositions[2]], NULL);
  //printf("%s", arrayvals[arraypositions[2]]);
  //printf("%Lf", beta);
 
  if (errno != 0) {
    perror("[ERROR] Failed to parse floating point number");
    errno = 0;

    return 1;
  }

  //sign funciton to get -1,0,1
  int sign = (beta > 0) - (beta < 0);
  printf("%lf", sign*fabs(qnorm(prob/2, 0.0, 1.0, 1, 0)));

  return 0;
}

// Z-score from pvalue, beta and N (linear regression)
/* 
  Use the N (must be per SNP N reported in sumstats file ) to convert P -> t
  Does not account for loss of degrees of freedom due to covariates
*/
int lin_operator_zscore_from_pval_beta_N(char **arrayvals, int arraypositions[]) {
  double Nindividuals = strtod(arrayvals[arraypositions[4]], NULL);
  double prob = strtod(arrayvals[arraypositions[0]], NULL);
  double beta = strtod(arrayvals[arraypositions[2]], NULL);

  if (errno != 0) {
    perror("[ERROR] Failed to parse floating point number");
    errno = 0;

    return 1;
  }

  //sign funciton to get -1,0,1
  int sign = (beta > 0) - (beta < 0);
  printf("%lf", sign*fabs( qt( prob/2, Nindividuals-2, 1, 0)));

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
int lin_operator_pval_from_zscore_N(char **arrayvals, int arraypositions[]) {
  double Nindividuals = strtod(arrayvals[arraypositions[4]], NULL);
  double zscore = strtod(arrayvals[arraypositions[5]], NULL);

  if (errno != 0) {
    perror("[ERROR] Failed to parse floating point number");
    errno = 0;

    return 1;
  }

  printf("%lf", 2*pt(-1*fabs(zscore), Nindividuals-2, 1, 0));
  
  return 0;
}


// pvalue from Z (linear regression)
/* 
  Use the test statistics and the z-distribution
  Ignores that SE uses sample variance
  bigger errors that might result from floating point storage? seem to peak around 0.1?
*/
int lin_operator_pval_from_zscore(char **arrayvals, int arraypositions[]) {
  double zscore = strtod(arrayvals[arraypositions[5]], NULL);

  if (errno != 0) {
    perror("[ERROR] Failed to parse floating point number");
    errno = 0;

    return 1;
  }

  printf("%lf", 2*pnorm( -fabs( zscore ), 0, 1, 1, 0 ));

  return 0;
}


// beta from zscore and se (linear regression)
/* 
   Use the test statistics and the estimated SE to infer effect size
*/
int lin_operator_beta_from_zscore_se(char **arrayvals, int arraypositions[]) {
  double stderror = strtod(arrayvals[arraypositions[3]], NULL);
  double zscore = strtod(arrayvals[arraypositions[5]], NULL);

  if (errno != 0) {
    perror("[ERROR] Failed to parse floating point number");
    errno = 0;

    return 1;
  }

  printf("%lf", zscore*stderror);

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
int lin_operator_beta_from_zscore_N_af(char **arrayvals, int arraypositions[]) {
  double zscore = strtod(arrayvals[arraypositions[5]], NULL);
  long double Nindividuals = strtod(arrayvals[arraypositions[4]], NULL);
  double af = strtod(arrayvals[arraypositions[6]], NULL);

  if (errno != 0) {
    perror("[ERROR] Failed to parse floating point number");
    errno = 0;

    return 1;
  }

  printf("%lf", zscore/sqrt(2*af*(1-af)*(Nindividuals+zscore*zscore)) );

  return 0;
}


// se from zscore and beta (linear regression)
/* 
 * Use the test statistics and the estimated effects to infer SE
*/
int lin_operator_se_from_zscore_beta(char **arrayvals, int arraypositions[]) {
  double zscore = strtod(arrayvals[arraypositions[5]], NULL);
  double beta = strtod(arrayvals[arraypositions[2]], NULL);

  if (errno != 0) {
    perror("[ERROR] Failed to parse floating point number");
    errno = 0;

    return 1;
  }

  printf("%lf", beta/zscore );

  return 0;
}


// se from zscore and beta (linear regression)
/* 
  Use the test statistic, sample size, and allele frequency to infer standard errors
  Assumptions:
  Error creeps in from var(SNP) not being 2p(1-p),
  Shows upward bias in test data.
  Citation: Supplement of https://www.nature.com/articles/ng.3538
*/
int lin_operator_se_from_zscore_N_af(char **arrayvals, int arraypositions[]) {
  double zscore = strtod(arrayvals[arraypositions[5]], NULL);
  double Nindividuals = strtod(arrayvals[arraypositions[4]], NULL);
  double allelefreq = strtod(arrayvals[arraypositions[6]], NULL);

  if (errno != 0) {
    perror("[ERROR] Failed to parse floating point number");
    errno = 0;

    return 1;
  }

  printf("%lf", 1/sqrt(2*allelefreq*(1-allelefreq)*(Nindividuals+zscore*zscore)) );

  return 0;
}

// N from zscore, beta and af (linear regression)
/* 
 Derived the sample size from beta, z, and allele frequency
 Error can come in from:
   1) Assumption that var(SNP) = 2p(1-p) / e.g., HWE not true, SNPs standardized, reference p not good
 Shows upward bias in test data.
*/
int lin_operator_N_from_zscore_beta_af(char **arrayvals, int arraypositions[]) {
  double zscore = strtod(arrayvals[arraypositions[5]], NULL);
  double beta = strtod(arrayvals[arraypositions[2]], NULL);
  double allelefreq = strtod(arrayvals[arraypositions[6]], NULL);

  if (errno != 0) {
    perror("[ERROR] Failed to parse floating point number");
    errno = 0;

    return 1;
  }

  printf("%lf", ((zscore*zscore)/(2*allelefreq*(1-allelefreq)*beta*beta)) - zscore*zscore );

  return 0;
}

