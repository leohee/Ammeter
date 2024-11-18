#include <msp430x14x.h>
#include "iic.h"
#include "osc.h"
#include "main.h"
#include "wdi.h"
#include "glb_var.h"
#include "uart.h"
unsigned char gRegisterWriteFlagCom=LOGIC_FALSE;
unsigned char gRegisterWriteFlagMul=LOGIC_FALSE;
unsigned char gRegisterWriteFlagOff=LOGIC_FALSE;
unsigned char gRegisterWriteFlagEpt=LOGIC_FALSE;
unsigned char gRegisterWriteFlagEqt=LOGIC_FALSE;
unsigned char gRegisterWriteFlagDianNeng=LOGIC_FALSE;

unsigned char gRegisterWriteStatus=0x00;
unsigned int gRegWriteLoopCnt=0;
unsigned int gE2Address=0;
unsigned char gE2Size=0;
unsigned char gE2Buf[32];



//----------------------------------------
//    功能:时序开始
//----------------------------------------
void i2c_start( void )
{
  SDA_OUT;
  SetSdaH();
  Delay_us(5);
  SetSclH();
  Delay_us(5);
  SetSdaL();
  Delay_us(5);
  SetSclL();
 // Delay_us(10);
}
//----------------------------------------
//    功能:时序结束
//----------------------------------------
void i2c_stop( void )
{
  SDA_OUT;
  SetSdaL();
  Delay_us(5);
  SetSclL();
  Delay_us(5);
  SetSclH();
  Delay_us(5);
  SetSdaH();
  Delay_us(5);
}
//----------------------------------------
//    功能:发送应答
//----------------------------------------
void i2c_ack( void )
{
  SetSclL();
  Delay_us(5);
  SDA_OUT;
  SetSdaL();
  Delay_us(5);
  SetSclL();
}
unsigned char ReceiveAck(void)
{
  unsigned Ack=0;
  SDA_IN;
   Delay_us(5);
  SetSclH();
  Delay_us(5);
  if(ReadSda() == 0x01)//判断是否接收到应答信号
  {
    Ack=0;
  }
  else
  {
    Ack=1;//产生应答
  }
  SetSclL();
  Delay_us(5);
  SDA_OUT;
  return Ack;
}
//----------------------------------------
//    功能:发送N应答
//----------------------------------------
void i2c_Nack( void )
{
  SetSdaH();
  Delay_us(5);
  SetSclH();
  Delay_us(5);
  SetSclL();
  Delay_us(5);
}                                                     
//----------------------------------------
//    功能:I2C总线初始化
//----------------------------------------
void i2c_init( void )
{
  P3SEL&=~(WP+SCL+SDA);
  P3DIR|=(WP+SCL+SDA);
  P3OUT|=(SCL+SDA);
  P3OUT&=~WP;
  i2c_stop();    
  Delay_ms(5);
}
//----------------------------------------
//    功能:发送8位(主循环体调用)
//    入口:待发字节
//    出口:成功返回真失败返回假
//    说明:上层程序不单独调用本函数
//----------------------------------------
void i2c_send8bit( unsigned char dat )  
{
  unsigned char i;

  WDI_Clr();    
  for( i = 0; i < 8; i++ )
  {
    SetSclL();
    Delay_us(5);
    if( dat & 0x80 )
    {
      SetSdaH();
    }
    else
    {
      SetSdaL();
    }
    Delay_us(5);
    SetSclH();
    dat= dat<<1;
    Delay_us(5);
  }
  SetSclL();
  Delay_us(5);
  SetSdaH();
  Delay_us(10);                 
}
//----------------------------------------
//    功能:接收8位(主循环体调用)
//    出口:数据
//    说明:上层程序不单独调用本函数
//----------------------------------------
unsigned char i2c_receive8bit( void ) 
{
  unsigned char i;
  unsigned char TempBit=0;
  unsigned char TempData=0;
  SetSclL();
  Delay_us(5);
  SetSdaH();//置数据线为输入方式
  for(i=0;i<8;i++)
  {
    Delay_us(5);
    SetSclH();//置时钟线为高使数据线上数据有效
    Delay_us(5);
    SDA_IN;
    if(ReadSda()==0x01)
    {
      TempBit=1;
    }
    else
    {
      TempBit=0;
    }
    SDA_OUT;
    TempData=(TempData<<1)|TempBit;
    SetSclL();
  }
  Delay_us(5);
  return TempData;
}
//=============================================
//    Description: write n Bytes.
//    return: none
//=============================================
void EE_Page_Write( unsigned int adr,
            unsigned char size,
            unsigned char *dat )
{
  for(unsigned char i=0;i<size;i++)
  {
    EEPROM_Write(adr+i,dat[i]);
//    Delay_ms(2);
  }
} 
//=============================================
//    Description: write 1 Byte.
//    return: none
//=============================================
void EEPROM_Write( unsigned int adr,unsigned char dat )
{
  unsigned char Temp=0;
  unsigned char LowAddress;
  unsigned char HighAddress;
  LowAddress=(unsigned char)adr;
  HighAddress=(unsigned char)(adr>>8);
  i2c_start();;//发送开始信息
  //发送从地址信息，为写操作
  i2c_send8bit(0xa0);
  Temp=ReceiveAck();
  if(!Temp) return ;
  //发送写入的地址
  i2c_send8bit(HighAddress);//高八位
  Temp=ReceiveAck();  
  if(!Temp) return ;
  i2c_send8bit(LowAddress);//低八位
  Temp=ReceiveAck();  
  if(!Temp) return ;
  //写入数据
  i2c_send8bit(dat);
  Temp=ReceiveAck(); 
  if(!Temp) return ;
  i2c_stop();
} 
//=============================================
//    Description: read n Bytes.
//    return: none
//=============================================
void EE_Page_Read(  unsigned int adr, 
            unsigned char size,
            unsigned char *dat )
{
  for(unsigned char i = 0; i <size; i++ )                                
  {
    dat[i]=EEPROM_Read(adr+i); 
//    Delay_ms(5);
  }
}
//=============================================
//    Description: read 1 Byte.
//    return: dat
//=============================================
unsigned char EEPROM_Read( unsigned int adr )
{
  unsigned char Temp;
  unsigned char Data;
  unsigned char LowAddress;
  unsigned char HighAddress;
  LowAddress=(unsigned char)adr;
  HighAddress=(unsigned char)(adr>>8);
  i2c_start();//发送开始信息
  //发送从地址信息，为写操作
  i2c_send8bit(0xa0);
  Temp=ReceiveAck(); 
  if(!Temp) return 0;
  //发送写入的地址
  i2c_send8bit(HighAddress);//高八位
  Temp=ReceiveAck(); 
  if(!Temp) return 0;
  i2c_send8bit(LowAddress);//低八位
  Temp=ReceiveAck(); 
  if(!Temp) return 0;
  i2c_start();
  i2c_send8bit(0xa1);//读
  Temp=ReceiveAck(); 
  if(!Temp) return 0;
  Data=i2c_receive8bit();
 // Acknowledge();
  i2c_stop();
  return Data;
}

