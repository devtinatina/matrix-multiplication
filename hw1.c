#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#define N 2048

int main(int argc, char *argv[])
{
  int i;
  int j;
  int k;
  int fd; // File descriptor
  float **A;
  float **B;
  float **C;
  float sum;
  clock_t start;
  clock_t end;

  /* Error Message & Usage */

  if (argc < 3)
  {
    fprintf(stderr, "Usage: %s <file1> <file2>\n", argv[0]);
    exit(1);
  }

  /* Dynamic allocation */
  A = (float **)malloc(sizeof(float *) * 2048);
  for (i = 0; i < 2048; i++)
  {
    A[i] = (float *)malloc(sizeof(float) * 2048);
  }

  fd = open(argv[1], O_RDONLY);
  if (fd == -1)
  {
    printf("File Open Error\n");
  }

  /* read file from argv[1] */

  start = clock();
  for (i = 0; i < 2048; i++)
  {
    for (j = 0; j < 2048; j++)
    {
      lseek(fd, sizeof(float) * (i * N + j), SEEK_SET);
      read(fd, &(A[i][j]), sizeof(float));
    }
  }
  end = clock();
  printf("time for read %s : %lf\n", argv[1], (double)(end - start) / CLOCKS_PER_SEC);

  close(fd);

  /* Dynamic allocation */

  B = (float **)malloc(sizeof(float *) * 2048);
  for (i = 0; i < 2048; i++)
  {
    B[i] = (float *)malloc(sizeof(float) * 2048);
  }

  fd = open(argv[2], O_RDONLY);
  if (fd == -1)
  {
    printf("File Open Error\n");
  }

  /* read file from argv[2] */

  start = clock();
  for (i = 0; i < 2048; i++)
  {
    for (j = 0; j < 2048; j++)
    {
      lseek(fd, sizeof(float) * (i * N + j), SEEK_SET);
      read(fd, &(B[i][j]), sizeof(float));
    }
  }
  end = clock();
  printf("time for read %s : %lf\n", argv[2], (double)(end - start) / CLOCKS_PER_SEC);
  close(fd);

  /* Dynamic allocation */

  C = (float **)malloc(sizeof(float *) * 2048);
  for (i = 0; i < 2048; i++)
  {
    C[i] = (float *)malloc(sizeof(float) * N);
  }

  /* calculate matrix multiplication */

  start = clock();
  for (i = 0; i < 2048; i++)
  {
    for (j = 0; j < 2048; j++)
    {
      sum = 0;
      for (k = 0; k < 2048; k++)
      {
        sum += A[i][k] * B[k][j];
      }
      C[i][j] = sum;
    }
  }
  end = clock();
  printf("time for calculate matrix multiplication: %lf\n", (double)(end - start) / CLOCKS_PER_SEC);

  /* Free Dynamic allocation */

  for (i = 0; i < 2048; i++)
  {
    free(A[i]);
  }
  free(A);

  for (i = 0; i < 2048; i++)
  {
    free(B[i]);
  }
  free(B);

  /* save result to new file */

  fd = open("c1.dat", O_WRONLY | O_CREAT, 00644);
  if (fd == -1)
  {
    printf("File Open Error\n");
  }

  start = clock();
  for (i = 0; i < 2048; i++)
  {
    for (j = 0; j < 2048; j++)
    {
      write(fd, &(C[i][j]), sizeof(float));
    }
  }
  end = clock();
  printf("time for write result : %lf\n", (double)(end - start) / CLOCKS_PER_SEC);

  /* Free Dynamic allocation */

  for (i = 0; i < 2048; i++)
  {
    free(C[i]);
  }
  free(C);

  close(fd);
}
