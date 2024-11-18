#include <msp430x14x.h>
#include "lcd.h"
#include "osc.h"
#include "wdi.h"

extern unsigned char dispBuf[64];

/********************************
*功能:LCD 初始化
*说明:控制LCD 背光灯设为输出，片选、读写信号设为输出
********************************/
void LCD_Init()
{
  P5DIR|=LCD_LED;
  P5DIR|=CSB+RDB+WRB;
  P5DIR|=DATA;
  Delay_1ms();
  LCDCmd_Write(SYS_EN);
  Delay_1ms();
  LCDCmd_Write(Normal);
  Delay_1ms();
  LCDCmd_Write(RC_32K);
  Delay_1ms();
  LCDCmd_Write(LCD_ON);
}

/*********************************
*功能:LCD 送数
*说明:LCD-FL603 数据在高锁存
*参数:bit
*返回:无
**********************************/
void LCDBit_Write(unsigned char bit)
{
  if(!bit){DATA_0;}
  else{DATA_1;}
  asm("NOP");
  WRB_0;
  Delay_us(2);
  WRB_1;
}
unsigned char LCDBit_Read(void)
{
   unsigned char retc;
   if(!(P5IN&DATA))retc = 0;
   else retc = 1;
   asm("NOP");
   RDB_0;
   Delay_us(10);
   RDB_1;
   return retc;
}
/*********************************
*功能: LCD 送数
*说明: LCD-FL603 命令:101 
*参数: addr 地址，data 数据
*返回；无
**********************************/
void LCDByte_Write(unsigned char addr,unsigned char data)
{
  unsigned char i=0;;
  CSB_0;
  Delay_1us();
  LCDBit_Write(1);
  LCDBit_Write(0);
  LCDBit_Write(1);
  addr=addr<<2;                  
  for(i=0;i<6;i++)//送地址
  {
    LCDBit_Write(addr&0x80);
    addr=addr<<1;
  }
  for(i=0;i<4;i++)//送数
  {
    LCDBit_Write(data&0x01);
    data=data>>1;
  }
  Delay_1us();
  CSB_1;
}



/**********************************
* 功能:LCD 送命令
* 说明:LCD-FL603 命令:100
* 参数:unsigned char cmd
* 返回:无
***********************************/
void LCDCmd_Write(unsigned char cmd)
{
  unsigned char i;
  CSB_0;
  LCDBit_Write(1);
  LCDBit_Write(0);
  LCDBit_Write(0);
  for(i=0;i<8;i++)
  {
    LCDBit_Write(cmd&0x80);
    cmd=cmd<<1;
  }
  LCDBit_Write(0);
  CSB_1;
}

/************************************
*功能:将显示缓冲区数据写入LCD,长度为length
*说明:LCD-FL603
*参数:*p 指向显示缓冲区，length 显示缓冲区长度
*返回:无
************************************/
void LCD_Update(unsigned char *p,unsigned char length)
{
  unsigned char i,j;
  CSB_0;
  LCDBit_Write(1);
  LCDBit_Write(0);
  LCDBit_Write(1);
  for(i=0;i<6;i++)
  {
    LCDBit_Write(0);
  }
  for(i=0;i<length;i++)
  {
    for(j=0;j<4;j++)
    {
      LCDBit_Write((*p)&0x01);
      (*p)=(*p)>>1;
    }
    p++;
  }
  CSB_1;
}
void LCD_Read(unsigned char *p,unsigned char length)
{
  unsigned char i,j;
  CSB_0;
  LCDBit_Write(1);
  LCDBit_Write(1);
  LCDBit_Write(0);
  for(i=0;i<6;i++)
  {
    LCDBit_Write(0);
  }
  DATA_IN();
  for(i=0;i<length;i++)
  {
      *p = 0;
    for(j=0;j<4;j++)
    {
      if(LCDBit_Read()) (*p)++;
      (*p) <<= 1;
    }
    p++;
  }
  DATA_OUT();
  CSB_1;
}
void LCD_Clr()
{
  unsigned char buf[48];     
  WDI_Clr();
  for(unsigned char i=0; i<48;i++) buf[i] = 0;
  LCD_Update(buf,48);
}

void LCD_All()
{
  unsigned char buf[48];    
  WDI_Clr();
  for(unsigned char i=0; i<48;i++) buf[i] = 0xff;
  LCD_Update(buf,48);
}



