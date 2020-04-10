#include <stdlib.h>
#include <stdio.h>
#include "Rmath.h"

int main() {

  double prob = 0.0;
  while( scanf("%lf", &prob) != EOF ){
    printf("%lf\n",qnorm(prob, 0.0, 1.0, 1, 0));
  }
  return 0;
}
