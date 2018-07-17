#include"Stm32f10x_led.h"

#include"delay.h"

#include "sys.h"

int main(void){
    delay_init();
    LED_Init();
    LED0 = 0;        
    LED1 = 1;
    delay_ms(300);
    LED0 = 1;
    LED1 = 0;
    delay_ms(300);
}
