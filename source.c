#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  
#include "Rmath.h"

int operator_qnorm(char *buf) {
  double prob = strtod(buf, NULL);

  if (errno != 0) {
    perror("[ERROR] Failed to parse floating point number");
    errno = 0;

    return 1;
  } else if (prob == 0.0 && strstr(buf, "0.0") == NULL) {
    fprintf(stderr, "[ERROR] Given line was not a floating point number: %s", buf);

    return 1;
  }

  printf("%lf\n", qnorm(prob, 0.0, 1.0, 1, 0));

  return 0;
}

int main(int argc, char *argv[]) {
  char *buf = NULL;
  size_t buf_len = 0;
  ssize_t bytes_read = 0;

  int (*operator)(char*);

  if (argc < 2) {
    fprintf(stderr, "[ERROR] No argument given for the function to use\n");
    return 1;
  }

  if (strcmp(argv[1], "qnorm") == 0) {
    operator = &operator_qnorm;
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
                whichisindexcolumn = atoi(optarg); 
                break;  
            case '1':
                // think about replacing atoi to something more modern
                argcol1 = atoi(optarg); 
                break;  
            case '2':
                // think about replacing atoi to something more modern
                argcol2 = atoi(optarg); 
                break;
            case ':':  
                printf("option needs a value\n");  
                break;  
            case '?':  
                printf("unknown option: %c\n", optopt); 
                break;  
        }  
    }  

  int return_value = 0;

  // Skip header rows for calculation according to value in -h argument, 
  // but keep in output
  int i;
  for (i = 1; i <= skiplines; ++i) {
    getline(&buf, &buf_len, stdin);
    printf("%s", buf); 
    
  }
  // Loop through remaining rows
  while ((bytes_read = getline(&buf, &buf_len, stdin)) != -1) {
    return_value = operator(buf);

    if (return_value != 0) {
      break;
    }
  }

  if (errno != 0) {
    perror("[ERROR] Failed to read line from stdin");
    return_value = 1;
  }

  if (buf) free(buf);

  return return_value;
}
