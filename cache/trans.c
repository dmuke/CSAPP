/* 
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */ 
#include <stdio.h>
#include "cachelab.h"

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

void trans_32(int M,int N,int A[N][M],int B[M][N]);
void trans_64(int M,int N,int A[N][M],int B[M][N]);

/* 
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded. 
 */
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N])
{	
	if (N == 32 && M ==32) trans_32(M,N,A,B);
	else if (N == 64 && M ==64) trans_64(M,N,A,B);
	else{
	int i,j;
	int bsize = 18;
	for (i = 0;i < N;i += bsize){
		for (j = 0;j < M;j += bsize){
			for (int x = i;x < (i+bsize) && x < N;x++){
				for (int y = j;y < (j+bsize) && y < M;y++){
					B[y][x] = A[x][y];
				}
			}
		}
	}
}}

void trans_32(int M,int N,int A[N][M],int B[M][N])
{
    int i, j;
    int bsize = 8;
    int x1,x2,x3,x4,x5,x6,x7,x8;
    for (i = 0; i < N; (i=i+bsize)) {
        for (j = 0; j < M; (j = j+bsize)) {
            for (int k = i;k <(i + bsize);k++){
            x1 = A[k][j];
            x2 = A[k][j+1];
            x3 = A[k][j+2];
            x4 = A[k][j+3];
            x5 = A[k][j+4];
            x6 = A[k][j+5];
            x7 = A[k][j+6];
            x8 = A[k][j+7];
            B[j][k] = x1;
            B[j+1][k] = x2;
            B[j+2][k] = x3;
            B[j+3][k] = x4;
            B[j+4][k] = x5;
            B[j+5][k] = x6;
            B[j+6][k] = x7;
            B[j+7][k] = x8; 
        }}
    }
}

void trans_64(int M,int N,int A[N][M],int B[M][N])
{
   int i, j;
    int bsize = 8;
    int x1,x2,x3,x4,x5,x6,x7,x8;
    for (i = 0; i < N; (i=i+bsize)) {
        for (j = 0; j < M; (j = j+bsize)) {
            for (int k = i;k <(i + 4);k++){
                x1 = A[k][j];
                x2 = A[k][j+1];
                x3 = A[k][j+2];
                x4 = A[k][j+3];
                x5 = A[k][j+4];
                x6 = A[k][j+5];
                x7 = A[k][j+6];
                x8 = A[k][j+7];
                B[j][k] = x1;
                B[j+1][k] = x2;
                B[j+2][k] = x3;
                B[j+3][k] = x4;
                B[j][k+4] = x5;
                B[j+1][k+4] = x6;
                B[j+2][k+4] = x7;
                B[j+3][k+4] = x8;
            }
            for (int k = j+4;k<(j+bsize);k++){
                x5= A[i+4][k-4];
                x6 = A[i+5][k-4];
                x7 = A[i+6][k-4];
                x8 = A[i+7][k-4];
                x1 = B[k-4][i+4];
                x2 = B[k-4][i+5];
                x3 = B[k-4][i+6];
                x4 = B[k-4][i+7];
                B[k-4][i+4] = x5;
                B[k-4][i+5] = x6;
                B[k-4][i+6] = x7;
                B[k-4][i+7] = x8;
                B[k][i] =x1;
                B[k][i+1] = x2;
                B[k][i+2] = x3;
                B[k][i+3] = x4;
            }
            for (int k = i+4;k<(i+bsize);k++){
                x1 = A[k][j+4];
                x2 = A[k][j+5];
                x3 = A[k][j+6];
                x4 = A[k][j+7];
                B[j+4][k] = x1;
                B[j+5][k] = x2;
                B[j+6][k] = x3;
                B[j+7][k] = x4;
            }
		}}
}

/* 
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started. 
 */ 

/* 
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }    

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc); 

    /* Register any additional transpose functions */
//    registerTransFunction(trans, trans_desc); 

}

/* 
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

