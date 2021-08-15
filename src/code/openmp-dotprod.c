double dotprod(int n, double *x, double *y) {
  double sum = 0;
#pragma omp parallel for reduction(+:sum)
  for (int i = 0; i < n; i++) {
    sum += x[i] * y[i];
  }
  return sum;
}

#include "timing.h"
#include <assert.h>
#include <stdio.h>

int main(int argc, char** argv) {
  assert(argc==2);
  int n = atoi(argv[1]);
  double *x = malloc(n*sizeof(double));
  double *y = malloc(n*sizeof(double));

  for (int i = 0; i < n; i++) {
    x[i] = (double)i/n;
    y[i] = (double)i*2/n;
  }

  double bef = seconds();
  double result = dotprod(n, x, y);
  double aft = seconds();

  free(x);
  free(y);

  printf("Result: %f\n", result);
  printf("Time (s): %f\n", aft-bef);
}
