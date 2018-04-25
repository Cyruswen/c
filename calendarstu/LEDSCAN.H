#ifndef _LEDSCAN_H_
#define _LEDSCAN_H_
#define uchar unsigned char 
#define uint unsigned int 
extern uchar data dotBuf;
extern uchar data tubeBuf; 
void Mn_led(void);
void decimal_Point_On(uchar lighten);	//点亮小数点		
void decimal_Point_Off(void); 
void decimal_Tube_On(void); 
void decimal_Tube_Off(uchar lighten1); 
void decimal_Point(uchar temp); 
void tube(uchar temp1); 
void led_Display();
#endif
