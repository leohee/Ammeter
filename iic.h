#ifndef IIC_H
#define IIC_H
//Port3
#define WP    BIT1
#define SCL   BIT2
#define SDA   BIT3
     
#define SetSdaH()       P3OUT|=BIT3    //SDA=1
#define SetSdaL()       P3OUT&=~BIT3   //SDA=0
#define SetSclH()       P3OUT|=BIT2    //SCL=1
#define SetSclL()       P3OUT&=~BIT2   //SCL=0

#define SDA_IN          P3DIR&=~BIT3  //SDA 为输入
#define SDA_OUT         P3DIR|=BIT3  //SDA 为输出

#define SCL_IN          P3DIR&=~SCL
#define SCL_OUT         P3DIR|=SCL;

#define ReadSda()      ((P3IN>>3)&0x01) //Read SDA

void i2c_start(void);
void i2c_stop(void);
void i2c_ack(void);
unsigned char ReceiveAck(void);
void i2c_Nack(void);
void i2c_init(void);
void i2c_send8bit( unsigned char dat);
unsigned char i2c_receive8bit(void);
void EE_Page_Write( unsigned int adr,unsigned char size,unsigned char *dat);
void EEPROM_Write(unsigned int adr,unsigned char dat);
void EE_Page_Read(unsigned int adr, unsigned char size,unsigned char *dat);
unsigned char EEPROM_Read(unsigned int adr);
void RegisterWriteDriver(void);
void ReadRegister( void );
void InitRegister(void);

void LoadInt2Char( unsigned int * IntBuf, unsigned char * CharBuf, unsigned char Size );
void LoadChar2Int( unsigned int * IntBuf, unsigned char * CharBuf, unsigned char Size );
void LoadLong2Char( unsigned long * LongBuf, unsigned char * CharBuf, unsigned char Size );
void LoadChar2Long( unsigned long * LongBuf, unsigned char * CharBuf, unsigned char Size );

#endif



