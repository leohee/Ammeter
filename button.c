#include <msp430x14x.h>
#include "button.h"


void Button_Init (void)
{
	P2SEL&=~(BUTTON0+BUTTON1+BUTTON2);
	P2DIR&=~(BUTTON0+BUTTON1+BUTTON2);
	P2IE|=(BUTTON0+BUTTON1+BUTTON2);    //P2.0~P2.2 使能中断
	P2IES|=(BUTTON0+BUTTON1+BUTTON2);    //对应管脚下降沿触发
	P2IFG&=~(BUTTON0+BUTTON1+BUTTON2);
}


