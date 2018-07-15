#include "led.h"
#include "delay.h"

int main(void){
    while(1){
        (*((u32*)0x42210180)) = 0;

        Delay(0xFFFFF);

        (*((u32*)0x42210180)) = 1;

        Delay(0xFFFFF);

        (*((u32*)0x42210184)) = 0;

        Delay(0xFFFFF);

        (*((u32*)0x42210184)) = 1;

        Delay(0xFFFFF);


    }
}
