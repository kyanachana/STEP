#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
//c言語
double get_time()
{
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec + tv.tv_usec * 1e-6;
}

int main(int argc, char** argv)
{

  if (argc != 2) {
    printf("usage: %s N\n", argv[0]);
    return -1;
  }
  

  int n = atoi(argv[1]);
  double* a = (double*)malloc(n * n * sizeof(double)); // Matrix A
  double* b = (double*)malloc(n * n * sizeof(double)); // Matrix B
  double* c = (double*)malloc(n * n * sizeof(double)); // Matrix C

  // Initialize the matrices to some values.
  int i, j;
  for (i = 0; i < n; i++) {
    for (j = 0; j < n; j++) {
      a[i * n + j] = i * n + j; // A[i][j]
      b[i * n + j] = j * n + i; // B[i][j]
      c[i * n + j] = 0; // C[i][j]
    }
  }

  double begin = get_time();

  /**************************************/
  /* Write code to calculate C = A * B. */
  //行列の積のプログラム
  // //pattern1 i,j,k
  // //最内ループでアクセスが連続でない行列の数：1
  //   for(int i=0;i<n;i++){
  //       for(int j=0;j<n;j++){
  //           for(int k=0;k<n;k++){
  //               c[i * n+j]+=a[i * n+k] * b[k * n+j];
  //           }
            
  //       }
  //   }
  // //pattern2 i,k,j
  // //アクセスが連続でない行列の数：０
  //   for(int i=0;i<n;i++){
  //       for(int k=0;k<n;k++){
  //           for(int j=0;j<n;j++){
  //               c[i * n+j]+=a[i * n+k] * b[k * n+j];
  //           }
            
  //       }
  //   }
  // //pattern3 j,i,k
  // //最内ループでアクセスが連続でない行列の数：1
  //   for(int j=0;j<n;j++){
  //       for(int i=0;i<n;i++){
  //           for(int k=0;k<n;k++){
  //               c[i * n+j]+=a[i * n+k] * b[k * n+j];
  //           }
            
  //       }
  //   }
  // //pattern4 j,k,i
  // //最内ループでアクセスが連続でない行列の数：１
  // //真ん中のループでアクセスが連続でない行列の数：１
  //   for(int j=0;j<n;j++){
  //       for(int k=0;k<n;k++){
  //           for(int i=0;i<n;i++){
  //               c[i * n+j]+=a[i * n+k] * b[k * n+j];
  //           }
            
  //       }
  //   }
  // //pattern5 k,i,j
  // //真ん中のループでアクセスが連続でない行列の数：1
  //   for(int k=0;k<n;k++){
  //       for(int i=0;i<n;i++){
  //           for(int j=0;j<n;j++){
  //               c[i * n+j]+=a[i * n+k] * b[k * n+j];
  //           }
            
  //       }
  //   }
  
  //pattern6 k,j,i
  //最内のループでアクセスが連続でない行列の数：1
    for(int k=0;k<n;k++){
        for(int j=0;j<n;j++){
            for(int i=0;i<n;i++){
                c[i * n+j]+=a[i * n+k] * b[k * n+j];
            }
            
        }
    }
  //予想
  //*アクセスが連続しない行列の数が少ない方が速い
  //*最内のループの方が真ん中のループよりもアクセス数が多いので、アクセスが連続でないと遅くなる影響が大きい
  //→ 2<5<1=3=6<4
  //結果
  //1:34.187586 sec
  //2: 3.360891 sec
  //3:24.013735 sec
  //4:40.258584 sec
  //5: 3.614800 sec
  //6:37.819395 sec
  //真ん中のループで連続でないことによる影響が小さい

  
    /**************************************/
  double end = get_time();
  printf("time: %.6lf sec\n", end - begin);

  // Print C for debugging. Comment out the print before measuring the execution time.
  double sum = 0;
  for (i = 0; i < n; i++) {
    for (j = 0; j < n; j++) {
      sum += c[i * n + j];
      //printf("c[%d][%d]=%lf\n", i, j, c[i * n + j]);
    }
  }
  // Print out the sum of all values in C.
  // This should be 450 for N=3, 3680 for N=4, and 18250 for N=5.
  //printf("sum: %.6lf\n", sum);

  free(a);
  free(b);
  free(c);
  return 0;
}
