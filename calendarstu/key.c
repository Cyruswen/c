#include <reg51.h>
#include	"key.h"
#include	"app.h"
#include	"timer.h"			
#define	KeyPort		P2	//�����˿�
#define	KeyTime		50	//������ʱʱ��
#define	Key_Con_Time	500		//�������ʱ��
uchar data	KeyInBuf=0;			//���뵱ǰ��ֵ������
uchar data	KeyOldBuf=0;			//��ȷ�ϵļ�
uint data KeyTimeBuf=0;		//������ʱ�䴦��Ԫ
uchar data	Key_state=0;		//��״̬
uchar data	KeyValBuf=0; uchar data	KeyBuf[4];	
	//��״̬
//������: void Key_Scan(void)
//��������: �������
void Key_Scan(void)
{
	KeyInBuf= KeyPort ;
	KeyInBuf ^=0xff;	//��λȡ��
	switch(Key_state)
	{ 
		case 0:
		if(KeyInBuf)	//�а�������
		{
			KeyOldBuf	= KeyInBuf;
			KeyTimeBuf = KeyTime;	//����ʱ��
			Key_state = 1; 
		}
		else
		{
			KeyOldBuf = KeyInBuf;
		} break;
		case 1:	//��������
		if(KeyInBuf==KeyOldBuf)
		{ 
			KeyTimeBuf--; 
			if(KeyTimeBuf==0)
			{
				KeyTimeBuf = KeyTime*12;	//��������ʱ��Լ 1S
				Key_state=2;
			}
		}
		else
		{
			Key_state=0;
		} break;
		case 2:	//��ⳤ��
		if(KeyTimeBuf)
		{
			if(KeyInBuf==KeyOldBuf)
			KeyTimeBuf--; 
			else if(KeyInBuf==0)
			{
				KeyValBuf=KeyOldBuf; write_Event(KeyValBuf);	//�̰�
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
		case 3:	//�������
		if(KeyTimeBuf)
		{ 
			if(KeyInBuf==KeyOldBuf)
				KeyTimeBuf--; 
			else if(KeyInBuf==0)
			{
				KeyValBuf=KeyOldBuf; 
				KeyValBuf|=0x10; 
				write_Event(KeyValBuf);	//����
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
//������: MnKey() //��������: ����
void	Mn_Key(void) 
{ 
	if(f_systick_2ms)
	{ 
		f_systick_2ms=0;
		Key_Scan();
	}
}
