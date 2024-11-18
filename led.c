#include <msp430x14x.h>
#include "led.h"


void LED_RUN_Init (void)
{
	P3SEL&=~LED_RUN;
	P3DIR|=LED_RUN;
	P3OUT&=~LED_RUN;
}