void ReadRegister( void )
{
    unsigned char i;
    unsigned char Buf[24];
    
    EE_Page_Read( EEP_ADR_FLAG, 9, Buf );
    Uart_Init(3);

    if(    ( Buf[0] != '3' ) || ( Buf[1] != 'A' ) || ( Buf[2] != 'C' ) || ( Buf[3] != 'V' ) ||
        ( Buf[4] != 'E' ) || ( Buf[5] != 'R' ) || ( Buf[6] != '1' ) || ( Buf[7] != '.' ) || ( Buf[8] != '3' ) )
    {//    //3ACVER1.0
        ComTest(Buf,9,1);
        InitRegister();        //初始化EEPROM
        return;
    }
    
    //ComTest(Buf,9,0);
    
    EE_Page_Read( EEP_ADR_COM, 13, Buf);
    para.addr = Buf[0];
    if(( para.addr == 0x00 ) || ( para.addr == 0xff )){    //站号范围:1-254
        para.addr = 1;
    }
    para.baud = Buf[1];
    if( para.baud > 4){
        para.baud = 3;
    }    
    para.sysmode = Buf[2];
    if( para.sysmode > 3 ) {    //线制:三相三线,三相四线
        para.sysmode = 0;
    }    
    *((uint8*)(&para.CT)) = Buf[3];
    *((uint8*)(&para.CT)+1) = Buf[4];
    if((para.CT > MAX_CT)||(para.CT == 0)){
        para.CT = 1;
    }
    *((uint8*)(&para.PT)) = Buf[5];
    *((uint8*)(&para.PT)+1) = Buf[6];
    if((para.PT > MAX_PT)||(para.PT == 0)){
        para.PT = 1;
    }
    *((uint8*)(&para.password)) = Buf[7];
    *((uint8*)(&para.password)+1) = Buf[8];
    *((uint8*)(&para.OverVol)) = Buf[9];
    *((uint8*)(&para.OverVol)+1) = Buf[10];
    if((para.OverVol > 460)||(para.OverVol < 380)){
        para.OverVol = 430;
    }    
    *((uint8*)(&para.LowVol)) = Buf[11];
    *((uint8*)(&para.LowVol)+1) = Buf[12];
    if((para.LowVol > 700)||(para.LowVol < 200)){
        para.LowVol = 500;
    }    


EE_Page_Read(EEP_ADR_COM_VPREC, 3, Buf); // V_prec, A_prec 可显示最低值百分比
para.V_prec = Buf[0];
para.A_prec = Buf[1]+Buf[2]*256;
if (para.V_prec > SET_V_prec_hi)
{
    para.V_prec = DEFAULT_VPREC;
    EEPROM_Write(EEP_ADR_COM_VPREC, *((unsigned char*)(&para.V_prec)));
    Delay_ms (2);
}
if (para.A_prec > SET_A_prec_hi)
{
    para.A_prec = DEFAULT_APREC;
    //EEPROM_Write(EEP_ADR_COM_APREC, *((unsigned char*)(&para.A_prec)));
    EE_Page_Write(EEP_ADR_COM_APREC, 2,((unsigned char*)(&para.A_prec)));
    Delay_ms (2);
}

    EE_Page_Read( EEP_ADR_MUL,18,(unsigned char*)(&gMul[0]) );    //比例校正
    for( i = 0; i < 9; i++ ){
        if( ( gMul[ i ] > 1100 ) || ( gMul[ i ] < 900 ) ){//范围:950-1050
            gMul[ i ] = 1000;
        }
    }
    
    EE_Page_Read( EEP_ADR_OFF,18,(unsigned char*)(&gOff[0]) );    //偏移校正
    for( i = 0; i < 9; i++ ){
        if( ( gOff[ i ] > 1200 ) || ( gOff[ i ] < 800 ) ){
            gOff[ i ] = 1000;
        }
    }
    
    EE_Page_Read( EEP_ADR_DIANNENG,24,(unsigned char*)(&gDianNeng[0]) );    //电能
    for( i = 0; i < 6; i++ ){
        if( gDianNeng[ i ] > 99999999 ){
            gDianNeng[ i ] = 0;
        }
    }
//    measure.Ept = gDianNeng[ 0 ] + gDianNeng[ 1 ] + gDianNeng[ 2 ];    //有功电能
//    measure.Eqt = gDianNeng[ 3 ] + gDianNeng[ 4 ] + gDianNeng[ 5 ];    //无功电能
     
    EE_Page_Read( EEP_ADDR_energy,8,(unsigned char*)(&DN_Sample[0]) );    //电能
    if(DN_Sample[0] > 99999999){
        DN_Sample[0] = 0;
        EE_Page_Write( EEP_ADDR_energy, 4, (unsigned char*)(&DN_Sample[0]) );//data err
        Delay_ms(2);
    }
    if(DN_Sample[1] > 99999999){
        DN_Sample[1] = 0;
        gE2Buf[0] = 0,gE2Buf[1] = 0,gE2Buf[2] = 0,gE2Buf[3] = 0;    
        EE_Page_Write( EEP_ADDR_energy+4, 4, (unsigned char*)(&DN_Sample[1]));    //data err
    }        
}

