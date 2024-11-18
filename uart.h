#ifndef UART_H
#define UART_H

#define TXD BIT4
#define RXD BIT5

//port4
#define DE  BIT0

#define SetCom0Receive    P4OUT|=DE
#define SetCom0Send       P4OUT&=~DE

void Uart_Init(unsigned char U0);
void SetBaud(unsigned char Baud);
void Data_Converter(unsigned char *p,unsigned int value);
void Send_Data(unsigned char *p);

unsigned int CRC16(unsigned char *PuchMsg,unsigned int DataLen);

void Com0Receive_Disable();
void Com0Receive_Enable();
void ComTest(unsigned char*  Uart_Tx_Buf,   unsigned int  Uart_Tx_Cnt, unsigned char flag);

#endif



