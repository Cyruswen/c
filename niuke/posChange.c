#include <stdio.h>

int posChange(char* str, int len, int pos){
    if(pos > len){
        return -1;
    }
    char tmp[len];
    int i = 0;
    int j = pos;
    int k = 0;
    int m = 0;
    for(;str[j] != '\0'; j++){
        tmp[i++] = str[j]; 
    }

    for(;k < pos; k++){
        tmp[i++] = str[k];
    }

    for(; m < len; m++){
        str[m] = tmp[m];
    }

    tmp[len -1] = '\0';
    return 0;
}

int main(){
    int i = 0;
    char str[10] = "abcdefgh";
    posChange(str, 8, 4);
    for(; i < 8; i++){
        printf("%c",str[i]);
    }
    printf("\n");
    return 0;
}
