//This script split arguments based on tab separator
//strtok() will consider any sequence of tabs a single delimiter. This can
// be fine for this application as the pipeline can introduce NAs early in
// the workflow


#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  
#include "Rmath.h"

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

//int operator_OR_pval_2_Zscore(char *or, char *pval) {
//  double or2 = strtof(or, NULL);
//  double pval = strtof(or, NULL);
//
//  if (errno != 0) {
//    perror("[ERROR] Failed to parse floating point number");
//    errno = 0;
//
//    return 1;
//  } else if (or2 == 0.0 && strstr(or, "0.0") == NULL) {
//    fprintf(stderr, "[ERROR] Given line was not a floating point number: %s", buf);
//
//    return 1;
//  }
//
//  printf("%lf\n", qnorm(prob, 0.0, 1.0, 1, 0));
//
//  return 0;
//}


int main(int argc, char *argv[]) {
  char *buf = NULL;
  size_t buf_len = 0;
  ssize_t bytes_read = 0;

  //int (*operator)(char*);
  int (*operator)(char**, int*);

  char operator_name[25] = "function_placeholder";

  if (argc < 2) {
    fprintf(stderr, "[ERROR] No argument given for the function to use\n");
    return 1;
  }

  if (strcmp(argv[1], "qnorm") == 0) {
    operator = &operator_qnorm;
    strcpy(operator_name, "qnorm");
  } else {
    fprintf(stderr, "[ERROR] Unknown function: %s", argv[1]);
  }

  // Parse arguments
  int opt; 
  int skiplines = 0;
  int whichisindexcolumn = 0;
  int argcol1 = 0;
  int argcol2 = 0;
    // put ':' in the starting of the 
    // string so that program can  
    //distinguish between '?' and ':'  
    while((opt = getopt(argc, argv, ":h:i:1:2:")) != -1) 
    {  
        switch(opt)  
        {  
            case 'h':
                // think about replacing stoi to something more modern
                skiplines = atoi(optarg);
                break;
            case 'i':
                // think about replacing atoi to something more modern
                whichisindexcolumn = atoi(optarg)-1; 
                break;  
            case '1':
                // think about replacing atoi to something more modern
                argcol1 = atoi(optarg) -1; 
                break;  
            case '2':
                // think about replacing atoi to something more modern
                argcol2 = atoi(optarg) -1; 
                break;
            case ':':  
                printf("option needs a value\n");  
                break;  
            case '?':  
                printf("unknown option: %c\n", optopt); 
                break;  
        }  
    }  

  // Check how many arguments are provided and collect values in an array
  int argtot = 0;
  
  if (argcol1 != 0) { argtot++;}
  if (argcol2 != 0) { argtot++;}

  int argcolvals[2] = {0};
  if (argcol1 != 0) { argcolvals[0] = argcol1;}
  if (argcol2 != 0) { argcolvals[1] = argcol2;}


  // return value if this function has no errors
  int return_value = 0;

  // Skip header rows for calculation according to value in -h argument, 
  // It should only be one row that needs to be skipped, and important
  // to remember is that in the output there will only be one header row.
  int i = 0;
  for (i = 1; i <= skiplines; ++i) {
    getline(&buf, &buf_len, stdin);
  }

  // Make new header based on function
  if (strcmp(operator_name, "qnorm") == 0) {
    if (whichisindexcolumn == 0) {
      printf("%s\n", "QNORM");
    } else {
      printf("%s\t%s\n", "0", "QNORM");
    }
  } else {
    fprintf(stderr, "[ERROR] Cannot make new header, unknown function: %s", argv[1]);
  }

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
  for ( i = 0; i < nrcols; i++ )
  {
    free(arr[i]);
  }
  // free array
  free(arr);

  return return_value;
}
