#include <msp430x14x.h>
#include "osc.h"
#include"wdi.h"

//*******晶体初始化***********//
void Osc_Init(void)
{
  unsigned char i;
  WDTCTL=WDTPW+WDTHOLD;    //关闭看门狗
  BCSCTL1&=~XT2OFF;        //开启高速晶体,LFTX1为低频模式,ACLK分频因子=1
  do
  {
   IFG1&=~OFIFG;
   for(i=0xFF;i>0;i--);
  }
  while((IFG1&OFIFG)!=0);//等待晶体初始化
  BCSCTL2|=SELM1+SELS;     //MCLK=XT2,分频因子=1;SMCLK=XT2,分频因子=1;
  return;   
}


void Delay_us(unsigned int nValue)
{
  int nCount;
  int i;
  int j;
  nCount = 3;
  WDI_Clr();
  for(i = nValue;i > 0;i--)
  {
   for(j = nCount;j > 0;j--);
  }
  return;
}


void Delay_1us()
{
  int i;
  int j;
  WDI_Clr();
  for(i = 1;i > 0;i--)
  {
   for(j = 3;j > 0;j--);
  }
}


void Delay_ms(unsigned int nValue)
{
  unsigned long nCount;
  int i;
  unsigned long j;
  nCount = 2667;
  WDI_Clr();
  for(i = nValue;i > 0;i--)
  {
    for(j = nCount;j > 0;j--);
  }
  return;
}


void Delay_1ms()
{
  int i;
  unsigned long j;
  WDI_Clr();
  for(i = 1;i > 0;i--)
  {
    for(j = 2667;j > 0;j--);
  }
}



