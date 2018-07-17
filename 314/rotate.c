#include <stdio.h>
#include <stdlib.h>

#define N 4

void rotate(int arr[N][N], int a, int b, int c, int d)
{
    int i = 0;
    for( ; i < d - b; i++)
    {
       int tmp = arr[a][b + i]; 
       arr[a][b + i] = arr[c - i][b];
       arr[c - i][b] = arr[c][d - i];
       arr[c][d - i] = arr[a + i][d];
       arr[a + i][d] = tmp;
    }
}

void show(int arr[N][N])
{
    int i = 0;
    int j = 0;
    for(; i < N; i++)
    {
        for(j = 0; j < N; j++)
        {
            printf("%-3d", arr[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void rotateArr(int arr[N][N])
{
    int a = 0;
    int b = 0;
    int c = N - 1;
    int d = N - 1;
    while(a < c)
    {
        rotate(arr, a++, b++, c--, d--);
    }
}

int main(){
    int arr[N][N];
    int num = 1;
    int i = 0;
    int j = 0;
    for(; i < N; i++)
    {
        for( j = 0; j < N; j++)
        {
            arr[i][j] = num++;

        }
    }
   show(arr);
   rotateArr(arr);
   //rotate(arr, 0, 0, 3, 3); 
   show(arr);
   return 0;
}
