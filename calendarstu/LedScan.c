#include <reg51.h>
#include "ledscan.h"
#include "app.h"
#include "timer.h"			
#define	ledNum	4	//数码管个数		
#define	dataPort P1	//段口		
#define	comPort	P0	//位口(4 bit)		
uchar data LedDispBuff[ledNum]; 
uchar data ScanBitCnt; 
uchar data dotBuf = 0x00; 
uchar data tubeBuf = 0x0f;	//LedNum 个数码管		
uchar data	segCode[]={ 0xc0,0xf9,0xa4,0xb0,		//	0-0	1-1	2-2	3-3
						0x99,0x92,0x82,0xf8,	//	4-4	5-5	6-6	7-7
						0x80,0x90,0xff,0x88,	//	8-8	9-9	10-空	11-A
						0x83,0xc6,0xa1,0x86,	//	12-b 13-C	14-d	15-E
						0x8e,0x8c,0xc1,0x89,	//	16-F 17-P	18-U	19-H
						0xc7,0xbf,0x91,0x92,	// 20-L 21--	22-y	23-S
						0xf7,0x8b,0xc2,0xa3,	// 24-_ 25-h	26-G	27-o
						0xc6,0xf6,0xf0,0xc8};
							//28-[	29-=	30-]	31-n
uchar	data	bitCode[]={0xfe,0xfd,0xfb,0xf7};	//数码管位选	
//函数名: Mn_led()
//功能描述: LED 数码管动态扫描函数
void	Mn_led(void)
{ 
	if(f_systick_4ms==1)
	{ 
		f_systick_4ms = 0;
		comPort |= 0x0f;	//关闭位选口
		if( tubeBuf & (0x01 << ScanBitCnt) )
		{		
			if( dotBuf & (0x01 << ScanBitCnt) )	//选择点亮哪个小数点
			{
				 dataPort = segCode[LedDispBuff[ScanBitCnt]] & 0x7F;	//段选（加上小数点）
			}
			else
			{ 
				dataPort = segCode[LedDispBuff[ScanBitCnt]];	//段选(小数点位不点亮)
			}
		}
		else
		{ 
			dataPort = 0xFF;
		}
		comPort &= bitCode[ScanBitCnt];	//位选（打开相应的位选口）
		ScanBitCnt = (ScanBitCnt + 1) % ledNum;
	}
}
//函数名: led_Display()
//功能描述: LED 数码管显示内容修改函数
void	led_Display(void)
{
	LedDispBuff[0] = display[0];
	LedDispBuff[1] = display[1];
	LedDispBuff[2] = display[2];
	LedDispBuff[3] = display[3];
}
//函数名: decimal_Point()
//功能描述: 小数点 1s 闪烁函数
void decimal_Point(uchar temp)
{
	if(flag_Dot_blink)
	{ 
		decimal_Point_On(temp); 
	}
	else
	{ 
		decimal_Point_Off();
	}
}
//函数名: decimal_Point_On(uchar lighten)
//功能描述: 点亮小数点
void decimal_Point_On(uchar lighten)
{ dotBuf = lighten;
}
//函数名: void decimal_Point_Off(void)
//功能描述: 关闭小数点
void decimal_Point_Off(void)
{ 
	dotBuf = 0x00;
}
//函数名: void tube(uchar temp1)
//功能描述: 数码管 1s 闪烁函数
void tube(uchar temp1)
{
	if(flag_Tube_blink == 1)
	{ 
		decimal_Tube_On ();
	}
	else
	{ 
		decimal_Tube_Off(temp1);//段选
	}
}
//函数名: decimal_Tube_On(uchar lighten)
//功能描述: 点亮数码管
void decimal_Tube_On(void) 
{ 
	tubeBuf = 0x0f;
}
//函数名: void decimal_Tube_Off(void)
//功能描述: 关闭数码管
void decimal_Tube_Off(uchar lighten_Tube)
{ 
	tubeBuf = lighten_Tube;
}
