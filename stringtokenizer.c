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

int main(int argc, char *argv[]) {
  char *buf = NULL;
  size_t buf_len = 0;
  ssize_t bytes_read = 0;

  // return value if this function has no errors
  int return_value = 0;

  // Loop through rows
  while ((bytes_read = getline(&buf, &buf_len, stdin)) != -1) {

    // remove newline
    int len;
    len = strlen(buf);
    if( buf[len-1] == '\n' ) {
     buf[len-1] = 0;
    }

    printf( "%s\t%s\n", buf , "funkar");
  }
    //create array (here old code not working, which we need to replace)
   // char *array[10] = {NULL};
   // int i=0;
   // array[i] = strtok(buf,"\t");
   // while(array[i]!=NULL) {
   //   array[++i] = strtok(NULL,"\t");
   // }


  if (errno != 0) {
    perror("[ERROR] Failed to read line from stdin");
    return_value = 1;
  }

  if (buf) free(buf);

  return return_value;
}
