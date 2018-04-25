#ifndef _TIMER_H_ 
#define _TIMER_H_
extern bit f_systick_1ms; 
extern bit f_systick_2ms; 
extern bit f_systick_4ms; 
extern bit f_systick_10ms;
extern bit f_systick_1s; 
extern bit flag_Dot_blink;
extern bit flag_Tube_blink;
extern bit flag_Alarm_blink;
void Timer0_Init(void); 
void SysTick_intgnd(void);
#endif
