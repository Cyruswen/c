//文件名称: 定时器函数
#include <reg51.h>
#include "timer.h"
#include "ledscan.h" 
#include "key.h" 
bit f_systick_1ms=0; 
bit f_systick_2ms=0; 
uchar cnt_systick_2ms=0; 
bit f_systick_4ms=0; 
uchar cnt_systick_4ms=0; 
bit f_systick_10ms=0; 
uchar cnt_systick_10ms=0; 
uchar cnt_systick_101ms=0; 
bit f_systick_505ms=0; 
uchar cnt_systick_505ms=0; 
bit f_systick_1s=0; 
uchar cnt_systick_1s=0; 
bit flag_Dot_blink; 
bit flag_Tube_blink; 
bit flag_Alarm_blink; //函数名:	Timer0_Init()
//功能描述: 定时器 T0 的初始化函数，产生 1ms 的时基
void Timer0_Init(void)
{	
	TMOD=0x01;	//T0 定时器工作方式 1
	TH0=0xfc;
	TL0=0x18;	//1ms 初值
	ET0=1;	//开定时器 T0 中断
	EA=1;	//开总中断
	TR0=1;
}	//启动定时器 T0
//函数名: ISR_T0(void) interrupt
//功能描述: T0 定时器中断处理函数	1
void ISR_T0(void) interrupt	1
{
	TH0=0xfc;	//1ms 中断
	TL0=0x18;	//重装 1ms 中断
	f_systick_1ms=1;	//1ms 时基标志位
}
//函数名: SysTick_intgnd(void)
//功能描述: 时基处理函数
void SysTick_intgnd(void)
{ 
	if(f_systick_1ms==1)
	{
		f_systick_1ms=0; 
		cnt_systick_2ms++; 
		if(cnt_systick_2ms==2)
		{ 
			cnt_systick_2ms=0;
			f_systick_2ms=1;
		} 
		cnt_systick_4ms++; 
		if(cnt_systick_4ms==4)
		{ 
			cnt_systick_4ms=0;	//按键扫描标志位
			f_systick_4ms=1;
		} 
		cnt_systick_10ms++; 
		if(cnt_systick_10ms==10)
		{ 
			cnt_systick_10ms=0; 
			f_systick_10ms=1; 
			cnt_systick_1s++; 
			if(cnt_systick_1s==100)
			{ 
				cnt_systick_1s=0;	//数码管扫描标志位
				f_systick_1s=1;	//时钟 1s 更新一次标志位
			}
		}
		cnt_systick_101ms++; 
		if(cnt_systick_101ms==101)
		{ 
			cnt_systick_101ms=0;
			flag_Alarm_blink = ~ flag_Alarm_blink; 
			cnt_systick_505ms++; 
			if(cnt_systick_505ms==5)
			{ 
				cnt_systick_505ms=0; 
				f_systick_505ms=1;	//闹钟报警标志位
				flag_Dot_blink=~flag_Dot_blink;	//小数点闪烁标志位
				flag_Tube_blink=~flag_Tube_blink;	//数码管闪烁标志位
			}
		}
	}
}
