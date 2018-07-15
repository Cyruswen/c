#include "led.h"

void LED_Init(void){
    SystemInit();
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHZ;

    GPIO_Init(GPIOA, &GPIO_InitStructure);
}
