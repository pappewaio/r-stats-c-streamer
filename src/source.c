//This script split arguments based on tab separator
//strtok() will consider any sequence of tabs a single delimiter. This can
// be fine for this application as the pipeline can introduce NAs early in
// the workflow

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <math.h>

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

  // Define list to store functions from file
  typedef struct node {
    char strvar[256];
    struct node * next;
  } node_t;


  // Parse arguments
  int c;
  int skiplines = 0;
  int indexcolumn = 0;
  int pvalue = 0;
  int neglog10p = 0;
  int oddsratio = 0;
  int beta = 0;
  int standarderror = 0;
  int Nindividuals = 0;
  int zscore = 0;
  int allelefreq = 0;
  int allelefreqswitch = 0;
  char statmodel[256];
  char functionfile[256];
  int replace = 0;
  //char *functionfile = "functiontestfile.txt";
  
  while (1)
  {
    int this_option_optind = optind ? optind : 1;
    int option_index = 0;
    static struct option long_options[] =
      {
        {"version",  no_argument, 0, 'v'},
        {"skiplines",  required_argument, 0, 's'},
        {"index",  required_argument, 0, 'i'},
        {"pvalue",  required_argument, 0, 'p'},
        {"neglog10p",    no_argument, 0, 'l'},
        {"oddsratio",    required_argument, 0, 'o'},
        {"beta",    required_argument, 0, 'b'},
        {"standarderror",    required_argument, 0, 'e'},
        {"Nindividuals",    required_argument, 0, 'n'},
        {"zscore",    required_argument, 0, 'z'},
        {"allelefreq",    required_argument, 0, 'a'},
        {"allelefreqswitch",    no_argument, 0, 'w'},
        {"functionfile",    required_argument, 0, 'f'},
        {"statmodel",    required_argument, 0, 'm'},
        {"replace",    required_argument, 0, 'r'},
        {0, 0, 0, 0}
      };

    c = getopt_long (argc, argv, "vlws:i:p:o:b:e:n:z:a:f:m:0",
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

      case 'v':
	printf("r-stats-c-stremer 1.1.1\n");
        return(0);

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

      case 'w':
        allelefreqswitch = 1;
        break;

      case 'l':
        neglog10p = 1;
        break;

      case 'f':
        strcpy(functionfile, optarg); 
        break;

      case 'm':
        strcpy(statmodel, optarg); 
        break;

      case 'r':
        replace = atoi(optarg) -1; 
        break;

      case '?':
        /* getopt_long already printed an error message. */
        break;

      default:
        abort ();
      }
  }
  
  // check that at least one function param has been given
  if (functionfile[0] == '\0' ) {
    fprintf(stderr, "[ERROR] No argument given for the function file to use. Supply a file with one function for each row\n");
    return 1;
  }
  
  // check that a stats model has been selected
  if (statmodel[0] == '\0' ) {
    fprintf(stderr, "[ERROR] No argument given for the stats method to use. Use one of lin (linear), log (logistic), linMM (linear mixed model) or logMM (logistic mixed model) \n");
    return 1;
  }
  
  // check that one of the allowed stats models has been selected
  if (strcmp(statmodel, "lin") != 0 && strcmp(statmodel, "log") != 0 && strcmp(statmodel, "none") != 0) {
    fprintf(stderr, "[ERROR] Wrong argument given for the stats method to use. Use one of lin (linear), log (logistic) or none if the calculation is independent of statmodel \n");
    return 1;
  }

  // Parse file of functions if available
  FILE* file = fopen(functionfile, "r"); /* should check the result */
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

  // Continue to fill the list with remaining rows
  int funlen = 1;
  while (fgets(line, sizeof(line), file)) {
    funlen++;
    // remove newline
    len = strlen(line);
    if( line[len-1] == '\n' ) {
     line[len-1] = 0;
    }

    current->next = (node_t *) malloc(sizeof(node_t));
    strcpy(current->next->strvar,line);
    current->next->next = NULL;
    current = current->next;
  }
  /* may check feof here to make a difference between eof and io failure -- network
     timeout for instance */
  fclose(file);

  //store in arrfun from our list
  int (*arrfun[funlen])(char**, int*, int*);

  int inx = 0;
  current = head;
  while (current != NULL) {
      //printf("%s\n", current->strvar);
      populate_array(arrfun, current->strvar, inx, statmodel);
      current = current->next;
      inx++;
  }

  // Check how many arguments for calculations are provided and collect values in an array
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
  if (beta != 0) { argcolvals[2] = beta;}
  if (standarderror != 0) { argcolvals[3] = standarderror;}
  if (Nindividuals != 0) { argcolvals[4] = Nindividuals;}
  if (zscore != 0) { argcolvals[5] = zscore;}
  if (allelefreq != 0) { argcolvals[6] = allelefreq;}
  
  // Add one more that will be 0 or 1
  int valmodifier[2] = {0};
  valmodifier[0] = allelefreqswitch;
  valmodifier[1] = neglog10p;

  // return value if this function has no errors
  int return_value = 0;

  // Skip header rows for calculation according to value in -h argument, 
  // It should only be one row that needs to be skipped, and important
  // to remember is that in the output there will only be one header row.

  int i = 0;
  if (replace != 0) {
    //Use original header
    getline(&buf, &buf_len, stdin);
    //newline is already in buff, so there will be an automatic newline
    printf("%s", buf);

  }else {
    for (i = 1; i <= skiplines; ++i) {
      getline(&buf, &buf_len, stdin);
    }

    // Make new header 
    // print index
    if (indexcolumn != 0) {
      printf("%s", "0");
    }

    // print remaining colnames
    current = head;
    if (indexcolumn == 0) {
      printf("%s", current->strvar);
      current = current->next;
    }
    while (current != NULL) {
      printf("\t%s", current->strvar);
      current = current->next;
    }
    printf("\n");

    //free list
    current = head;
    while (current != NULL) {
      head = current->next;
      free(current);
      current = head;
    }
  }

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
  
  int j = 0;
  int k = 0;
  if (replace != 0) {
    // init colindex counter
    for (k = 0; k < nrcols; k++){
      if (k == 0) {
        // if first column is the one to modify (usually this is an index, so maybe not)
        if (k == replace) {
          (*arrfun[j]) (arr, argcolvals, valmodifier);
        }else {
          printf("%s", arr[k] );
        }
      }else{
        printf("\t");
        // Check if replace index match present colindex
        if (k == replace) {
          (*arrfun[j]) (arr, argcolvals, valmodifier);
        }else {
          printf("%s", arr[k] );
        }
      }
    }
    printf("\n");
  }else {
    // use index value if available
    if (indexcolumn != 0) {
      printf("%s", arr[indexcolumn-1] );
    }
    for (j = 0; j < funlen; j++){
      if (j == 0 && indexcolumn == 0) {
        (*arrfun[j]) (arr, argcolvals, valmodifier);
      }else{
        printf("\t");
        (*arrfun[j]) (arr, argcolvals, valmodifier);
      }
    }
    printf("\n");
  }

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
       //printf("&HERE=%s=HERE&\t", arr[i-1]);
    }
 
  
    if (replace != 0) {
      // init colindex counter
      for (k = 0; k < nrcols; k++){
        if (k == 0) {
          // if first column is the one to modify (usually this is an index, so maybe not)
          if (k == replace) {
            (*arrfun[j]) (arr, argcolvals, valmodifier);
          }else {
            printf("%s", arr[k] );
          }
        }else{
          printf("\t");
          // Check if replace index match present colindex
          if (k == replace) {
            (*arrfun[j]) (arr, argcolvals, valmodifier);
          }else {
            printf("%s", arr[k] );
          }
        }
      }
      printf("\n");
    }else {
      // use index value if available
      if (indexcolumn != 0) {
        printf("%s", arr[indexcolumn-1] );
      }
      for (j = 0; j < funlen; j++){
        if (j == 0 && indexcolumn == 0) {
          (*arrfun[j]) (arr, argcolvals, valmodifier);
        }else{
          printf("\t");
          (*arrfun[j]) (arr, argcolvals, valmodifier);
        }
      }
      printf("\n");
    }

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
  for ( i = 0; i < nrcols; i++ )
  {
    free(arr[i]);
  }
  // free array
  free(arr);

  return return_value;
}
