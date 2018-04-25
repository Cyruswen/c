#include <reg51.h>
#include	"key.h"
#include	"app.h"
#include	"timer.h"			
#define	KeyPort		P2	//按键端口
#define	KeyTime		50	//防抖定时时间
#define	Key_Con_Time	500		//连按间隔时间
uchar data	KeyInBuf=0;			//读入当前键值缓冲区
uchar data	KeyOldBuf=0;			//待确认的键
uint data KeyTimeBuf=0;		//防抖动时间处理单元
uchar data	Key_state=0;		//键状态
uchar data	KeyValBuf=0; uchar data	KeyBuf[4];	
	//键状态
//函数名: void Key_Scan(void)
//功能描述: 按键检测
void Key_Scan(void)
{
	KeyInBuf= KeyPort ;
	KeyInBuf ^=0xff;	//按位取反
	switch(Key_state)
	{ 
		case 0:
		if(KeyInBuf)	//有按键按下
		{
			KeyOldBuf	= KeyInBuf;
			KeyTimeBuf = KeyTime;	//消抖时间
			Key_state = 1; 
		}
		else
		{
			KeyOldBuf = KeyInBuf;
		} break;
		case 1:	//按键消抖
		if(KeyInBuf==KeyOldBuf)
		{ 
			KeyTimeBuf--; 
			if(KeyTimeBuf==0)
			{
				KeyTimeBuf = KeyTime*12;	//长按消抖时间约 1S
				Key_state=2;
			}
		}
		else
		{
			Key_state=0;
		} break;
		case 2:	//检测长按
		if(KeyTimeBuf)
		{
			if(KeyInBuf==KeyOldBuf)
			KeyTimeBuf--; 
			else if(KeyInBuf==0)
			{
				KeyValBuf=KeyOldBuf; write_Event(KeyValBuf);	//短按
				KeyValBuf=0;
				KeyOldBuf=0;
				Key_state=0;
			}
		}
		else
		{
			KeyTimeBuf=KeyTime*10;
			Key_state=3;
		} break;
		case 3:	//检测连按
		if(KeyTimeBuf)
		{ 
			if(KeyInBuf==KeyOldBuf)
				KeyTimeBuf--; 
			else if(KeyInBuf==0)
			{
				KeyValBuf=KeyOldBuf; 
				KeyValBuf|=0x10; 
				write_Event(KeyValBuf);	//长按
				KeyValBuf=0;
				KeyOldBuf=0;
				Key_state=0;
			}
		}
		else
		{
			KeyValBuf=KeyOldBuf; 
			write_Event(KeyValBuf);
			KeyValBuf=0;
			KeyTimeBuf=KeyTime;
			Key_state=4;
		} break; 
		case 4: 
			if(KeyTimeBuf)
			{ 
				if(KeyInBuf==KeyOldBuf)
					KeyTimeBuf--; 
				else if(KeyInBuf==0)
				{
					KeyOldBuf=0;
					Key_state=0;
				}
			}
			else
			{ 
				KeyValBuf=KeyOldBuf; 
				write_Event(KeyValBuf); 
				KeyValBuf=0;
				KeyTimeBuf=Key_Con_Time;
			} break;
	}
}
//函数名: MnKey() //功能描述: 按键
void	Mn_Key(void) 
{ 
	if(f_systick_2ms)
	{ 
		f_systick_2ms=0;
		Key_Scan();
	}
}
