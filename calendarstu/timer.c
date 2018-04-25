//�ļ�����: ��ʱ������
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
bit flag_Alarm_blink; //������:	Timer0_Init()
//��������: ��ʱ�� T0 �ĳ�ʼ������������ 1ms ��ʱ��
void Timer0_Init(void)
{	
	TMOD=0x01;	//T0 ��ʱ��������ʽ 1
	TH0=0xfc;
	TL0=0x18;	//1ms ��ֵ
	ET0=1;	//����ʱ�� T0 �ж�
	EA=1;	//�����ж�
	TR0=1;
}	//������ʱ�� T0
//������: ISR_T0(void) interrupt
//��������: T0 ��ʱ���жϴ�����	1
void ISR_T0(void) interrupt	1
{
	TH0=0xfc;	//1ms �ж�
	TL0=0x18;	//��װ 1ms �ж�
	f_systick_1ms=1;	//1ms ʱ����־λ
}
//������: SysTick_intgnd(void)
//��������: ʱ��������
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
			cnt_systick_4ms=0;	//����ɨ���־λ
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
				cnt_systick_1s=0;	//�����ɨ���־λ
				f_systick_1s=1;	//ʱ�� 1s ����һ�α�־λ
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
				f_systick_505ms=1;	//���ӱ�����־λ
				flag_Dot_blink=~flag_Dot_blink;	//С������˸��־λ
				flag_Tube_blink=~flag_Tube_blink;	//�������˸��־λ
			}
		}
	}
}
