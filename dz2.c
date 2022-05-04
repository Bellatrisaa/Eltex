#include <stdio.h>
#include <malloc.h>
#define n 5

void ShowMatrix(int *M[], int N)
{
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++)
            printf("%d ", M[i][j]);
    printf("\n");    
    }
}

void task1(int N)
{    
    for(int i = 1; i <= N; i++){
        for(int j = 1; j <= N; j++)
            printf("%d ", j + (i - 1) * N);
     printf("\n"); 
    }  
}

void task2(int Arr[], int N)
{
for(int i = N - 1; i >= 0; i--)
    printf("%d ", Arr[i]);
}

void task3(int N)
{
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++)
            if(i + j < N - 1) printf("0");
            else printf("1");
     printf("\n");
    }
}

void task4(int *M[], int N)
{
    int i = 0, j = 0, k = 0;
    for(int s = 1; s <= N * N; s++){
       M[i][j] = s;
       if (s % ((N - 1 - k) * 4) == 0) k++;
       if((j < N - 1 - k) && (i == k)) j++;
       else if((j > k) && (i == N - 1 - k)) j--;
       else if((i < N - 1) && (j == N - 1 - k)) i++;
       else if((i > k) && (j == k)) i--;
       else break;
    }
    ShowMatrix(M, N);
}

int main()
{
    int Arr[n]{1,2,3,4,5};
    int *Matrix[n];

    for(int i = 0; i < n; i++)
    Matrix[i] = (int*)malloc(n * sizeof(int));    

    printf("\n\nTask 1\n\n");
    task1(n);
    printf("\n\nTask 2\n\n");
    task2(Arr, n);
    printf("\n\nTask 3\n\n");
    task3(n);
    printf("\n\nTask 4\n\n");
    task4(Matrix, n);

    for(int i = 0; i < n; i++)
    free(Matrix[i]);
    return 0;
}
