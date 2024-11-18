#ifndef DIO_H
#define DIO_H

#define DI1 BIT3
#define DI2 BIT4
#define DI3 BIT5
#define DI4 BIT6

#define DO1 BIT6
#define DO2 BIT7

#define sw_ctrl_a BIT3
#define sw_ctrl_b BIT4

void DIO_Init();
void DIO_Chk();
void DO_Out(void);
void ClrDO1();
void SetDO1();
void ClrDO2();
void SetDO2();
void analog_sw_ctrl_init(void);
void analog_sw_ctrl(unsigned char ch);
void DI_delayDeal(void);

#endif


