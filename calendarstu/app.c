#include "timer.h" 
#include "app.h" 
#include<reg51.h>
sbit buzzer = P0^4;
uchar data	event_FIFO[8];	//�¼�������
uchar data	*event_WrIp = event_FIFO;	//дָ��
uchar data	*event_RdIp = event_FIFO;
uchar data	display[4];	//��ָ��
uchar data	sys_State = 0x14;	//����״̬����ʼ״̬��ʾʱ�֣�
uchar data	current_Event = 0;	//����״̬
uchar data	month_Day[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
//ÿ��������������飨������������ݳ�ֵ���� 2015�������꣬�����³�ֵΪ 28 �졣��
uchar data	second	= 50;	//���ֵ
char	data	minute	= 59;	//�ֳ�ֵ
char	data	hour = 23;	//ʱ��ֵ
uchar data	day	= 28;	//�³�ֵ
uchar data	month = 2;	//�ճ�ֵ
uint	data	year = 2016;	//���ֵ
uint	data	week = 7;	//�ܳ�ֵ
char	data	alarm1_Minute = 0;	//���� 1 ʱ��ֵ
char	data	alarm1_Hour	= 0;	//���� 1 �ֳ�ֵ
char	data	alarm2_Minute = 0;	//���� 2 ʱ��ֵ
char	data	alarm2_Hour	= 0;	//���� 2 �ֳ�ֵ
bit	swatch_Start_Flag = 0;	//����ʱ��ʼ����λ
bit	alarm1_Flag = 0;	
bit	alarm2_Flag = 0;	
uchar alarm1_State = 0;	//0-free 1-louding 2-handstop 3-stop
uchar alarm2_State = 0;
//������: void write_Event(uchar cmd)
//��������: ���¼�������д��ֵ
void	write_Event(uchar cmd)
{
	*event_WrIp=cmd; if(event_WrIp==(event_FIFO+7))	event_WrIp=event_FIFO; else event_WrIp++;
}
//������: void write_Event(uchar cmd)
//��������: ���¼�����������ֵ
uchar read_Event(void)
{ 
	uchar temp; 
	if(event_RdIp!=event_WrIp)
	{ 
		temp=*event_RdIp; 
		if(event_RdIp==(event_FIFO+7))	
			event_RdIp=event_FIFO;
		else event_RdIp++;
		return	temp;
	} else	
		return(0);
}
//������: void clock_update(void)
//��������: ʱ���Զ����º���
void clock_update(void)
{ 
	if( !f_systick_1s ) 
		return; 
		f_systick_1s = 0;
	if(++second != 60) 
		return; 
		second = 0; 
		if(++minute != 60) 
		return; 
		minute = 0;
	if(++hour != 24) 
		return; 
		hour = 0; 
		if(++week == 8) 
		week = 1; 
		if(++day <= month_Day[month - 1]) 
		return; 
		day = 1; 
		if(++month != 13) 
		return; 
		month = 1;
		year++; 
		month_Day[1] = ( (year%400 == 0) || ( (year%4 == 0)&&(year%100 != 0) ) ) ? 29 : 28;
//������Ŀ�����ж�����
}
//������: meau1_Display()
//��������: �˵� P-1 ����
void meau1_Display(void)
{ 
	switch(current_Event)
	{ 
		case 0:break;
		case 1:sys_State = 0x11;break;	//��ʾ��
		case 2:sys_State = 0x20;break;	//��ʾ�˵� P-2
		case 4:sys_State = 0x30;break;	//��ʾ�˵� P-3
		case 8:break; default:break;
	}
	//��ʾ�˵� P-1
	display[0] = 10; 
	display[1] = 17; 
	display[2] = 21; 
	display[3] = 1;
}
//������: meau2_Display(void)
//��������: �˵� P-2 ����
void meau2_Display(void)
{ 
	switch(current_Event)
	{ 
		case 0:break;
		case 1:sys_State = 0x21;break;	//���
		case 2:sys_State = 0x30;break;	//��ʾ�˵� P-3
		case 4:sys_State = 0x10;break;	//��ʾ�˵� P-1
		case 8:break; 
		default:break;
	}
//��ʾ�˵� P-2
	display[0] = 10; 
	display[1] = 17; 
	display[2] = 21; 
	display[3] = 2;
}
//������: meau3_Display()
//��������: �˵� P-3 ����
void meau3_Display(void)
{ 
	switch(current_Event)
	{ 
		case 0:break;
		case 1:sys_State = 0x31;break;	//����
		case 2:sys_State = 0x10;break;	//��ʾ�˵� P-1
		case 4:sys_State = 0x20;break;	//��ʾ�˵� P-2
		case 8:break; 
		default:break;
	}
//��ʾ�˵� P-3
	display[0] = 10; display[1] = 17; display[2] = 21; display[3] = 3;
}
///������::alarm_meau1()
//��������: ��ʾ���� 1 B-1 ����
void alarm_meau1(void)
{ 
	switch(current_Event)
	{
		case 0:break;
		case 1:sys_State = 0x32;break;	//��ʾ���� 1 ON/OFF
		case 2:sys_State = 0x35;break;	//��ʾ���� 2 B-2
		case 4:sys_State = 0x35;break;	//��ʾ���� 2 B-2
		case 8:sys_State = 0x30;break; 
		default:break;
	}
//��ʾ���� 1 B-1
	display[0] = 10; display[1] = 12; display[2] = 21; display[3] = 1;
}
//������::alarm_meau2()
//��������: ��ʾ���� 2 B-2 ����
void alarm_meau2(void)
{ 
	switch(current_Event)
	{ 
		case 0:break;	//��ʾ�˵� P-3
		case 1:sys_State = 0x36;break;	//��ʾ���� 2 ON/OFF
		case 2:sys_State = 0x31;break;	//��ʾ���� 1 B-1
		case 4:sys_State = 0x31;break;	//��ʾ���� 1 B-1
		case 8:sys_State = 0x30;break;	//��ʾ�˵� P-3
		default:break;
	}
//��ʾ���� 2 B-2
	display[0] = 10; 
	display[1] = 12; 
	display[2] = 21; 
	display[3] = 2;
}
//������: year_Set()
//��������: �����ú���
void year_Set(void)
{ 
	switch(current_Event)
	{ 
		case 0:break;
		case 1:sys_State = 0x11;break; 
		case 2:year--;break; 
		case 4:year++;break;	//��ʾ��
		case 8:sys_State = 0x10;break;	//��ʾ�˵� P-1
		default:break;
	}
	month_Day[1] = ( (year%400 == 0) || ( (year%4 == 0)&&(year%100 != 0) ) ) ? 29 : 28; 
	if(day > month_Day[month - 1]) 
		day = 1;
	tube(0x00);	//�������˸
	display[0] = year / 1000;
	display[1] = year / 100 % 10; 
	display[2] = year / 10 % 10; 
	display[3] = year % 10;
}
//������: week_Set()
//��������: �����ú���
void week_Set(void)
{ 
	switch(current_Event)
	{ 
		case 0:break;
		case 1:sys_State = 0x13;break;	//��ʾ��
		case 2:week--;if(week == 0) week = 7;break;	//��--
		case 4:week++;if(week == 8) week = 1;break;	//��++
		case 8:sys_State = 0x10;break;	//��ʾ�˵� P-1
		default:break;
	}
	tube(0x07); display[0] = 20; display[1] = 12; display[2] = 21; display[3] = week;
}
//������: month_Set(void)
//��������: �����ú���
void	month_Set(void)
{ 
switch(current_Event)
	{ 
		case 0:break;
		case 1:sys_State = 0x18;break;	//������
		case 2:month--; if(month == 0)month = 12; break;	//��--
		case 4:month++; if(month == 13)month = 1; break;	//��++
		case 8:sys_State = 0x10;break;	//��ʾ�˵� P-1
		default:break;
	} 
	if(day > month_Day[month - 1]) 
		day = 1;
	tube(0x0c); 
	decimal_Point_On(0x02); 
	display[0] = month / 10; 
	display[1] = month % 10; 
	display[2] = day / 10; 
	display[3] = day % 10;
}
	//������: void	day_Set(void)
//��������: �����ú���
void day_Set(void)
{ 
	switch(current_Event)
	{
		case 0:break;
		case 1:sys_State = 0x12;break;	//��ʾ����
		case 2:
		{ 
			day--; 
			if(day == 0) 
				day = month_Day[month - 1];
//ʹ��ÿ���µ�����������Ӧ�·�ʵ���������
			break; 
		} 
		case 4:
		{ 
			day++; 
			if(day > month_Day[month - 1])
//���䳬��ÿ���������ʱ��������һ
			{ 
				day = 1; 
			} 
			break; 
		} 
		case 8:sys_State = 0x10;break;	//��ʾ�˵� P-1
		default:break; 
		} 
		tube(0x03); 
		decimal_Point_On(0x02); 
		display[0] = month / 10; 
		display[1] = month % 10;
		display[2] = day / 10; 
		display[3] = day % 10;;
}
	//������: void	hour_Set(void)
//��������: Сʱ���ú���
void	hour_Set(void)
{ 
	switch(current_Event)
	{ 
		case 0:break;
		case 1:sys_State = 0x1b;break;	//���÷���
		case 2:hour--;if(hour < 0) hour = 23;break;	//Сʱ--
		case 4:hour++;if(hour == 24) hour = 0;break;	//Сʱ++
		case 8:sys_State = 0x10;break;	//��ʾ�˵� P-1
		default:break; 
	} 
	tube(0x0c); 
	decimal_Point(0x02); 
	display[0] = hour / 10; 
	display[1] = hour % 10; 
	display[2] = minute / 10; 
	display[3] = minute % 10;
}
void	minute_Set(void)
{ 
	switch(current_Event)
	{ 
		case 0:break;
		case 1:sys_State = 0x14;break;	//��ʾʱ��
		case 2:minute--;if(minute < 0) minute = 59;break;	//����--
		case 4:minute++;if(minute == 60) minute = 0;break;	//����++
		case 8:sys_State = 0x10;break;	//��ʾ�˵� P-1
		default:break;
	} 
	tube(0x03); 
	decimal_Point(0x02);
	display[0] = hour / 10; 
	display[1] = hour % 10; 
	display[2] = minute / 10; 
	display[3] = minute % 10;
}
	//������: void	alarm1_Hour_Set(void)
//��������: ���� 1--Сʱ����
void	alarm1_Hour_Set(void)
{ 
	switch(current_Event) 
	{ 
		case 0:break;
		case 1:sys_State = 0x34;break;	//�������� 1 ����
		case 2:alarm1_Hour--;if(alarm1_Hour < 0) alarm1_Hour = 23;break;	//���� 1 Сʱ--
		case 4:alarm1_Hour++;if(alarm1_Hour == 24) alarm1_Hour = 0;break;	//���� 1 Сʱ++
		case 8:sys_State = 0x31;break; default:break;
	} 
	tube(0x0c); 
	decimal_Point(0x02); 
	display[0] = alarm1_Hour / 10; 
	display[1] = alarm1_Hour % 10; 
	display[2] = alarm1_Minute / 10; 
	display[3] = alarm1_Minute % 10;
}
//������: void	alarm1_Minute_Set(void)
//��������: ���� 1--��������
void	alarm1_Minute_Set(void)
{ 
	switch(current_Event)
	{
		case 0:break;	//��ʾ���� 1 B-1
		case 1:sys_State = 0x33;break;	//�������� 1 Сʱ
		case 2:alarm1_Minute--;if(alarm1_Minute < 0) alarm1_Minute = 59;break;	//���� 1 ����-case 4:alarm1_Minute++;if(alarm1_Minute == 60) alarm1_Minute = 0;break;	//���� 1 ����++
		case 8:sys_State = 0x31;break; default:break;
	} 
	tube(0x03); 
	decimal_Point(0x02); 
	display[0] = alarm1_Hour / 10; 
	display[1] = alarm1_Hour % 10; 
	display[2] = alarm1_Minute / 10; 
	display[3] = alarm1_Minute % 10;
}	//��ʾ���� 1 B-1	
//������: void	alarm2_Hour_Set(void)
//��������: ���� 2--Сʱ����
void	alarm2_Hour_Set(void)
{ 
	switch(current_Event)
	{ 
		case 0:break;	
		case 1:sys_State = 0x38;break;	//�������� 2 ����
		case 2:alarm2_Hour--;if(alarm2_Hour < 0) alarm2_Hour = 23;break;	//���� 2 Сʱ--
		case 4:alarm2_Hour++;if(alarm2_Hour == 24) alarm2_Hour = 0;break;	//���� 2 Сʱ++
		case 8:sys_State = 0x35;break;	//��ʾ���� 2 B-2
		default:break;
	} 
	tube(0x0c); 
	decimal_Point(0x02); 
	display[0] = alarm2_Hour / 10; 
	display[1] = alarm2_Hour % 10; 
	display[2] = alarm2_Minute / 10; 
	display[3] = alarm2_Minute % 10;
}
	//������: void	alarm2_Minute_Set(void)
//��������: ���� 2--��������
void	alarm2_Minute_Set(void)
{ 
	switch(current_Event)
	{ 
		case 0:break;
		case 1:sys_State = 0x37;break;	//�������� 2 Сʱ
		case 2:alarm2_Minute--;if(alarm2_Minute < 0) alarm2_Minute = 59;break;	//���� 1 ����-case 4:alarm2_Minute++;if(alarm2_Minute == 60) alarm2_Minute = 0;break; //���� 1 ����++ case 8:sys_State = 0x35;break;	//��ʾ���� 2 B-2
		default:break;
	} 
	tube(0x03); 
	decimal_Point(0x02);
	display[0] = alarm2_Hour / 10; 
	display[1] = alarm2_Hour % 10; 
	display[2] = alarm2_Minute / 10; 
	display[3] = alarm2_Minute % 10;
}
//������: year_Display()
//��������: ����ʾ����
void year_Display(void)
{ 
	switch(current_Event)
	{ 
		case 0:break;
		case 1:sys_State = 0x16;break;	//������ case 2:sys_State = 0x12;break;	//��ʾ���� case 4:sys_State = 0x15;break;	//��ʾ�� case 8:sys_State = 0x10;break;	//��ʾ�˵� P-1
		default:break; 
	} display[0] = year / 1000; 
	display[1] = year / 100 % 10; 
	display[2] = year / 10 % 10; 
	display[3] = year % 10;
}
	//������: void	month_Day_Display(void)
//��������: ��-����ʾ����
void	month_Day_Display(void)
{ 
	switch(current_Event)
	{
		case 0:break;
		case 1:sys_State = 0x17;break;	//������
		case 2:sys_State = 0x13;break;	//��ʾ��
		case 4:sys_State = 0x11;break;	//��ʾ��
		case 8:sys_State = 0x10;break; default:break;
	} 
	display[0] = month / 10; 
	display[1] = month % 10; 
	display[2] = day / 10; 
	display[3] = day % 10;
	decimal_Point_On(0x02);
}
//������: void	week_Display(void)
//��������: ������ʾ����
void	week_Display(void)
{ 
	switch(current_Event)
	{ 
		case 0:break;	//��ʾ�˵� P-1
		case 1:sys_State = 0x19;break;	//������
		case 2:sys_State = 0x14;break;	//��ʾʱ��
		case 4:sys_State = 0x12;break;	//��ʾ����
		case 8:sys_State = 0x10;break;	//��ʾ�˵� P-1
		default:break;
	} 
	display[0] = 20; 
	display[1] = 12; 
	display[2] = 21; 
	display[3] = week;
}
	//������: void	hour_Minute_Display(void)
//��������: ʱ-����ʾ����
void hour_Minute_Display(void)
{ 
	switch(current_Event)
	{ 
		case 0:break;
		case 1:sys_State = 0x1a;break;	//����ʱ
		case 2:sys_State = 0x15;break;	//��ʾ��
		case 4:sys_State = 0x13;break;	//��ʾ��
		case 8:sys_State = 0x10;break; default:break;
	} 
	display[0] = hour / 10; 
	display[1] = hour % 10; 
	display[2] = minute / 10; 
	display[3] = minute % 10;	//��ʾ�˵� P-1
	decimal_Point(0x02);	//����С����
}
	//������: void	second_Display(void)
//��������: ����ʾ����
void second_Display(void)
{ 
	switch(current_Event)
	{ 
		case 0:break; case 1:break;
		case 2:sys_State = 0x11;break;	//��ʾ��
		case 4:sys_State = 0x14;break;	//��ʾʱ��
		case 8:sys_State = 0x10;break;	//��ʾ�˵� P-1
		default:break; 
	} 
	display[0] = 10; 
	display[1] = 10; 
	display[2] = second / 10; 
	display[3] = second % 10;
}
	//������: void	swatch(void)
//��������: �����
void	swatch(void)
{ 
	static	uint data count; 
	if(swatch_Start_Flag ==1)
	{
		if(f_systick_10ms == 1)	//��������
		{ 
			f_systick_10ms = 0; 
				count++; 
			if(count==9999) 
				count = 0;
		} 
	} 
	switch(current_Event)
	{ 
		case 0:break;
		case 1:break;	//�����ͣ
		case 2:count = 0; swatch_Start_Flag = 0; break;	//�������
		case 4:swatch_Start_Flag = !swatch_Start_Flag; break;	//��������ʼ
		case 8:sys_State = 0x20;break;	//��ʾ�˵� P-2
		default:break;
	} 
	display[0] = count/1000; 
	display[1] = count/100%10; 
	display[2] = count/10%10; 
	display[3] = count%10;
}
	//������: void	alarm1_Clock(void)
//��������: ���� 1 ����
void alarm1_Clock(void)
{
	switch(current_Event)
	{ 
		case 0:break;
		case 1:	//����
		{ 
			if(alarm1_Flag==1) 
			{
				sys_State = 0x33;
			}
			else
			{
				sys_State = 0x31;
			} 
			break;
		}
	case 2:	alarm1_Flag = 1;break;	//��ʾ ON�������ӡ�
	case 4:alarm1_Flag = 0;break; //��ʾ OFF,�ر����ӡ� 
	case 8:sys_State = 0x31;break; //��ʾ���� 1 B-1
	default:break;
	} 
	if(alarm1_Flag==1)
	{ 
		display[0] = 10; 
		display[1] = 10; 
		display[2] = 0; 
		display[3] = 31;
	}
	else
	{ 
		display[0] = 10; 
		display[1] = 0; 
		display[2] = 16; 
		display[3] = 16;
	}
}
	//������: void	alarm2_Clock(void)
//��������: ���� 2 ����
void alarm2_Clock(void)
{ 
	switch(current_Event)
	{
		case 0:break;
		case 1:	//����
		{ 
			if(alarm2_Flag==1) 
			{
				sys_State = 0x37;
			}
			else
			{
				sys_State = 0x35;
			} break;
		}
	case 2:	alarm2_Flag = 1;break;	//��ʾ ON�������ӡ�
	case 4:alarm2_Flag = 0;break;	//��ʾ OFF,�ر����ӡ�
	case 8:sys_State = 0x35;break;	//��ʾ���� 2 B-2
	default:break; } 
	if(alarm2_Flag==1)
	{ 
		display[0] = 10; 
		display[1] = 10; 
		display[2] = 0; 
		display[3] = 31; 
	}
	else
	{ 
		display[0] = 10; 
		display[1] = 0; 
		display[2] = 16; 
		display[3] = 16;
	}
}
//������: void	alarm_Clock_Loud(void) //��������:
void alarm_Clock_Loud(void)
{ 
	if(alarm1_Flag==1) 
	{
		if(alarm1_Hour == hour && alarm1_Minute == minute )	//���Ӷ�ʱʱ�䵽
		{ 
			if(alarm1_State == 0)
			{ 
				alarm1_State = 1;
			}
		}
		else
		{
			if(alarm1_State == 1)	//��������ʱ����û���ֶ�ֹͣ������һ���Ӻ������Զ�ֹͣ
			{ 
				buzzer = 1;
			}
			alarm1_State = 0;
		}	//���Ӷ�ʱʱ��δ����ʼ����λ����
		if(alarm1_State == 1)	//���ӿ�ʼ��
		{ 
			if(flag_Alarm_blink==1)
			{ 
				buzzer= 1;
			}
			else
			{
				buzzer= 0;
			}
			switch(current_Event)	//�ֶ�����--�����ֹͣ����
			{ 
				case 0:break; 
				case 1:alarm1_State = 2;break; 
				case 2:alarm1_State = 2;break; 
				case 4:alarm1_State = 2;break; 
				case 8:alarm1_State = 2;break;
			} 
		} else if(alarm1_State == 2)
		{
			buzzer = 1;	//�رշ�����
			alarm1_State = 3;	//handstop to stop
		} 
	} 
	if(alarm1_Flag==1||alarm2_Flag==1)
	{ 
		if(alarm2_Hour == hour && alarm2_Minute == minute)	//���Ӷ�ʱʱ�䵽
		{ 
			if(alarm2_State == 0)
			{ 
				alarm2_State = 1;
			}
		}
		else
		{ 
			if(alarm2_State == 1)
//��������ʱ����û���ֶ�ֹͣ������һ���Ӻ������Զ�ֹͣ
			{ 
				buzzer = 1; 
			} 
			alarm2_State = 0;
//���Ӷ�ʱʱ��δ����ʼ����λ����
		}
		if(alarm2_State == 1)
//���ӿ�ʼ��
		{ 
			if(flag_Alarm_blink==1)
			{
				 buzzer= 1; 
			}
			else
			{ 
				buzzer= 0; 
			} 
			switch(current_Event)
//�ֶ�����--�����ֹͣ����
			{
				case 0:break; 
				case 1:alarm2_State = 2;break; 
				case 2:alarm2_State = 2;break;
				case 4:alarm2_State = 2;break; 
				case 8:alarm2_State = 2;break;
			} 
		} else if(alarm2_State == 2)
		{
			buzzer = 1;	//�رշ�����
			alarm2_State = 3;	//handstop to stop
		}
	}
}
//������: Mn_app(void) 
void Mn_app(void) 
{ 
	dotBuf = 0x00; 
	tubeBuf = 0x0f; 
	led_Display(); 
	clock_update(); 
	alarm_Clock_Loud();
	current_Event=read_Event(); 
	switch(sys_State)
	{
		case 0x00:break;
//�˵���ĸ״̬
		case 0x10:meau1_Display();break;		//��ʾ�˵� P-1
		case 0x20:meau2_Display();break;		//��ʾ�˵� P-2
		case 0x30:meau3_Display();break;
//ʱ����״̬		//��ʾ�˵� P-3
		case 0x11:year_Display();break;		//��ʾ��
		case 0x12:month_Day_Display();break;		//��ʾ����
		case 0x13:week_Display();break;		//��ʾ��
		case 0x14:hour_Minute_Display();break;		//��ʾʱ��
		case 0x15:second_Display();break;		//��ʾ��
		case 0x16:year_Set();break;		//������
		case 0x17:month_Set();break;		//������
		case 0x18:day_Set();break;		//������
		case 0x19:week_Set();break;		//������
		case 0x1a:hour_Set();break;		//����ʱ
		case 0x1b:minute_Set();break;
//�����״̬
		case 0x21:swatch();break;	//���
//������״̬		//���÷�
		case 0x31:alarm_meau1();break;	//��ʾ���� 1 B-1	
		case 0x32:alarm1_Clock();break;	//��ʾ���� 1 on/off	
		case 0x33:alarm1_Hour_Set();break;	//�������� 1 ʱ	
		case 0x34:alarm1_Minute_Set();break;	//�������� 1 ��	
		case 0x35:alarm_meau2();break;	//��ʾ���� 2 B-1
		case 0x36:alarm2_Clock();break;	//��ʾ���� 2 on/off
		case 0x37:alarm2_Hour_Set();break;	//�������� 2 ʱ
		case 0x38:alarm2_Minute_Set();break;	//�������� 2 ��
		default:break;
	}
}
