#include <msp430x14x.h>
#include "wdi.h"

//******清狗初始化*****//
void WDI_Init()
{
  P1SEL&=~WDI;//P1.4 作为普通IO口输出
  P1DIR|=WDI;//P1.4 设置为输出
  P1OUT|=WDI;
}
//*****P1.4输出取反，清硬件狗*****//
void WDI_Clr()
{
  P1OUT^=WDI;//P1.4 输出取反
}