//=============================================
//    Description: Register Write driver.
//    return: none
//=============================================
void RegisterWriteDriver( void )
{
    unsigned char Buf[12];
  WDI_Clr();

      if( gRegisterWriteFlagCom != LOGIC_FALSE )
      {
    gRegisterWriteFlagCom = LOGIC_FALSE;
    gE2Address = EEP_ADR_COM;
    gE2Size = 7;
    Buf[0] = para.addr;
    Buf[1] = para.baud;
    Buf[2] = para.sysmode;
    Buf[3] = *((uint8*)(&para.CT));
    Buf[4] = *((uint8*)(&para.CT)+1);
    Buf[5] = *((uint8*)(&para.PT));
    Buf[6] = *((uint8*)(&para.PT)+1);
    Buf[7] = *((uint8*)(&para.password));
    Buf[8] = *((uint8*)(&para.password)+1);    
    Buf[9] = para.V_prec;
    Buf[10] = para.A_prec&0XFF;
     Buf[11] =( para.A_prec>>8)&0XFF;
    EE_Page_Write( EEP_ADR_COM,9,Buf );    //写设置参数
    Delay_ms(2);
    EE_Page_Write ( EEP_ADR_COM_VPREC, 3, &Buf[9]);
    Delay_ms (2);    
    }
    if( gRegisterWriteFlagMul != LOGIC_FALSE )
    {
      gRegisterWriteFlagMul = LOGIC_FALSE;
      gE2Address = EEP_ADR_MUL;
      gE2Size = 24;
      EE_Page_Write( gE2Address,24,(unsigned char*)(&gMul[0]) );

    }
    if( gRegisterWriteFlagOff != LOGIC_FALSE )
    {
      gRegisterWriteFlagOff = LOGIC_FALSE;
      gE2Address = EEP_ADR_OFF;
      EE_Page_Write( gE2Address,24,(unsigned char*)(&gOff[0]) );

    }
    if( gRegisterWriteFlagDianNeng != LOGIC_FALSE )
    {
      gRegisterWriteFlagDianNeng = LOGIC_FALSE;
      gE2Address = EEP_ADR_DIANNENG;
      EE_Page_Write( gE2Address,24,(unsigned char*)(&gDianNeng[0]) );
      Delay_ms(2);
      EE_Page_Write( EEP_ADDR_energy, 8, (unsigned char*)(&DN_Sample[0]));

    }
//    Delay_ms(2);
//    EE_Page_Write( gE2Address + EEP_ADR_BACKUP_OFFSET,24,(unsigned char*)(&gDianNeng[0]) );

}
//=============================================
//    Description: Init register
//    return: none
//=============================================
void InitRegister( void )
{
    unsigned char Buf[32];
    unsigned char i;
    Buf[0] = '3';
    Buf[1] = 'A';
    Buf[2] = 'C';
    Buf[3] = 'V';
    Buf[4] = 'E';
    Buf[5] = 'R';
    Buf[6] = '1';
    Buf[7] = '.';
    Buf[8] = '3';
    EE_Page_Write( EEP_ADR_FLAG, 9, Buf );
    Delay_ms(2);

    ComTest(Buf,9,1);
    EE_Page_Read( EEP_ADR_FLAG, 9, Buf );
    ComTest(Buf,9,1);

    
    para.addr = 1;
    para.baud = 3;
    para.sysmode = 0;
    para.CT = 1;
    para.PT = 1;
    para.password = 0;
    para.OverVol = 430;
    para.LowVol = 600;
    para.V_prec = DEFAULT_VPREC;
    para.A_prec = DEFAULT_APREC;
    
    Buf[0] = 1;
    Buf[1] = 3;
    Buf[2] = 0;
    Buf[3] = *((uint8*)(&para.CT));
    Buf[4] = *((uint8*)(&para.CT)+1);
    Buf[5] = *((uint8*)(&para.PT));
    Buf[6] = *((uint8*)(&para.PT)+1);
    Buf[7] = *((uint8*)(&para.password));
    Buf[8] = *((uint8*)(&para.password)+1);
    Buf[9] = *((uint8*)(&para.OverVol));
    Buf[10] = *((uint8*)(&para.OverVol)+1);
    Buf[11] = *((uint8*)(&para.LowVol));
    Buf[12] = *((uint8*)(&para.LowVol)+1);

    Buf[13] = para.V_prec;
    Buf[14] = para.A_prec&0XFF;
    Buf[15] = (para.A_prec>>8)&0XFF;
    
    EE_Page_Write( EEP_ADR_COM,13,Buf );    //写初始化参数
    Delay_ms(2);
    EE_Page_Write( EEP_ADR_COM + EEP_ADR_BACKUP_OFFSET,13,Buf );
    Delay_ms(2);

    EE_Page_Write ( EEP_ADR_COM_VPREC, 3, &Buf[13] );
    Delay_ms (2);
    
    for( i = 0; i < 9; i++ ){//初始化校正比例参数
        gMul[ i ] = 1000;
    }    
    EE_Page_Write( EEP_ADR_MUL,18,(unsigned char*)(&gMul[0]) );    
    Delay_ms(2);
    EE_Page_Write( EEP_ADR_MUL + EEP_ADR_BACKUP_OFFSET,18,(unsigned char*)(&gMul[0]));
    Delay_ms(2);
    
    for( i = 0; i < 9; i++ ){    //初始化校正偏移量参数
        gOff[ i ] = 1000;
    }
    EE_Page_Write( EEP_ADR_OFF,18,(unsigned char*)(&gOff[0]));    
    Delay_ms(2);
    EE_Page_Write( EEP_ADR_OFF + EEP_ADR_BACKUP_OFFSET,18,(unsigned char*)(&gOff[0]) );
    Delay_ms(2);
    
    for( i = 0; i < 6; i++ ){    ////初始化电能参数
        gDianNeng[ i ] = 0;
    }
    EE_Page_Write( EEP_ADR_DIANNENG, 24, (unsigned char*)(&gDianNeng[0]) );    
    Delay_ms(2);
    EE_Page_Write( EEP_ADR_DIANNENG + EEP_ADR_BACKUP_OFFSET, 24, (unsigned char*)(&gDianNeng[0]) );
    Delay_ms(2);    
    
      //lin 080917
    DN_Sample[1] = 0;
    DN_Sample[0] = 0;
    EE_Page_Write( EEP_ADDR_energy, 8, (unsigned char*)(&DN_Sample[0]) );    //
    Delay_ms(2);
    EE_Page_Write( EEP_ADDR_energy + EEP_ADR_BACKUP_OFFSET, 8, (unsigned char*)(&DN_Sample[0]) );
    Delay_ms(2);
}








