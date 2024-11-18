#include <msp430x14x.h>
#include "timer.h"

void FCompareUA_Init()
{
  P1SEL|=FCompareUA;//P1.1 设定为定时器捕获，CCI0A 输入
  P1DIR&=~FCompareUA;
  TACTL|=TASSEL_1+ID_0+TACLR;//定时器A的时钟源为ACLK,不分频,TAR清零
  TACCTL0|=CM_1+CCIS_0+SCS+CAP;//CCI0A 为信号源，上升沿捕获，同步捕获
}

void FCompareUA_Stop()
{
  TACTL|=MC_0;//停止计数
  TACCTL0&=~CCIE;//关闭中断
}

void FCompareUA_Start()
{
  TACTL|=MC1+TACLR;//处于连续计数模式
  TACCTL0|=CCIE;//捕获中断允许
}

void TimerA_Clr()
{
  TACTL=0x00;
  TACCTL0=0x00;
}

void AngleA_Init()
{
  P1SEL|=FUA+FIA;//P1.2,P1.3 作为TA1、TA2 输入
  P1DIR&=~(FUA+FIA);
  TACTL|=TASSEL_1+ID_0+TACLR;//定时器A的时钟源为ACLK,不分频，TAR清零
  TACCTL1|=CM_1+CCIS_0+SCS+CAP;//CCI1A 为信号源，上升沿捕获，同步捕获
  TACCTL2|=CM_1+CCIS_0+SCS+CAP;//CCI2A 为信号源，上升沿捕获，同步捕获
}

void AngleUA_Start()
{
  //TACTL|=MC1;//处于连续计数模式
  TACCTL1|=CCIE;//TA1 捕获中断允许
}

void AngleUA_Stop()
{
  TACTL|=MC_0;//停止计数
  TACCTL1&=~CCIE;//TA1 中断禁止
}

void AngleIA_Start()
{
  TACTL|=MC1+TACLR;//处于连续计数模式
  TACCTL2|=CCIE;//TA2 捕获中断允许
}

void AngleIA_Stop()
{
  //TACTL|=MC_0;//停止计数
  TACCTL2&=~CCIE;//TA2 中断禁止
}

void TimeB_Init()
{
  TBCTL=CNTL_1+TBSSEL_2+ID_3+TBCLR;//定时长度10位,时钟源为MCLK,8分频,计数器清零
  TBCCTL0=CCIE;//TBCCR0 中断允许
  TBCCR0=1250;//时间间隔为400us
}
void TimeB_Stop()
{
  TBCTL=MC_0;//停止模式
}
void TimeB_AddMode()
{
  TBCTL|=MC_1;//增计数模式
}
void TimeB_Clear()
{
  TBCTL=TBCLR;//定时器B清零
}

void WDT_timer_init(void)
{
    WDTCTL=WDT_ADLY_16;    //启动看门狗定时器:作定时器,ACLK,16ms
    IE1 |= WDTIE;        //启动定时器中断
}


