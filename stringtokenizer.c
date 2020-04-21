#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  

// Build and run tab separated example data using:
/* 
  gcc stringtokenizer.c -o test
  echo -e "0\tpval\tOR\n1\t0.9\t1.2\n2\t0.5\t1.3\n3\t0.1e-10\t1.1" | ./test
*/

// This forum post has interesting ideas on how to do this split and storage
/*
https://stackoverflow.com/questions/40222582/segfault-resulting-from-strdup-and-strtok
*/

int main(int argc, char *argv[]) {
  char *buf = NULL;
  size_t buf_len = 0;
  ssize_t bytes_read = 0;

  // return value if this function has no errors
  int return_value = 0;

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
  printf( "Got %d\n", nrcols );

  // redo and store each token split by \t in array

  //alt 1
  int i = 0;
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

  // Do what you need to do to get col headers right
  printf( "%s\t", arr[0]);
  printf( "%s\t", arr[1]);
  printf( "%s\t%s\n", arr[2] , "funkar");

  // free buf2
  if (buf2) free(buf2);

  // Loop through rows
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

    printf( "%s\t", arr[0]);
    printf( "%s\t", arr[1]);
    printf( "%s\t%s\n", arr[2] , "funkar");

  }

  // free buf
  if (buf) free(buf);

  // free 2d array
  for ( i = 0; i < nrcols; i++ )
  {
    free(arr[i]);
  }
  free(arr);

  return return_value;
}
