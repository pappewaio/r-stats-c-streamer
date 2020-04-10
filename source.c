#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

  int return_value = 0;

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
