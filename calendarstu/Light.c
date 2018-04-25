//============================================================
//文件名称:	流水灯函数
//功能说明: 
//硬件描述:
//资源说明: 11.0592M晶体
//创建日期: 2015/10/21
//软件版本: v 1.0     
//============================================================


//=============================头文件=========================
#include <reg51.h>
#include <intrins.h>
#include "light.h"
#include "delay.h"
#include "timer.h"
//=============================宏定义=========================
#define LedPort  P1
#define uchar unsigned char
//=============================变量===========================
uchar ScanCode=0xfe;
uchar LED_DisBuf=0xfe;
bit Flag=0;	

//============================================================
//函数名称：Mn_Led_Player(void)
//功能描述: 实现发光二极管的循环左移和右移
//输    入：
//输    出：
//返 回 值：
//说    明：
//============================================================
void Mn_Light_Player(void)
{
	if(Flag==0)
	{
		ScanCode=_crol_(ScanCode,1);		  //循环左移实现
		LedPort = ScanCode;
		
	}
  else
	{
		ScanCode=_cror_(ScanCode,1);		  //循环右移实现
		LedPort = ScanCode;
	}	
	Delay_Ms(50);
}
//============================================================
//函数名称：Mn_Led_Player(void)
//功能描述: 实现发光二极管的状态取反
//输    入：
//输    出：
//返 回 值：
//说    明：
//============================================================
void Mn_Light_toggle(void)
{
	if(f_systick_1s)
	{
		f_systick_1s=0;
		LedPort = ~LedPort;		 //取反 
	}
}
//============================================================
//函数名称：Mn_Led_Player(void)
//功能描述: 实现发光二极管的多种花样变化
//输    入：
//输    出：
//返 回 值：
//说    明：
//============================================================
void Mn_Led_Pat_Change(void)
{  
	static uchar i=0;
	static uchar LedState=0;
  if(f_systick_100ms==1)
  {
   	 f_systick_100ms=0;
	   switch(LedState)
	   {
			case 0:					                                           //单个从上到下点亮
			{	
				LedPort=LED_DisBuf;
				LED_DisBuf=_crol_(LED_DisBuf,1);                         //循环左移
				if(LED_DisBuf==0x7f)
				{
					LedState=1;	
				}
			  break;
			}
			case 1:						                                         //单个从下到上点亮
			{
				LedPort=LED_DisBuf;
				LED_DisBuf=_cror_(LED_DisBuf,1);                         //循环右移
				if(LED_DisBuf==0xfe)
				{
				 	LedState=2;
				}
				break;	
			}
			case 2:							                                      //顺序从上到下依次点亮
			{	
				LedPort=LED_DisBuf;
				LED_DisBuf<<=1;
				if(LED_DisBuf==0)
				{
					LedState=3;	
				}		
				break;
			}
			case 3:						                                        //顺序从上到下依次熄灭
			{
				LedPort=LED_DisBuf;
				LED_DisBuf<<=1;
		    LED_DisBuf++;
				if(LED_DisBuf==0xff)
				{
					LedState=4;	
				}
				break;
			}
			case 4:					                                         //顺序从下到上依次点亮
			{	
				LedPort=LED_DisBuf;
				LED_DisBuf>>=1;
				if(LED_DisBuf==0)
				{
					LedState=5;	
				}
				break;
			}
			case 5:					                                        //顺序从下到上依次熄灭
			{	
				LedPort=LED_DisBuf;
				LED_DisBuf>>=1;
				LED_DisBuf+=0x80;
				if(LED_DisBuf==0xff)
				{
					LedState=6;
					LED_DisBuf=0xaa;	
				}
				break;
			}
			case 6:						                                      //顺序从上到下隔一点亮
			{	
				i++;
				LedPort=LED_DisBuf;
				if(LED_DisBuf==0xaa) LED_DisBuf=0x55;
				else  LED_DisBuf=0xaa;
				if(i==7)
				{
					i=0;
					LedState=7;	
				}
				break;
			}
			case 7:
			{	
				LedPort=LED_DisBuf;
				LED_DisBuf=0xfe;
				LedState=0;
				break;
			}
			default: break;
		}
   }
}