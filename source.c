//This script split arguments based on tab separator
//strtok() will consider any sequence of tabs a single delimiter. This can
// be fine for this application as the pipeline can introduce NAs early in
// the workflow
//#include <unistd.h>  
//
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include "Rmath.h"

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
  } else if (strcmp(operator_name, "pval_oddsratio_2_zscore") == 0) {
    if (indexcolumn == 0) {
      printf("%s\n", "ZSCORE");
    } else {
      printf("%s\t%s\n", "0", "ZSCORE");
    }
  } else if (strcmp(operator_name, "pval_beta_2_zscore") == 0) {
    if (indexcolumn == 0) {
      printf("%s\n", "ZSCORE");
    } else {
      printf("%s\t%s\n", "0", "ZSCORE");
    }
  } else if (strcmp(operator_name, "pval_beta_N_2_zscore") == 0) {
    if (indexcolumn == 0) {
      printf("%s\n", "ZSCORE");
    } else {
      printf("%s\t%s\n", "0", "ZSCORE");
    }
  } else {
    fprintf(stderr, "[ERROR] Cannot make new header, unknown function: %s", operator_name);
  }

  return 0;
}



/*************
Main program
**************/

int main(int argc, char *argv[]) {
  char *buf = NULL;
  size_t buf_len = 0;
  ssize_t bytes_read = 0;

  //int (*operator)(char*);
  int (*operator)(char**, int*);

  char operator_name[35] = "function_placeholder";

  if (argc < 2) {
    fprintf(stderr, "[ERROR] No argument given for the function to use\n");
    return 1;
  }

  // Declare the different functions we can use as operators
  if (strcmp(argv[1], "qnorm") == 0) {
    operator = &operator_qnorm;
    strcpy(operator_name, "qnorm");
  } else if (strcmp(argv[1], "pval_oddsratio_2_zscore") == 0) {
    operator = &operator_pval_oddsratio_2_zscore;
    strcpy(operator_name, "pval_oddsratio_2_zscore");
  } else if (strcmp(argv[1], "pval_beta_2_zscore") == 0) {
    operator = &operator_pval_beta_2_zscore;
    strcpy(operator_name, "pval_beta_2_zscore");
  } else if (strcmp(argv[1], "pval_beta_N_2_zscore") == 0) {
    operator = &operator_pval_beta_N_2_zscore;
    strcpy(operator_name, "pval_beta_N_2_zscore");
  } else {
    fprintf(stderr, "[ERROR] Unknown function: %s", argv[1]);
  }

  // Parse arguments
  int c;
  int skiplines = 0;
  int indexcolumn = 0;
  int pvalue = 0;
  int oddsratio = 0;
  int beta = 0;
  int standarderror = 0;
  int Nindividuals = 0;
  
  while (1)
  {
    int this_option_optind = optind ? optind : 1;
    int option_index = 0;
    static struct option long_options[] =
      {
        {"skiplines",  required_argument, 0, 's'},
        {"index",  required_argument, 0, 'i'},
        {"pvalue",  required_argument, 0, 'p'},
        {"oddsratio",    required_argument, 0, 'o'},
        {"beta",    required_argument, 0, 'b'},
        {"standarderror",    required_argument, 0, 'e'},
        {"Nindividuals",    required_argument, 0, 'n'},
        {0, 0, 0, 0}
      };

    c = getopt_long (argc, argv, "s:i:p:o:0",
                     long_options, &option_index);

    /* Detect the end of the options. */
    if (c == -1)
      break;

    switch (c)
      {
      case 0:
        /* If this option set a flag, do nothing else now. */
        if (long_options[option_index].flag != 0)
          break;
        printf ("option %s", long_options[option_index].name);
        if (optarg)
          printf (" with arg %s", optarg);
        printf ("\n");
        break;

      case 's':
        skiplines = atoi(optarg);
        break;

      case 'i':
        indexcolumn = atoi(optarg); 
        break;

      case 'p':
        pvalue = atoi(optarg) -1; 
        break;

      case 'o':
        oddsratio = atoi(optarg) -1; 
        break;

      case 'b':
        beta = atoi(optarg) -1; 
        break;

      case 'e':
        standarderror = atoi(optarg) -1; 
        break;

      case 'n':
        Nindividuals = atoi(optarg) -1; 
        break;

      case '?':
        /* getopt_long already printed an error message. */
        break;

      default:
        abort ();
      }
  }


  // Check how many arguments are provided and collect values in an array
  int argtot = 0;
  if (pvalue != 0) { argtot++;}
  if (oddsratio != 0) { argtot++;}
  if (beta != 0) { argtot++;}
  if (standarderror != 0) { argtot++;}
  if (Nindividuals != 0) { argtot++;}

  // init argcolvals (pvalue always first element, oddsratio second, etc)
  int argcolvals[5] = {0};
  if (pvalue != 0) { argcolvals[0] = pvalue;}
  if (oddsratio != 0) { argcolvals[1] = oddsratio;}
  if (beta != 0) { argcolvals[2] = oddsratio;}
  if (standarderror != 0) { argcolvals[3] = oddsratio;}
  if (Nindividuals != 0) { argcolvals[4] = Nindividuals;}


  // return value if this function has no errors
  int return_value = 0;

  // Skip header rows for calculation according to value in -h argument, 
  // It should only be one row that needs to be skipped, and important
  // to remember is that in the output there will only be one header row.
  int i = 0;
  for (i = 1; i <= skiplines; ++i) {
    getline(&buf, &buf_len, stdin);
  }
  // Make new header based on header function
  generate_header(operator_name, indexcolumn);

  // Start processing first row
  // Get first line to set correct dimensions
  getline(&buf, &buf_len, stdin);

  // remove newline from buf
  int len;
  len = strlen(buf);
  if( buf[len-1] == '\n' ) {
   buf[len-1] = 0;
  }

  // strtok uses the original buf when iterating forward by replacing the present value with NULL
  // Therefore we have to make a hard copy if we want to loop through the first line twice
  char *buf2 = malloc(buf_len * sizeof(char));
  strcpy(buf2, buf);

  char *token;
  // Calc number of columns specified by \t
  int nrcols = 0;
  // get the first token
  token = strtok(buf, "\t");
  // walk through other tokens
  while( token != NULL ) {
     nrcols++;
     token = strtok(NULL, "\t");
  }
  //printf( "Got %d\n columns", nrcols );

  // redo and store each token split by \t in array
  i = 0;
  int token_len = 0;
  token = strtok(buf2, "\t");
  // sizeof(char*) allocates memory for pointers for char
  // because pointers require a different amount of memory
  // than a normal char
  char **arr = (char**) malloc(nrcols * sizeof(char*));
  while( token != NULL ) {
     token_len = strlen(token);
     arr[i] = (char*) malloc(token_len * sizeof(char) + 1);
     strcpy(arr[i++], token);
     token = strtok(NULL, "\t");
  }

  // use index value if available
  unsigned long int inxval = 0;
  if (indexcolumn != 0) {
    inxval = strtol(arr[indexcolumn-1], NULL, 10);
    printf("%ld\t", inxval );
  }
  
  // use user specified operator for first time (a single time)
  return_value = operator(arr, argcolvals);

  // free buf2
  if (buf2) free(buf2);

  // Loop through remaining rows
  while ((bytes_read = getline(&buf, &buf_len, stdin)) != -1) {

    // remove newline
    int len;
    len = strlen(buf);
    if( buf[len-1] == '\n' ) {
     buf[len-1] = 0;
    }

    i = 0;
    token = strtok(buf, "\t");
    while( token != NULL ) {
       token_len = strlen(token);
       free(arr[i]);
       arr[i] = (char*) malloc(token_len * sizeof(char) + 1);
       strcpy(arr[i++], token);
       token = strtok(NULL, "\t");
    }
 
    // Check if index is assigned, if so, then use it
    if (indexcolumn != 0) {
      inxval = strtol(arr[indexcolumn-1], NULL, 10);
      printf("%ld\t", inxval );
    }
    
    // use user specified operator for the second time(loop over remaining rows)
    return_value = operator(arr, argcolvals);

    if (return_value != 0) {
      break;
    }
  }

  if (errno != 0) {
    perror("[ERROR] Failed to read line from stdin");
    return_value = 1;
  }

  // free buf
  if (buf) free(buf);

  // free 2d array pointer elements
  for ( i = 1; i < nrcols; i++ )
  {
    free(arr[i]);
  }
  // free array
  free(arr);

  return return_value;
}
