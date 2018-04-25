#include <reg51.h>
#include "ledscan.h"
#include "app.h"
#include "timer.h"			
#define	ledNum	4	//����ܸ���		
#define	dataPort P1	//�ο�		
#define	comPort	P0	//λ��(4 bit)		
uchar data LedDispBuff[ledNum]; 
uchar data ScanBitCnt; 
uchar data dotBuf = 0x00; 
uchar data tubeBuf = 0x0f;	//LedNum �������		
uchar data	segCode[]={ 0xc0,0xf9,0xa4,0xb0,		//	0-0	1-1	2-2	3-3
						0x99,0x92,0x82,0xf8,	//	4-4	5-5	6-6	7-7
						0x80,0x90,0xff,0x88,	//	8-8	9-9	10-��	11-A
						0x83,0xc6,0xa1,0x86,	//	12-b 13-C	14-d	15-E
						0x8e,0x8c,0xc1,0x89,	//	16-F 17-P	18-U	19-H
						0xc7,0xbf,0x91,0x92,	// 20-L 21--	22-y	23-S
						0xf7,0x8b,0xc2,0xa3,	// 24-_ 25-h	26-G	27-o
						0xc6,0xf6,0xf0,0xc8};
							//28-[	29-=	30-]	31-n
uchar	data	bitCode[]={0xfe,0xfd,0xfb,0xf7};	//�����λѡ	
//������: Mn_led()
//��������: LED ����ܶ�̬ɨ�躯��
void	Mn_led(void)
{ 
	if(f_systick_4ms==1)
	{ 
		f_systick_4ms = 0;
		comPort |= 0x0f;	//�ر�λѡ��
		if( tubeBuf & (0x01 << ScanBitCnt) )
		{		
			if( dotBuf & (0x01 << ScanBitCnt) )	//ѡ������ĸ�С����
			{
				 dataPort = segCode[LedDispBuff[ScanBitCnt]] & 0x7F;	//��ѡ������С���㣩
			}
			else
			{ 
				dataPort = segCode[LedDispBuff[ScanBitCnt]];	//��ѡ(С����λ������)
			}
		}
		else
		{ 
			dataPort = 0xFF;
		}
		comPort &= bitCode[ScanBitCnt];	//λѡ������Ӧ��λѡ�ڣ�
		ScanBitCnt = (ScanBitCnt + 1) % ledNum;
	}
}
//������: led_Display()
//��������: LED �������ʾ�����޸ĺ���
void	led_Display(void)
{
	LedDispBuff[0] = display[0];
	LedDispBuff[1] = display[1];
	LedDispBuff[2] = display[2];
	LedDispBuff[3] = display[3];
}
//������: decimal_Point()
//��������: С���� 1s ��˸����
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
//������: decimal_Point_On(uchar lighten)
//��������: ����С����
void decimal_Point_On(uchar lighten)
{ dotBuf = lighten;
}
//������: void decimal_Point_Off(void)
//��������: �ر�С����
void decimal_Point_Off(void)
{ 
	dotBuf = 0x00;
}
//������: void tube(uchar temp1)
//��������: ����� 1s ��˸����
void tube(uchar temp1)
{
	if(flag_Tube_blink == 1)
	{ 
		decimal_Tube_On ();
	}
	else
	{ 
		decimal_Tube_Off(temp1);//��ѡ
	}
}
//������: decimal_Tube_On(uchar lighten)
//��������: ���������
void decimal_Tube_On(void) 
{ 
	tubeBuf = 0x0f;
}
//������: void decimal_Tube_Off(void)
//��������: �ر������
void decimal_Tube_Off(uchar lighten_Tube)
{ 
	tubeBuf = lighten_Tube;
}
