#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX 100

int main(int argc, char* argv[]) {
  float coeffs[MAX][MAX];
  float rhs[MAX];
  float error[MAX];
  float res1[MAX];
  float res2[MAX];
  int i, j;
  float precision = 0.000001;
  FILE* fp = NULL;

  if (argc == 3) {
    char* filename = argv[1];
    fp = fopen(filename, "r");
    precision = atof(argv[2]);
  } else if (argc == 2) {
    char* filename = argv[1];
    fp = fopen(filename, "r");
    printf("no precision mentioned, using internal precision: %f\n", precision);
  } else {
    printf("usage: gauss-seidel <eqn_filename> <precision>\n\n");
    return -1;
  }
  if (fp != NULL) {
    /* read from file */
    /* no of eq */
    int n_eq;
    fscanf(fp, "N %d", &n_eq);

    /* coefficients and rhs */
    for (i = 0; i < n_eq; i++) {
      for (j = 0; j < n_eq + 1; j++) {
        if (j < n_eq) {
          fscanf(fp, "%f", &coeffs[i][j]);
        } else {
          fscanf(fp, "%f", &rhs[i]);
        }
      }
    }

    /* done taking loading data. close file. */
    fclose(fp);

    /* printing the coeffs */
    printf("\nCoeffs:\n");
    for (i = 0; i < n_eq; i++) {
      for (j = 0; j < n_eq; j++) {
        printf("%.2f ", coeffs[i][j]);
      }
      printf("\n");
    }

    /* printing the rhs */
    printf("\nRHS:\n");
    for (i = 0; i < n_eq; i++) {
      printf("%f\n", rhs[i]);
    }

    /* fill res1 & res2 with initial value */
    for (i = 0; i < n_eq; i++) {
      res1[i] = 0;
      res2[i] = 0;
    }

    /* get 1st set of data in res2 */
    for (i = 0; i < n_eq; i++) {
      float sum = 0;
      for (j = 0; j < n_eq; j++) {
        if (j != i) {
          sum += res2[j] * coeffs[i][j];
        }
      }
      res2[i] = (rhs[i] - sum) / coeffs[i][i];
      error[i] = fabs(res2[i] - res1[i]);
    }

    /* start main loop */
    int flag = 1;
    int count = 1; /* 1 iteration outside loop */
    while (flag) {
      /* reset flag = 0 at every iteration */
      flag = 0;

      for (i = 0; i < n_eq; i++) {
        float sum = 0;
        for (j = 0; j < n_eq; j++) {
          if (j != i) {
            sum += res2[j] * coeffs[i][j];
          }
        }
        res1[i] = res2[i];
        res2[i] = (rhs[i] - sum) / coeffs[i][i];
        error[i] = fabs(res2[i] - res1[i]);
      }

      /* evaluate flag from error                  */
      /* if any error[i] > precision then flag = 1 */
      for (i = 0; i < n_eq; i++) {
        if (error[i] > precision) {
          flag = 1;
          break;
        }
      }
      count++;
    }

    /* printing result */
    printf("\nResult:\n");
    for (i = 0; i < n_eq; i++) {
      printf("x%d = %f\n", i, res2[i]);
    }

    printf("\nIterations: %d\n", count);
  } else {
    printf("couldn't open file '%s', make sure the file exists\n", argv[1]);
  }
  return 0;
}