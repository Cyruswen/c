//================================================================
//文件名称: 数码管驱动头文件
//功能说明:	点亮数码管
//硬件描述: p1口
//使用晶体:	12M晶体
//创建日期:	2015.9.26
//软件版本:	v1.0    
//================================================================
#ifndef _LIGHT_H_
#define _LIGHT_H_

//======================包含头文件=================================

//=======================宏定义====================================

//=======================变量声明==================================
extern bit Flag;

//=======================函数声明==================================
 
void Mn_Light_Player(void);
void Mn_Light_toggle(void); 
void Mn_Led_Pat_Change(void);
 
#endif
/*___________________________THE END _____________________________*/