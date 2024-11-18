#ifndef LCD_H
#define LCD_H

#define CSB       BIT0// 逻辑"1"禁止，逻辑"0" 使能
#define RDB       BIT1// 读时钟输入，RD 上升沿，数据从RAM 输出，主控制器在下降沿时读数据
#define WRB       BIT2// 写时钟输入，WR 上升沿时数据锁存到FL603
#define DATA      BIT3// 串行数据接口
#define LCD_LED   BIT4
#define DATA_OUT()  P5DIR|=DATA
#define DATA_IN()   P5DIR&=~DATA
#define DATA_1    P5OUT|=DATA
#define DATA_0    P5OUT&=~DATA
#define CSB_1     P5OUT|=CSB
#define CSB_0     P5OUT&=~CSB
#define RDB_1     P5OUT|=RDB
#define RDB_0     P5OUT&=~RDB
#define WRB_1     P5OUT|=WRB
#define WRB_0     P5OUT&=~WRB

#define SYS_DS    0x00
#define LCD_OFF    0x02

#define SYS_EN    0x01
#define LCD_ON    0x03
#define RC_32K    0x18
#define Normal    0xE3


unsigned char LCD_Number(unsigned char number);
unsigned char LCD_Number_Dot(unsigned char number);


void LCD_Init();

void LCDBit_Write(unsigned char bit);
void LCDByte_Write(unsigned char addr,unsigned char data);
void LCDCmd_Write(unsigned char cmd);
void LCD_Update(unsigned char *p,unsigned char length);
void LCD_Read(unsigned char *p,unsigned char length);
void LCD_Clr();
void LCD_All();


#endif


