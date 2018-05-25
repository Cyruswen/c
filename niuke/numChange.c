#include <stdio.h>

int numChange(char* str){
    int num = 0;
    int i = 0;
    for(;str[i] != '\0'; i++){
        if(str[i] >= 48 && str[i] <= 57){
            num = num * 10 + str[i] - 48;
        }else{
            return 0;
        }
    }
    return num;
}

int main(){
    char str[10] = "1234";
    int ret = numChange(str);
    if(ret == 0){
        printf("转换失败\n");
    }else{
        printf("%d", ret);
    }
    return 0;
}
