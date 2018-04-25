//============================================================
//�ļ�����:	��ˮ�ƺ���
//����˵��: 
//Ӳ������:
//��Դ˵��: 11.0592M����
//��������: 2015/10/21
//����汾: v 1.0     
//============================================================


//=============================ͷ�ļ�=========================
#include <reg51.h>
#include <intrins.h>
#include "light.h"
#include "delay.h"
#include "timer.h"
//=============================�궨��=========================
#define LedPort  P1
#define uchar unsigned char
//=============================����===========================
uchar ScanCode=0xfe;
uchar LED_DisBuf=0xfe;
bit Flag=0;	

//============================================================
//�������ƣ�Mn_Led_Player(void)
//��������: ʵ�ַ�������ܵ�ѭ�����ƺ�����
//��    �룺
//��    ����
//�� �� ֵ��
//˵    ����
//============================================================
void Mn_Light_Player(void)
{
	if(Flag==0)
	{
		ScanCode=_crol_(ScanCode,1);		  //ѭ������ʵ��
		LedPort = ScanCode;
		
	}
  else
	{
		ScanCode=_cror_(ScanCode,1);		  //ѭ������ʵ��
		LedPort = ScanCode;
	}	
	Delay_Ms(50);
}
//============================================================
//�������ƣ�Mn_Led_Player(void)
//��������: ʵ�ַ�������ܵ�״̬ȡ��
//��    �룺
//��    ����
//�� �� ֵ��
//˵    ����
//============================================================
void Mn_Light_toggle(void)
{
	if(f_systick_1s)
	{
		f_systick_1s=0;
		LedPort = ~LedPort;		 //ȡ�� 
	}
}
//============================================================
//�������ƣ�Mn_Led_Player(void)
//��������: ʵ�ַ�������ܵĶ��ֻ����仯
//��    �룺
//��    ����
//�� �� ֵ��
//˵    ����
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
			case 0:					                                           //�������ϵ��µ���
			{	
				LedPort=LED_DisBuf;
				LED_DisBuf=_crol_(LED_DisBuf,1);                         //ѭ������
				if(LED_DisBuf==0x7f)
				{
					LedState=1;	
				}
			  break;
			}
			case 1:						                                         //�������µ��ϵ���
			{
				LedPort=LED_DisBuf;
				LED_DisBuf=_cror_(LED_DisBuf,1);                         //ѭ������
				if(LED_DisBuf==0xfe)
				{
				 	LedState=2;
				}
				break;	
			}
			case 2:							                                      //˳����ϵ������ε���
			{	
				LedPort=LED_DisBuf;
				LED_DisBuf<<=1;
				if(LED_DisBuf==0)
				{
					LedState=3;	
				}		
				break;
			}
			case 3:						                                        //˳����ϵ�������Ϩ��
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
			case 4:					                                         //˳����µ������ε���
			{	
				LedPort=LED_DisBuf;
				LED_DisBuf>>=1;
				if(LED_DisBuf==0)
				{
					LedState=5;	
				}
				break;
			}
			case 5:					                                        //˳����µ�������Ϩ��
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
			case 6:						                                      //˳����ϵ��¸�һ����
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