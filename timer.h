#ifndef TIMER_H
#define TIMER_H

#define FCompareUA BIT1
#define FUA        BIT2
#define FIA        BIT3

void FCompareUA_Init();
void FCompareUA_Start();
void FCompareUA_Stop();

void TimerA_Clr();

void AngleA_Init();
void AngleUA_Start();
void AngleUA_Stop();
void AngleIA_Start();
void AngleIA_Stop();

void TimeB_Init();
void TimeB_Stop();
void TimeB_AddMode();
void TimeB_Clear();

void WDT_timer_init(void);

#endif

