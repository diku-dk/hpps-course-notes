#include "timing.h"
#include <stdio.h>

int fib(int n) {
  if (n <= 1) {
    return 1;
  } else {
    return fib(n-1) + fib(n-2);
  }
}

int main() {
  double bef, aft;

  int n = 45;
  int *fibs = malloc(n * sizeof(int));

  bef = seconds();
#pragma omp parallel for schedule(static)
  for (int i = 0; i < n; i++) {
    fibs[i] = fib(i);
  }
  aft = seconds();
  printf("Static scheduling:  %fs\n", aft-bef);

  bef = seconds();
#pragma omp parallel for schedule(dynamic)
  for (int i = 0; i < n; i++) {
    fibs[i] = fib(i);
  }
  aft = seconds();
  printf("Dynamic scheduling: %fs\n", aft-bef);

  free(fibs);
}
