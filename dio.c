#include <msp430x14x.h>
#include "dio.h"
#include "glb_var.h"
#include "main.h"

extern unsigned char DI_status[];
extern unsigned char DO_status[];



void ClrDO1() {P2OUT &= ~DO1;DO_status[0]=0;}
void SetDO1() {P2OUT |=  DO1;DO_status[0]=1;}
void ClrDO2() {P2OUT &= ~DO2;DO_status[1]=0;}
void SetDO2() {P2OUT |=  DO2;DO_status[1]=1;}

void DIO_Init (void)
{
  P2DIR &= ~DI1;
  P2DIR &= ~DI2;
  P2DIR &= ~DI3;
  P3DIR &= ~DI4;
  P2DIR |= DO1;
  P2DIR |= DO2;
  P2OUT &= ~DO1;
  P2OUT &= ~DO2;
}

void DIO_Chk (void)
{
#ifdef DI_CHK_KEEP
    if (!DIkeepFlg1) {
      if (!(P2IN&DI1)){DIkeepFlg1 = 1;DIkeepCnt1 = 26;DI_status[0] = 1;}
      else  DI_status[0] = 0;
    }

    if (!DIkeepFlg2) {
      if (!(P2IN&DI2)){DIkeepFlg2 = 1;DIkeepCnt2 = 26;DI_status[1] = 1;}
      else  DI_status[1] = 0;
    }

    if (!DIkeepFlg3) {
	    if(!(P2IN&DI3)){DIkeepFlg3 = 1;DIkeepCnt3 = 26;DI_status[2] = 1;}
	    else  DI_status[2] = 0;

	    if (!DIkeepFlg4){
	      if(!(P3IN&DI4)){DIkeepFlg4 = 1;DIkeepCnt4 = 26;DI_status[3] = 1;}
	      else  DI_status[3] = 0;
	    }
#else
	if(!(P2IN&DI1)) DI_status[0] = 1;
	else  DI_status[0] = 0;
	if(!(P2IN&DI2)) DI_status[1] = 1;
	else  DI_status[1] = 0;
	if(!(P2IN&DI3)) DI_status[2] = 1;
	else  DI_status[2] = 0;
	if(!(P3IN&DI4)) DI_status[3] = 1;
	else  DI_status[3] = 0;
#endif

    DO_status[0]=((P2OUT>>6)&0x01);
    DO_status[1]=((P2OUT>>7)&0x01);
}


void analog_sw_ctrl_init (void)
{    
	P4DIR|=BIT3;
	P4DIR|=BIT4;

	P4OUT &= ~BIT3;
	P4OUT &= ~BIT4;
}

void analog_sw_ctrl (unsigned char ch)
{
	if(ch==0) {
	  P4OUT &= ~BIT3;  
	  P4OUT &= ~BIT4;
	} else if(ch==1) {
	  P4OUT |= BIT3;  
	  P4OUT &= ~BIT4;
	} else if(ch==2) {
	  P4OUT &= ~BIT4;  
	  P4OUT |= BIT3;
	}
}
#ifdef DI_CHK_KEEP
void DI_delayDeal(void)
{
  if (DIkeepFlg1){
    if (--DIkeepCnt1 < 2){
      DIkeepFlg1 = 0;
    }
  }

  if (DIkeepFlg2){
    if (--DIkeepCnt2 < 2){
      DIkeepFlg2 = 0;
    }
  }

  if (DIkeepFlg3){
    if (--DIkeepCnt3 < 2){
      DIkeepFlg3 = 0;
    }
  }
}
#endif

