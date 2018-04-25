#include "timer.h" 
#include "app.h" 
#include<reg51.h>
sbit buzzer = P0^4;
uchar data	event_FIFO[8];	//事件缓冲区
uchar data	*event_WrIp = event_FIFO;	//写指针
uchar data	*event_RdIp = event_FIFO;
uchar data	display[4];	//读指针
uchar data	sys_State = 0x14;	//界面状态（初始状态显示时分）
uchar data	current_Event = 0;	//按键状态
uchar data	month_Day[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
//每个月最大天数数组（二月是由于年份初值给的 2015，非闰年，即二月初值为 28 天。）
uchar data	second	= 50;	//秒初值
char	data	minute	= 59;	//分初值
char	data	hour = 23;	//时初值
uchar data	day	= 28;	//月初值
uchar data	month = 2;	//日初值
uint	data	year = 2016;	//年初值
uint	data	week = 7;	//周初值
char	data	alarm1_Minute = 0;	//闹钟 1 时初值
char	data	alarm1_Hour	= 0;	//闹钟 1 分初值
char	data	alarm2_Minute = 0;	//闹钟 2 时初值
char	data	alarm2_Hour	= 0;	//闹钟 2 分初值
bit	swatch_Start_Flag = 0;	//秒表计时开始标致位
bit	alarm1_Flag = 0;	
bit	alarm2_Flag = 0;	
uchar alarm1_State = 0;	//0-free 1-louding 2-handstop 3-stop
uchar alarm2_State = 0;
//函数名: void write_Event(uchar cmd)
//功能描述: 向事件缓冲区写入值
void	write_Event(uchar cmd)
{
	*event_WrIp=cmd; if(event_WrIp==(event_FIFO+7))	event_WrIp=event_FIFO; else event_WrIp++;
}
//函数名: void write_Event(uchar cmd)
//功能描述: 从事件缓冲区读入值
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
//函数名: void clock_update(void)
//功能描述: 时间自动更新函数
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
//利用三目运算判断闰年
}
//函数名: meau1_Display()
//功能描述: 菜单 P-1 函数
void meau1_Display(void)
{ 
	switch(current_Event)
	{ 
		case 0:break;
		case 1:sys_State = 0x11;break;	//显示年
		case 2:sys_State = 0x20;break;	//显示菜单 P-2
		case 4:sys_State = 0x30;break;	//显示菜单 P-3
		case 8:break; default:break;
	}
	//显示菜单 P-1
	display[0] = 10; 
	display[1] = 17; 
	display[2] = 21; 
	display[3] = 1;
}
//函数名: meau2_Display(void)
//功能描述: 菜单 P-2 函数
void meau2_Display(void)
{ 
	switch(current_Event)
	{ 
		case 0:break;
		case 1:sys_State = 0x21;break;	//秒表
		case 2:sys_State = 0x30;break;	//显示菜单 P-3
		case 4:sys_State = 0x10;break;	//显示菜单 P-1
		case 8:break; 
		default:break;
	}
//显示菜单 P-2
	display[0] = 10; 
	display[1] = 17; 
	display[2] = 21; 
	display[3] = 2;
}
//函数名: meau3_Display()
//功能描述: 菜单 P-3 函数
void meau3_Display(void)
{ 
	switch(current_Event)
	{ 
		case 0:break;
		case 1:sys_State = 0x31;break;	//闹钟
		case 2:sys_State = 0x10;break;	//显示菜单 P-1
		case 4:sys_State = 0x20;break;	//显示菜单 P-2
		case 8:break; 
		default:break;
	}
//显示菜单 P-3
	display[0] = 10; display[1] = 17; display[2] = 21; display[3] = 3;
}
///函数名::alarm_meau1()
//功能描述: 显示闹钟 1 B-1 函数
void alarm_meau1(void)
{ 
	switch(current_Event)
	{
		case 0:break;
		case 1:sys_State = 0x32;break;	//显示闹钟 1 ON/OFF
		case 2:sys_State = 0x35;break;	//显示闹钟 2 B-2
		case 4:sys_State = 0x35;break;	//显示闹钟 2 B-2
		case 8:sys_State = 0x30;break; 
		default:break;
	}
//显示闹钟 1 B-1
	display[0] = 10; display[1] = 12; display[2] = 21; display[3] = 1;
}
//函数名::alarm_meau2()
//功能描述: 显示闹钟 2 B-2 函数
void alarm_meau2(void)
{ 
	switch(current_Event)
	{ 
		case 0:break;	//显示菜单 P-3
		case 1:sys_State = 0x36;break;	//显示闹钟 2 ON/OFF
		case 2:sys_State = 0x31;break;	//显示闹钟 1 B-1
		case 4:sys_State = 0x31;break;	//显示闹钟 1 B-1
		case 8:sys_State = 0x30;break;	//显示菜单 P-3
		default:break;
	}
//显示闹钟 2 B-2
	display[0] = 10; 
	display[1] = 12; 
	display[2] = 21; 
	display[3] = 2;
}
//函数名: year_Set()
//功能描述: 年设置函数
void year_Set(void)
{ 
	switch(current_Event)
	{ 
		case 0:break;
		case 1:sys_State = 0x11;break; 
		case 2:year--;break; 
		case 4:year++;break;	//显示年
		case 8:sys_State = 0x10;break;	//显示菜单 P-1
		default:break;
	}
	month_Day[1] = ( (year%400 == 0) || ( (year%4 == 0)&&(year%100 != 0) ) ) ? 29 : 28; 
	if(day > month_Day[month - 1]) 
		day = 1;
	tube(0x00);	//数码管闪烁
	display[0] = year / 1000;
	display[1] = year / 100 % 10; 
	display[2] = year / 10 % 10; 
	display[3] = year % 10;
}
//函数名: week_Set()
//功能描述: 周设置函数
void week_Set(void)
{ 
	switch(current_Event)
	{ 
		case 0:break;
		case 1:sys_State = 0x13;break;	//显示周
		case 2:week--;if(week == 0) week = 7;break;	//周--
		case 4:week++;if(week == 8) week = 1;break;	//周++
		case 8:sys_State = 0x10;break;	//显示菜单 P-1
		default:break;
	}
	tube(0x07); display[0] = 20; display[1] = 12; display[2] = 21; display[3] = week;
}
//函数名: month_Set(void)
//功能描述: 月设置函数
void	month_Set(void)
{ 
switch(current_Event)
	{ 
		case 0:break;
		case 1:sys_State = 0x18;break;	//设置日
		case 2:month--; if(month == 0)month = 12; break;	//月--
		case 4:month++; if(month == 13)month = 1; break;	//月++
		case 8:sys_State = 0x10;break;	//显示菜单 P-1
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
	//函数名: void	day_Set(void)
//功能描述: 日设置函数
void day_Set(void)
{ 
	switch(current_Event)
	{
		case 0:break;
		case 1:sys_State = 0x12;break;	//显示月日
		case 2:
		{ 
			day--; 
			if(day == 0) 
				day = month_Day[month - 1];
//使得每个月的最大天数与对应月份实际天数相等
			break; 
		} 
		case 4:
		{ 
			day++; 
			if(day > month_Day[month - 1])
//当其超过每月最大天数时，天数置一
			{ 
				day = 1; 
			} 
			break; 
		} 
		case 8:sys_State = 0x10;break;	//显示菜单 P-1
		default:break; 
		} 
		tube(0x03); 
		decimal_Point_On(0x02); 
		display[0] = month / 10; 
		display[1] = month % 10;
		display[2] = day / 10; 
		display[3] = day % 10;;
}
	//函数名: void	hour_Set(void)
//功能描述: 小时设置函数
void	hour_Set(void)
{ 
	switch(current_Event)
	{ 
		case 0:break;
		case 1:sys_State = 0x1b;break;	//设置分钟
		case 2:hour--;if(hour < 0) hour = 23;break;	//小时--
		case 4:hour++;if(hour == 24) hour = 0;break;	//小时++
		case 8:sys_State = 0x10;break;	//显示菜单 P-1
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
		case 1:sys_State = 0x14;break;	//显示时分
		case 2:minute--;if(minute < 0) minute = 59;break;	//分钟--
		case 4:minute++;if(minute == 60) minute = 0;break;	//分钟++
		case 8:sys_State = 0x10;break;	//显示菜单 P-1
		default:break;
	} 
	tube(0x03); 
	decimal_Point(0x02);
	display[0] = hour / 10; 
	display[1] = hour % 10; 
	display[2] = minute / 10; 
	display[3] = minute % 10;
}
	//函数名: void	alarm1_Hour_Set(void)
//功能描述: 闹钟 1--小时设置
void	alarm1_Hour_Set(void)
{ 
	switch(current_Event) 
	{ 
		case 0:break;
		case 1:sys_State = 0x34;break;	//设置闹钟 1 分钟
		case 2:alarm1_Hour--;if(alarm1_Hour < 0) alarm1_Hour = 23;break;	//闹钟 1 小时--
		case 4:alarm1_Hour++;if(alarm1_Hour == 24) alarm1_Hour = 0;break;	//闹钟 1 小时++
		case 8:sys_State = 0x31;break; default:break;
	} 
	tube(0x0c); 
	decimal_Point(0x02); 
	display[0] = alarm1_Hour / 10; 
	display[1] = alarm1_Hour % 10; 
	display[2] = alarm1_Minute / 10; 
	display[3] = alarm1_Minute % 10;
}
//函数名: void	alarm1_Minute_Set(void)
//功能描述: 闹钟 1--分钟设置
void	alarm1_Minute_Set(void)
{ 
	switch(current_Event)
	{
		case 0:break;	//显示闹钟 1 B-1
		case 1:sys_State = 0x33;break;	//设置闹钟 1 小时
		case 2:alarm1_Minute--;if(alarm1_Minute < 0) alarm1_Minute = 59;break;	//闹钟 1 分钟-case 4:alarm1_Minute++;if(alarm1_Minute == 60) alarm1_Minute = 0;break;	//闹钟 1 分钟++
		case 8:sys_State = 0x31;break; default:break;
	} 
	tube(0x03); 
	decimal_Point(0x02); 
	display[0] = alarm1_Hour / 10; 
	display[1] = alarm1_Hour % 10; 
	display[2] = alarm1_Minute / 10; 
	display[3] = alarm1_Minute % 10;
}	//显示闹钟 1 B-1	
//函数名: void	alarm2_Hour_Set(void)
//功能描述: 闹钟 2--小时设置
void	alarm2_Hour_Set(void)
{ 
	switch(current_Event)
	{ 
		case 0:break;	
		case 1:sys_State = 0x38;break;	//设置闹钟 2 分钟
		case 2:alarm2_Hour--;if(alarm2_Hour < 0) alarm2_Hour = 23;break;	//闹钟 2 小时--
		case 4:alarm2_Hour++;if(alarm2_Hour == 24) alarm2_Hour = 0;break;	//闹钟 2 小时++
		case 8:sys_State = 0x35;break;	//显示闹钟 2 B-2
		default:break;
	} 
	tube(0x0c); 
	decimal_Point(0x02); 
	display[0] = alarm2_Hour / 10; 
	display[1] = alarm2_Hour % 10; 
	display[2] = alarm2_Minute / 10; 
	display[3] = alarm2_Minute % 10;
}
	//函数名: void	alarm2_Minute_Set(void)
//功能描述: 闹钟 2--分钟设置
void	alarm2_Minute_Set(void)
{ 
	switch(current_Event)
	{ 
		case 0:break;
		case 1:sys_State = 0x37;break;	//设置闹钟 2 小时
		case 2:alarm2_Minute--;if(alarm2_Minute < 0) alarm2_Minute = 59;break;	//闹钟 1 分钟-case 4:alarm2_Minute++;if(alarm2_Minute == 60) alarm2_Minute = 0;break; //闹钟 1 分钟++ case 8:sys_State = 0x35;break;	//显示闹钟 2 B-2
		default:break;
	} 
	tube(0x03); 
	decimal_Point(0x02);
	display[0] = alarm2_Hour / 10; 
	display[1] = alarm2_Hour % 10; 
	display[2] = alarm2_Minute / 10; 
	display[3] = alarm2_Minute % 10;
}
//函数名: year_Display()
//功能描述: 年显示函数
void year_Display(void)
{ 
	switch(current_Event)
	{ 
		case 0:break;
		case 1:sys_State = 0x16;break;	//设置年 case 2:sys_State = 0x12;break;	//显示月日 case 4:sys_State = 0x15;break;	//显示秒 case 8:sys_State = 0x10;break;	//显示菜单 P-1
		default:break; 
	} display[0] = year / 1000; 
	display[1] = year / 100 % 10; 
	display[2] = year / 10 % 10; 
	display[3] = year % 10;
}
	//函数名: void	month_Day_Display(void)
//功能描述: 月-日显示函数
void	month_Day_Display(void)
{ 
	switch(current_Event)
	{
		case 0:break;
		case 1:sys_State = 0x17;break;	//设置月
		case 2:sys_State = 0x13;break;	//显示周
		case 4:sys_State = 0x11;break;	//显示年
		case 8:sys_State = 0x10;break; default:break;
	} 
	display[0] = month / 10; 
	display[1] = month % 10; 
	display[2] = day / 10; 
	display[3] = day % 10;
	decimal_Point_On(0x02);
}
//函数名: void	week_Display(void)
//功能描述: 星期显示函数
void	week_Display(void)
{ 
	switch(current_Event)
	{ 
		case 0:break;	//显示菜单 P-1
		case 1:sys_State = 0x19;break;	//设置周
		case 2:sys_State = 0x14;break;	//显示时分
		case 4:sys_State = 0x12;break;	//显示月日
		case 8:sys_State = 0x10;break;	//显示菜单 P-1
		default:break;
	} 
	display[0] = 20; 
	display[1] = 12; 
	display[2] = 21; 
	display[3] = week;
}
	//函数名: void	hour_Minute_Display(void)
//功能描述: 时-分显示函数
void hour_Minute_Display(void)
{ 
	switch(current_Event)
	{ 
		case 0:break;
		case 1:sys_State = 0x1a;break;	//设置时
		case 2:sys_State = 0x15;break;	//显示秒
		case 4:sys_State = 0x13;break;	//显示周
		case 8:sys_State = 0x10;break; default:break;
	} 
	display[0] = hour / 10; 
	display[1] = hour % 10; 
	display[2] = minute / 10; 
	display[3] = minute % 10;	//显示菜单 P-1
	decimal_Point(0x02);	//点亮小数点
}
	//函数名: void	second_Display(void)
//功能描述: 秒显示函数
void second_Display(void)
{ 
	switch(current_Event)
	{ 
		case 0:break; case 1:break;
		case 2:sys_State = 0x11;break;	//显示年
		case 4:sys_State = 0x14;break;	//显示时分
		case 8:sys_State = 0x10;break;	//显示菜单 P-1
		default:break; 
	} 
	display[0] = 10; 
	display[1] = 10; 
	display[2] = second / 10; 
	display[3] = second % 10;
}
	//函数名: void	swatch(void)
//功能描述: 秒表函数
void	swatch(void)
{ 
	static	uint data count; 
	if(swatch_Start_Flag ==1)
	{
		if(f_systick_10ms == 1)	//产生跑秒
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
		case 1:break;	//秒表暂停
		case 2:count = 0; swatch_Start_Flag = 0; break;	//秒表清零
		case 4:swatch_Start_Flag = !swatch_Start_Flag; break;	//秒表计数开始
		case 8:sys_State = 0x20;break;	//显示菜单 P-2
		default:break;
	} 
	display[0] = count/1000; 
	display[1] = count/100%10; 
	display[2] = count/10%10; 
	display[3] = count%10;
}
	//函数名: void	alarm1_Clock(void)
//功能描述: 闹钟 1 函数
void alarm1_Clock(void)
{
	switch(current_Event)
	{ 
		case 0:break;
		case 1:	//设置
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
	case 2:	alarm1_Flag = 1;break;	//显示 ON，开闹钟。
	case 4:alarm1_Flag = 0;break; //显示 OFF,关闭闹钟。 
	case 8:sys_State = 0x31;break; //显示闹钟 1 B-1
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
	//函数名: void	alarm2_Clock(void)
//功能描述: 闹钟 2 函数
void alarm2_Clock(void)
{ 
	switch(current_Event)
	{
		case 0:break;
		case 1:	//设置
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
	case 2:	alarm2_Flag = 1;break;	//显示 ON，开闹钟。
	case 4:alarm2_Flag = 0;break;	//显示 OFF,关闭闹钟。
	case 8:sys_State = 0x35;break;	//显示闹钟 2 B-2
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
//函数名: void	alarm_Clock_Loud(void) //功能描述:
void alarm_Clock_Loud(void)
{ 
	if(alarm1_Flag==1) 
	{
		if(alarm1_Hour == hour && alarm1_Minute == minute )	//闹钟定时时间到
		{ 
			if(alarm1_State == 0)
			{ 
				alarm1_State = 1;
			}
		}
		else
		{
			if(alarm1_State == 1)	//闹钟响铃时间内没有手动停止闹钟则一分钟后闹钟自动停止
			{ 
				buzzer = 1;
			}
			alarm1_State = 0;
		}	//闹钟定时时间未到开始标致位清零
		if(alarm1_State == 1)	//闹钟开始响
		{ 
			if(flag_Alarm_blink==1)
			{ 
				buzzer= 1;
			}
			else
			{
				buzzer= 0;
			}
			switch(current_Event)	//手动控制--任意键停止闹钟
			{ 
				case 0:break; 
				case 1:alarm1_State = 2;break; 
				case 2:alarm1_State = 2;break; 
				case 4:alarm1_State = 2;break; 
				case 8:alarm1_State = 2;break;
			} 
		} else if(alarm1_State == 2)
		{
			buzzer = 1;	//关闭蜂鸣器
			alarm1_State = 3;	//handstop to stop
		} 
	} 
	if(alarm1_Flag==1||alarm2_Flag==1)
	{ 
		if(alarm2_Hour == hour && alarm2_Minute == minute)	//闹钟定时时间到
		{ 
			if(alarm2_State == 0)
			{ 
				alarm2_State = 1;
			}
		}
		else
		{ 
			if(alarm2_State == 1)
//闹钟响铃时间内没有手动停止闹钟则一分钟后闹钟自动停止
			{ 
				buzzer = 1; 
			} 
			alarm2_State = 0;
//闹钟定时时间未到开始标致位清零
		}
		if(alarm2_State == 1)
//闹钟开始响
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
//手动控制--任意键停止闹钟
			{
				case 0:break; 
				case 1:alarm2_State = 2;break; 
				case 2:alarm2_State = 2;break;
				case 4:alarm2_State = 2;break; 
				case 8:alarm2_State = 2;break;
			} 
		} else if(alarm2_State == 2)
		{
			buzzer = 1;	//关闭蜂鸣器
			alarm2_State = 3;	//handstop to stop
		}
	}
}
//函数名: Mn_app(void) 
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
//菜单，母状态
		case 0x10:meau1_Display();break;		//显示菜单 P-1
		case 0x20:meau2_Display();break;		//显示菜单 P-2
		case 0x30:meau3_Display();break;
//时钟子状态		//显示菜单 P-3
		case 0x11:year_Display();break;		//显示年
		case 0x12:month_Day_Display();break;		//显示月日
		case 0x13:week_Display();break;		//显示周
		case 0x14:hour_Minute_Display();break;		//显示时分
		case 0x15:second_Display();break;		//显示秒
		case 0x16:year_Set();break;		//设置年
		case 0x17:month_Set();break;		//设置月
		case 0x18:day_Set();break;		//设置日
		case 0x19:week_Set();break;		//设置周
		case 0x1a:hour_Set();break;		//设置时
		case 0x1b:minute_Set();break;
//秒表子状态
		case 0x21:swatch();break;	//秒表
//闹钟子状态		//设置分
		case 0x31:alarm_meau1();break;	//显示闹钟 1 B-1	
		case 0x32:alarm1_Clock();break;	//显示闹钟 1 on/off	
		case 0x33:alarm1_Hour_Set();break;	//设置闹钟 1 时	
		case 0x34:alarm1_Minute_Set();break;	//设置闹钟 1 分	
		case 0x35:alarm_meau2();break;	//显示闹钟 2 B-1
		case 0x36:alarm2_Clock();break;	//显示闹钟 2 on/off
		case 0x37:alarm2_Hour_Set();break;	//设置闹钟 2 时
		case 0x38:alarm2_Minute_Set();break;	//设置闹钟 2 分
		default:break;
	}
}
