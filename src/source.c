//This script split arguments based on tab separator
//strtok() will consider any sequence of tabs a single delimiter. This can
// be fine for this application as the pipeline can introduce NAs early in
// the workflow

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "Rmath.h"
#include "stat_operators.h"


/*************
Main program
**************/

int main(int argc, char *argv[]) {
  char *buf = NULL;
  size_t buf_len = 0;
  ssize_t bytes_read = 0;
  int len;

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
  } else if (strcmp(argv[1], "beta_se_2_zscore") == 0) {
    operator = &operator_beta_se_2_zscore;
    strcpy(operator_name, "beta_se_2_zscore");
  } else if (strcmp(argv[1], "zscore_N_2_pvalue") == 0) {
    operator = &operator_zscore_N_2_pvalue;
    strcpy(operator_name, "zscore_N_2_pvalue");
  } else if (strcmp(argv[1], "zscore_2_pvalue") == 0) {
    operator = &operator_zscore_2_pvalue;
    strcpy(operator_name, "zscore_2_pvalue");
  } else if (strcmp(argv[1], "zscore_se_2_beta") == 0) {
    operator = &operator_zscore_se_2_beta;
    strcpy(operator_name, "zscore_se_2_beta");
  } else if (strcmp(argv[1], "zscore_N_af_2_beta") == 0) {
    operator = &operator_zscore_N_af_2_beta;
    strcpy(operator_name, "zscore_N_af_2_beta");
  } else if (strcmp(argv[1], "zscore_beta_2_se") == 0) {
    operator = &operator_zscore_beta_2_se;
    strcpy(operator_name, "zscore_beta_2_se");
  } else if (strcmp(argv[1], "zscore_N_af_2_se") == 0) {
    operator = &operator_zscore_N_af_2_se;
    strcpy(operator_name, "zscore_N_af_2_se");
  } else if (strcmp(argv[1], "zscore_beta_af_2_N") == 0) {
    operator = &operator_zscore_beta_af_2_N;
    strcpy(operator_name, "zscore_beta_af_2_N");
  } else {
    fprintf(stderr, "[ERROR] Unknown function: %s", argv[1]);
  }

  // Define list
  typedef struct node {
    char strvar[256];
    struct node * next;
  } node_t;


  // Parse file of functions
  char const* const fileName = "functiontestfile.txt"; 
  FILE* file = fopen(fileName, "r"); /* should check the result */
  char line[256];

  // Initiate list structure and access first row of file
  node_t * head = NULL;
  head = (node_t *) malloc(sizeof(node_t));
  fgets(line, sizeof(line), file);
  // remove newline
  len = strlen(line);
  if( line[len-1] == '\n' ) {
   line[len-1] = 0;
  }
  strcpy(head->strvar,line);
  node_t * current = head;

  while (fgets(line, sizeof(line), file)) {
    // remove newline
    len = strlen(line);
    if( line[len-1] == '\n' ) {
     line[len-1] = 0;
    }

    head->next = (node_t *) malloc(sizeof(node_t));
    strcpy(head->next->strvar,line);
    head->next->next = NULL;
  }
  /* may check feof here to make a difference between eof and io failure -- network
     timeout for instance */
  fclose(file);

  //check what we got in our list
  while (current != NULL) {
      printf("%s\n", current->strvar);
      current = current->next;
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
  int zscore = 0;
  int allelefreq = 0;
  
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
        {"zscore",    required_argument, 0, 'z'},
        {"allelefreq",    required_argument, 0, 'a'},
        {0, 0, 0, 0}
      };

    c = getopt_long (argc, argv, "s:i:p:o:b:e:n:z:a:0",
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

      case 'z':
        zscore = atoi(optarg) -1; 
        break;

      case 'a':
        allelefreq = atoi(optarg) -1; 
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
  if (zscore != 0) { argtot++;}
  if (allelefreq != 0) { argtot++;}

  // init argcolvals (pvalue always first element, oddsratio second, etc)
  int argcolvals[7] = {0};
  if (pvalue != 0) { argcolvals[0] = pvalue;}
  if (oddsratio != 0) { argcolvals[1] = oddsratio;}
  if (beta != 0) { argcolvals[2] = oddsratio;}
  if (standarderror != 0) { argcolvals[3] = oddsratio;}
  if (Nindividuals != 0) { argcolvals[4] = Nindividuals;}
  if (zscore != 0) { argcolvals[5] = zscore;}
  if (allelefreq != 0) { argcolvals[6] = allelefreq;}

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
