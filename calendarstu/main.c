#include "init.h"
#include "timer.h"
#include "ledscan.h"
#include "key.h" 
#include "app.h" 
void main(void)
{
	Mn_Init(); 
	while(1)
	{
		SysTick_intgnd();
		Mn_led();
		Mn_Key();
		Mn_app();
	}
}
