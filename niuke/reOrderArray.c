#include <stdio.h>
#include <string.h>

void reOrderArray(int* array) {
    int size = sizeof(array);
    int tmp1[size];
    int tmp2[size];
    int m = 0;
    int n = 0;
    int i = 0;
    for( i = 0; i < size; ++i){
        if(array[i] % 2 == 0){
            tmp1[m++] = array[i];
        }else{
            tmp2[n++] = array[i];
        }
    }
    int j = 0;
    for(; j < n; j++){
        tmp1[m + j] = tmp2[j];
    }
    int k = 0;
    for(;k < size; k++){
        array[k] = tmp1[k];
    }
}
        
int main(){
    int array[] = {1,2,3,4,5,6,7,8};
    reOrderArray(array);
    int i = 0;
    for(; i < 8; i++){
        printf("%d ", array[i]);
    }
    return 0;
}

