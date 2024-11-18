
#include <msp430x14x.h>
#include "osc.h"
#include "led.h"
#include "timer.h"
#include "ad.h"
#include "wdi.h"
#include "math.h"
#include "lcd.h"
#include "button.h"
#include "dio.h"
#include "iic.h"
#include "uart.h"
#include "main.h"
#include "display_maps.h"
#include "menu_disp.h"
#include "arithmetic.h"
#include "glb_var.h"

unsigned char CRC16flag=0;
#define CRC16HiLo 1
#define CRC16LoHi 2

/*******************************************************
*时钟
*******************************************************/
unsigned char clkFlg=0;
unsigned int clkCnt=0;
unsigned int clk_en_cnt = 0;
unsigned char clk500msFlg = 0;
unsigned char clk1sFlg=0;
unsigned char keyExitCnt=0;

unsigned char smplFlg1 = 0;

unsigned int gMul[12]={0};                //校正比例
unsigned int gOff[12]={0};                //校正偏移量


/*****************************************
*参数:频率对应变量
*****************************************/
unsigned char Frequency_Flag=0;
unsigned int Frequency_Sample_Value[11]={0};
unsigned char Frequency_Count=0;

unsigned int ADC_TBCCR0 = 625;


/*******************************************
*串口0 对应变量
********************************************/
unsigned int Uart_Rx_Cnt=0;            //串口接收数据计数
unsigned int Uart_Tx_Cnt=0;            //发送数据长度
unsigned int Uart_Rx_Length=0;        //接收数据包长度 
unsigned char Uart_Rx_Buf[100]={0};
unsigned char Uart_Tx_Buf[100]={0};
unsigned char Com0_Pack_Error=0x00;
unsigned char Uart_Length_Flag=0;
unsigned char Uart_Rx_Flag=0;
unsigned int Constant_Angle=540;

unsigned char Uart_Read_Flag=0;
unsigned char Uart_Write_Flag=0;
unsigned int RcvTimeoutCount = 0;

/*******************************************
*IIC 接口对应变量
********************************************/

//unsigned int gMode=0;    //初始设置为三相四线

/*******************************************
*参数: 案件参数
********************************************/


/*===========================================================================

U,I adjust

==========================================================================*/
void u_i_adjust_deal(void)
{
    unsigned long tmp=0;
    unsigned int gMulbuf[6]={0};
    unsigned char i=0;
    float u_i[6]={0};
    if(u_i_adjust_flg == 1)
        {
              tmp = (200UL);
        gMulbuf[0] = (unsigned int)((tmp*gMul[0])/measure.Ua); 
        gMulbuf[1] = (unsigned int)((tmp*gMul[1])/measure.Ub); 
        gMulbuf[2] = (unsigned int)((tmp*gMul[2])/measure.Uc); 
              tmp = (250UL);
        gMulbuf[3] = (unsigned int)((tmp*gMul[3])/(measure.Ia*100)); 
        gMulbuf[4] = (unsigned int)((tmp*gMul[4])/(measure.Ib*100)); 
        gMulbuf[5] = (unsigned int)((tmp*gMul[5])/(measure.Ic*100)); 
        u_i_adjust_flg=1;
        for(i = 0; i < 6; i++)
            {
            if((gMulbuf[i] > 1100)||(gMulbuf[i] < 900))
                {
                
                u_i_adjust_flg=0;
                break;
                }
            else 
                gMul[i] = gMulbuf[i];
            }
        if(u_i_adjust_flg==1)
            {
            EE_Page_Write( EEP_ADR_MUL,12,(unsigned char*)(&gMul[0]));
            u_i_adjust_flg=0x0a;
            }
        }
        else if(u_i_adjust_flg == 0xaa){
            u_i[0]=fabs(Pa_deal());
            u_i[1]=fabs(Pb_deal());
            u_i[2]=fabs(Pc_deal());
           
            u_i[3] = measure.Ua*measure.Ia*1000;
            u_i[4] = measure.Ub*measure.Ib*1000;
            u_i[5] = measure.Uc*measure.Ic*1000;
            gMulbuf[0] = u_i[3]/u_i[0];                                 /////有功功率修正系数
            gMulbuf[1] = u_i[4]/u_i[1]; 
            gMulbuf[2] = u_i[5]/u_i[2];        
            for(unsigned char i = 0; i < 3; i++){
                if((gMulbuf[i] > 1050)||(gMulbuf[i] < 950))
                    gMulbuf[i] = gMul[i+6];
                else gMul[i+6] = gMulbuf[i];
            }
            EE_Page_Write( EEP_ADR_MUL+12,6,(unsigned char*)(&gMul[6]));
            u_i_adjust_flg = 0x05;
        }
    /*    while(((!(P4IN&BIT1))||(!(P4IN&BIT2)))&&((u_i_adjust_flg==0x0a)||(u_i_adjust_flg==0x05)))
            {
            Display_u_i_adjust();
            disp_refurbish(); 
            }*/
        }


//#define I_MIN_VALUE 0.3
//#define U_MIN_VALUE 50
// V_prec, A_prec 可显示最低值百分比
#define I_MIN_VALUE   (para.A_prec*0.001)  // 0.1 //0.3
#define U_MIN_VALUE  (para.V_prec)   //50
unsigned char en_cnt = 0;
void Sample_Process()                        //++定时器B停止,进入此函数处理采样数据
{
    unsigned long long tmp=0;
    float ua=0,ub=0,uc=0,ia=0,ib=0,ic=0,in=0;
    static unsigned char adStopCnt = 0; 
    //static unsigned long q_en = 0,p_en = 0;
    static unsigned long long q_en = 0,p_en = 0;
//    unsigned long vol32 = 0;
unsigned char B_blancedF=0,C_blancedF=0;
    if((smpl.flg==1)||(smplFlg1==1))
    {
        smpl.flg=0;
        smplFlg1=0;
        smpl.cnt=0;
        
//        for(unsigned char i=0;i<SMPL_MAX;i++){
//            vol32 += smpl.AD_value[0][i];
//        }
//        smpl.average[0] = vol32/SMPL_MAX;              //+++??为什么要算出这个直流分量
        
        if(!(P4IN&BIT1))
        {    //校准
            u_i_adjust_deal();
            if(u_i_adjust_flg == 0)
            {
                u_i_adjust_flg = 1;
            }
        }

        ua = VoltageA_Process();
        ia = CurrentA_Process();
        ub = VoltageB_Process();
        ib = CurrentB_Process();
        uc = VoltageC_Process();    
        ic = CurrentC_Process();
        in = CurrentN_Process();

        if (ua < U_MIN_VALUE)
        {
            ua=0;
        }

        if (ub < U_MIN_VALUE)
        {
            ub=0;
        }

        if (uc < U_MIN_VALUE)
        {
          uc=0;
        }

        if (ia < I_MIN_VALUE)
        {
            ia = 0;
            //ia = 0.25;
        }

        if (ib < I_MIN_VALUE)
        {
            ib = 0;
            //ib = 0.25;
        }

        if (ic < I_MIN_VALUE)
        {
            ic = 0;
            //ic = 0.25;
        }

        if (in < I_MIN_VALUE)
        {
            //in = 0;
            in = 0.25;
        }
    if(para.sysmode==1)
        {
        if(ia>0.05)
            {
            if(ib<0.05)
                {
                B_blancedF=1;
                }
            if(ic<0.05)
                {
                C_blancedF=1;
                }
            }
        }

        if((ia==0)&&(ib==0)&&(ic==0))in=0;
        #ifdef U_I_RATIO_ADJUST
            measure.Ua = ua*gMul[0]/1000;
            measure.Ia = ia*gMul[3]/1000;
        if(para.sysmode==1)
            {
            if(B_blancedF)
                measure.Ib=measure.Ia;
            else
                measure.Ib = ib*gMul[4]/1000;
            measure.Ub = ub*gMul[1]/1000;
            }
        else if(para.sysmode>2)
            {
            measure.Ib=0;
            measure.Ub =0;
            }
        else
                {
                measure.Ib = ib*gMul[4]/1000;
            measure.Ub = ub*gMul[1]/1000;
            }
        if(para.sysmode==1)
            {
            if(C_blancedF)
                measure.Ic=measure.Ia;
            else
                measure.Ic = ic*gMul[5]/1000;
            measure.Uc = uc*gMul[2]/1000;
            }
        else if(para.sysmode>1)
            {
            measure.Ic=0;
            measure.Uc =0;
            }
        else
                {
                measure.Ic = ic*gMul[5]/1000;
            measure.Uc = uc*gMul[2]/1000;
            }
            measure.In = in;
        #else                   
            measure.Ua=ua;
            measure.Ia=ia;
        if(para.sysmode==1)
            {
            if(B_blancedF)
                measure.Ib=measure.Ia;
            else
                measure.Ib=ib;
            measure.Ub=ub;
            }
        else if(para.sysmode>2)
            {
            measure.Ib=0;
            measure.Ub=0;
            }
        else
                {
                measure.Ib=ib;
            measure.Ub=ub;
            }
        if(para.sysmode==1)
            {
            if(C_blancedF)
                measure.Ic=measure.Ia;
            else
                measure.Ic=ic;
            measure.Uc=uc;    
            }
        else  if(para.sysmode>1)
            {
            measure.Ic=0;
            measure.Uc=0;    
            }
        else
            {
                measure.Ic=ic;
            measure.Uc=uc;    
            }
            measure.In = in;
        #endif        
       
        if(!(P4IN&BIT2)){    //校准
            u_i_adjust_deal();
            if(u_i_adjust_flg == 0){
                u_i_adjust_flg = 0xaa;
            }
        }

    //if(para.sysmode==0)
        {
           //A相    
            if((measure.Ua < U_MIN_VALUE)&&(measure.Ia < I_MIN_VALUE)){
                measure.Sa = 0;
                measure.Pa = 0;
                //measure.Pfa = 0;
                //measure.Qa = 0;
            }
            else{
                measure.Sa=measure.Ua*measure.Ia/1000;                          //视在功率
                #ifdef U_I_RATIO_ADJUST                                         //有功功率
                    measure.Pa =fabs(Pa_deal())*gMul[6]/1000000;                   
                #else
                    measure.Pa=fabs(Pa_deal());
                #endif
            }
            if(para.sysmode==1)
                {
                if(B_blancedF)
                    {
                            measure.Sb = measure.Sa;
                            measure.Pb = measure.Pa;
                    }
                else
                    {
                        measure.Sb=measure.Ub*measure.Ib/1000;
                        #ifdef U_I_RATIO_ADJUST
                            measure.Pb =fabs(Pb_deal())*gMul[7]/1000000;   
                        #else
                            measure.Pb=fabs(Pb_deal()); 
                        #endif
                    }
                }
            else if(para.sysmode>2)
                {
                    measure.Sb = 0;
                    measure.Pb = 0;
                }
            else
                {
                    //B相
                    if((measure.Ub < U_MIN_VALUE)&&(measure.Ib < I_MIN_VALUE)){
                        measure.Sb = 0;
                        measure.Pb = 0;
                        //measure.Pfb = 0;
                        //measure.Qb = 0;
                    }
                    else{
                        measure.Sb=measure.Ub*measure.Ib/1000;
                        #ifdef U_I_RATIO_ADJUST
                            measure.Pb =fabs(Pb_deal())*gMul[7]/1000000;   
                        #else
                            measure.Pb=fabs(Pb_deal()); 
                        #endif
                    }
                }
            if(para.sysmode==1)
                {
                if(C_blancedF)
                    {
                        measure.Sc = measure.Sa;
                        measure.Pc = measure.Pa;
                    }
                else
                    {
                        measure.Sc=measure.Uc*measure.Ic/1000; 
                        #ifdef U_I_RATIO_ADJUST
                            measure.Pc =fabs(Pc_deal())*gMul[8]/1000000;   
                        #else
                            measure.Pc=fabs(Pc_deal()); 
                        #endif
                    }
                }
            else if(para.sysmode>1)
                {
                    measure.Sc = 0;
                    measure.Pc = 0;
                }
            else
                {
                //C相
                if((measure.Uc < U_MIN_VALUE)&&(measure.Ic < I_MIN_VALUE)){
                    measure.Sc = 0;
                    measure.Pc = 0;
                    //measure.Pfc = 0;
                    //measure.Qc = 0;
                }
                else{
                    measure.Sc=measure.Uc*measure.Ic/1000; 
                    #ifdef U_I_RATIO_ADJUST
                        measure.Pc =fabs(Pc_deal())*gMul[8]/1000000;   
                    #else
                        measure.Pc=fabs(Pc_deal()); 
                    #endif
                } 
            }
        }
        adStopCnt = 0; 
        //开启AD 转换，并开始定时器B
        Adc12Open();
        Adc12Start();
        TimeB_Init();
        TimeB_AddMode();
/**************??2011.06.28 add zhu****************      
    if((!(P4IN&BIT1))||(!(P4IN&BIT2)))
        u_i_adjust_deal();
**************************************************/
        if(measure.Pa > measure.Sa){measure.Pa = measure.Sa;} 
        measure.Pfa=measure.Pa/measure.Sa;                              //功率因数
        measure.Qa=(float)sqrt(fabs(measure.Sa*measure.Sa - measure.Pa*measure.Pa));//无功功率
        
        if(measure.Pb > measure.Sb){measure.Pb = measure.Sb;}
        measure.Pfb=measure.Pb/measure.Sb;       
        measure.Qb=(float)sqrt(fabs(measure.Sb*measure.Sb - measure.Pb*measure.Pb));
        
        if(measure.Pc > measure.Sc){measure.Pc = measure.Sc;}
        measure.Pfc=measure.Pc/measure.Sc;
        measure.Qc=(float)sqrt(fabs(measure.Sc*measure.Sc - measure.Pc*measure.Pc));
        
        measure.Uva = sqrt(measure.Ua*measure.Ua + measure.Ua*measure.Ub + measure.Ub*measure.Ub);
        measure.Uvb = sqrt(measure.Ub*measure.Ub + measure.Ub*measure.Uc + measure.Uc*measure.Uc);
        measure.Uvc = sqrt(measure.Uc*measure.Uc + measure.Uc*measure.Ua + measure.Ua*measure.Ua);     
/************************************/
    if(para.sysmode==2)
        {
            measure.Uvb=0;    
         measure.Uvc=0;    
            measure.Ut = (measure.Ua + measure.Ub)/2;
            measure.Uvt = measure.Uva;
        }
    else
    if(para.sysmode==3)
        {
            measure.Uva=0;    
            measure.Uvb=0;    
         measure.Uvc=0;    
            measure.Ut = measure.Ua;
            measure.Uvt = 0 ;
        }
    else
        {
            measure.Ut = (measure.Ua + measure.Ub + measure.Uc)/3;
            measure.Uvt = (measure.Uva + measure.Uvb + measure.Uvc)/3;
        }
     /************************************/          
        measure.St=(measure.Sa+measure.Sb+measure.Sc);
        measure.Pt=(measure.Pa+measure.Pb+measure.Pc);          //总有功功率  
        measure.Qt=(measure.Qa+measure.Qb+measure.Qc);          //总无功功率        
        measure.Pft=measure.Pt/measure.St;                      //总功率因数 
        
        if(en_cnt++ > 9){    
            en_cnt = 0;
            //p_en = p_en + (long)(measure.Pt*para.CT*para.PT*10)*(long)clk_en_cnt;  //++ clk_en_cnt=16ms ///???为什么要乘10
            p_en = p_en + (unsigned long long )((measure.Pt*para.CT*para.PT*10)*clk_en_cnt);
            tmp = p_en/(225000UL);                                                 //+++ 225000=3600*62.5
            DN_Sample[0]=DN_Sample[0]+tmp;                        //有功电能计数
            measure.Ept = DN_Sample[0];
            p_en %= (225000UL);                                   
              
            //q_en = q_en + (long)(measure.Qt*para.CT*para.PT*10)*(long)clk_en_cnt;  
            q_en = q_en + (unsigned long long )((measure.Qt*para.CT*para.PT*10)*clk_en_cnt);
            tmp = q_en/(225000UL);
            DN_Sample[1]=DN_Sample[1]+tmp;                        //无功电能 
            measure.Eqt = DN_Sample[1];
            q_en %= (225000UL);
            
            if(DN_Sample[0] > 99999999)DN_Sample[0] = 0;
         if(DN_Sample[1] > 99999999)DN_Sample[1] = 0;        
            EE_Page_Write( EEP_ADDR_energy, 8, (unsigned char*)(&DN_Sample[0]));    //data err
            
            clk_en_cnt = 0;
        }
    }
    else{
        if(adStopCnt++ > 220){
            adStopCnt = 0; 
            Adc12Init();
            ADC12Channel();
            Adc12Open();
            Adc12Start();
            TimeB_Init();
            TimeB_AddMode();
        }
    }
}



//==================================
//=================================


/***************************************
*功能: 频率计算处理函数
***************************************/
void Frequency_Process()
{
  float Freq=0;
  unsigned int buf[10]={0};
  unsigned char Frequency_True_Count=0;
  static unsigned char Fch=0;
  if(Frequency_Flag==1)
  {
    Frequency_Flag=0;
    for(unsigned char i=0;i<10;i++)
    {
      if(Frequency_Sample_Value[i+1]>Frequency_Sample_Value[i])
      {
        buf[i]=Frequency_Sample_Value[i+1]-Frequency_Sample_Value[i];
      }
      else
      {
        buf[i]=65536-Frequency_Sample_Value[i]+Frequency_Sample_Value[i+1];
      }
    }
    Frequency_True_Count=0;
    for(unsigned char i=1;i<10;i++)
    {
      if((500<buf[i])&&(buf[i]<820))//采样频率必须在40HZ~65HZ范围内，否则认为采样的值有干扰，丢掉该值
      {
        buf[Frequency_True_Count]=buf[i];
        Frequency_True_Count++;
      }
    }
    for(unsigned char i=0;i<Frequency_True_Count;i++)
    {
      Freq=Freq+buf[i];
    }
    Freq=Freq/Frequency_True_Count;
//    Frequency_Temp=(unsigned int)Freq;
    measure.Frq=(float)32768/Freq+0.01;                 //相电压的频率为采样周期的倒数 
    if(measure.Frq==0)
        {
        if(Fch++>2)
        Fch=0;
        analog_sw_ctrl(Fch);
        }
    ADC_TBCCR0 = (float)31250/measure.Frq + 1;          //++ 31250=1MHz/32
    
    Frequency_Count=0;
    TimerA_Clr();
    FCompareUA_Init();
    FCompareUA_Start();
 }
// if(measure.Ua<30)
// {
 //  measure.Frq=0;
 //}

}



/*****************************************
*功能: 读数据
*****************************************/
void LoadRegister(unsigned int StartAddress, unsigned int Numbers, unsigned char *buf)
{
  unsigned int i=0;
  unsigned int RegisterAddress=0;
  unsigned int Index=0;
  unsigned int ValueInt=0;
  WDI_Clr();
  for(i=0;i<Numbers;i++)
  {
    RegisterAddress=StartAddress+i;
    if((RegisterAddress>40000)&&(RegisterAddress<=40040))//只读寄存器
    {
      if(RegisterAddress<=40006)//三相电压电流
      {
        Index=RegisterAddress-40001;
        switch(Index)
        {
        case 0:
          ValueInt=(unsigned int)(measure.Ua*100);//A 相电压 乘100
          buf[i*2]=ValueInt>>8;
          buf[i*2+1]=ValueInt&0xff;
          break;
        case 1:
          ValueInt=(unsigned int)(measure.Ub*100);//B 相电压 乘100
          buf[i*2]=ValueInt>>8;
          buf[i*2+1]=ValueInt&0xff;
          break;
        case 2:
          ValueInt=(unsigned int)(measure.Uc*100);//C 相电压 乘100
          buf[i*2]=ValueInt>>8;
          buf[i*2+1]=ValueInt&0xff;
          break;
        case 3:
          ValueInt=(unsigned int)(measure.Ia*1000);//A 相电流 乘1000
          buf[i*2]=ValueInt>>8;
          buf[i*2+1]=ValueInt&0xff;
          break;
        case 4:
          ValueInt=(unsigned int)(measure.Ib*1000);//B 相电流 乘1000
          buf[i*2]=ValueInt>>8;
          buf[i*2+1]=ValueInt&0xff;
          break;
        case 5:
          ValueInt=(unsigned int)(measure.Ic*1000);//C 相电流 乘1000
          buf[i*2]=ValueInt>>8;
          buf[i*2+1]=ValueInt&0xff;
          break;
        }
      }
      else if(RegisterAddress<=40011)
      {
        if(RegisterAddress==40007)//总有功功率低16位
        {
         buf[i*2]=((long)(measure.Pt*1000)>>8)&0xff;
         buf[i*2+1]=(long)(measure.Pt*1000)&0xff;

        }
        else if(RegisterAddress==40008)//总有功功率高16位
        {
          buf[i*2]=((long)(measure.Pt*1000)>>24)&0xff;
          buf[i*2+1]=((long)(measure.Pt*1000)>>16)&0xff;
        }
        else if(RegisterAddress==40009)//总无功功率低16位
        {
          buf[i*2]=(((long)(measure.Qt*1000))>>8)&0xff;
          buf[i*2+1]=((long)(measure.Qt*1000))&0xff;
        }
        else if(RegisterAddress==40010)//总无功功率高16位
        {
          buf[i*2]=(((long)(measure.Qt*1000))>>24)&0xff;
          buf[i*2+1]=(((long)(measure.Qt*1000))>>16)&0xff;
        }
        else if(RegisterAddress==40011)//总功率因数
        {
          buf[i*2]=((unsigned int)(measure.Pft*1000))>>8; //总功率因数乘以1000倍
          buf[i*2+1]=(unsigned int)(measure.Pft*1000);
        }
      }
      else if(RegisterAddress<=40017)//三相有功功率、无功功率
      {
        if(RegisterAddress==40012)//A 相有功功率,单位 W
        {
          buf[i*2]=((unsigned int)(measure.Pa*1000))>>8;
          buf[i*2+1]=(unsigned int)(measure.Pa*1000);
        }
        else if(RegisterAddress==40013)//B 相有功功率,单位 W
        {
          buf[i*2]=((unsigned int)(measure.Pb*1000))>>8;
          buf[i*2+1]=(unsigned int)(measure.Pb*1000);
        }
        else if(RegisterAddress==40014)//C 相有功功率,单位 W
        {
          buf[i*2]=((unsigned int)(measure.Pc*1000))>>8;
          buf[i*2+1]=(unsigned int)(measure.Pc*1000);
        }
        else if(RegisterAddress==40015)//A 相无功功率,单位 W
        {
          buf[i*2]=((unsigned int)(measure.Qa*1000))>>8;
          buf[i*2+1]=(unsigned int)(measure.Qa*1000);
        }
        else if(RegisterAddress==40016)//B 相无功功率,单位 W
        {
          buf[i*2]=((unsigned int)(measure.Qb*1000))>>8;
          buf[i*2+1]=(unsigned int)(measure.Qb*1000);
        }
        else if(RegisterAddress==40017)//C 相无功功率,单位 W
        {
          buf[i*2]=((unsigned int)(measure.Qc*1000))>>8;
          buf[i*2+1]=(unsigned int)(measure.Qc*1000);
        }
      }
      else if(RegisterAddress<=40020) //三相功率因数
      {
        if(RegisterAddress==40018) //A相功率因数
        {
          buf[i*2]=((unsigned int)(measure.Pfa*1000))>>8; //功率因数乘以1000 倍
          buf[i*2+1]=(unsigned int)(measure.Pfa*1000);
        }
        else if(RegisterAddress==40019) //B相功率因数
        {
          buf[i*2]=((unsigned int)(measure.Pfb*1000))>>8; //功率因数乘以1000 倍
          buf[i*2+1]=(unsigned int)(measure.Pfb*1000);
        }
        else if(RegisterAddress==40020) //C相功率因数
        {
          buf[i*2]=((unsigned int)(measure.Pfc*1000))>>8; //功率因数乘以1000 倍
          buf[i*2+1]=(unsigned int)(measure.Pfc*1000);
        }
      }
      else if(RegisterAddress<=40033)
        {
          if(RegisterAddress==40021)  //频率
          {
            ValueInt=(unsigned int)(measure.Frq*100); //频率乘以100，精度为0.01HZ
            buf[i*2]=ValueInt>>8;
            buf[i*2+1]=ValueInt;
          }
          else if(RegisterAddress==40022) //总有功电能,低16位，0.1KWh
          {
            buf[i*2]=(measure.Ept >> 8) & 0xff;
            buf[i*2+1]= measure.Ept & 0xff;
          }
          else if(RegisterAddress==40023) //总有功电能，高16位
          {
            buf[i*2]=(measure.Ept>>24)&0xff;
            buf[i*2+1]=(measure.Ept >> 16)&0xff;
          }
          else if(RegisterAddress==40024) //总无功电能,低16位
          {
            buf[i*2]=(measure.Eqt >> 8) & 0xff;
            buf[i*2+1]=measure.Eqt & 0xff;
          }
          else if(RegisterAddress==40025) //总无功电能，高16位
          {
            buf[i*2]=(measure.Eqt >> 24) & 0xff;
            buf[i*2+1]=(measure.Eqt >> 16) & 0xff;
          }
          else if(RegisterAddress==40026) //输入有功电能,低16位(虚的,无值)
          {
            buf[i*2]=(measure.Epti >> 8) & 0xff;
            buf[i*2+1]=measure.Epti & 0xff;
          }
          else if(RegisterAddress==40027) //输入有功电能，高16位
          {
            buf[i*2]=(measure.Epti >> 24) & 0xff;
            buf[i*2+1]=(measure.Epti >> 16) & 0xff;
          }
          else if(RegisterAddress==40028) //输出有功电能,低16位(虚的,无值)
          {
            buf[i*2]=(measure.Epto >> 8) & 0xff;
            buf[i*2+1]=measure.Epto & 0xff;
          }
          else if(RegisterAddress==40029) //输出有功电能，高16位
          {
            buf[i*2]=(measure.Epto >> 24) & 0xff;
            buf[i*2+1]=(measure.Epto >> 16) & 0xff;
          }
          else if(RegisterAddress==40030) //输入无功电能,低16位(虚的,无值)
          {
            buf[i*2]=(measure.Eqti >> 8) & 0xff;
            buf[i*2+1]=measure.Eqti & 0xff;
          }
          else if(RegisterAddress==40031) //输入无功电能，高16位
          {
            buf[i*2]=(measure.Eqti >> 24) & 0xff;
            buf[i*2+1]=(measure.Eqti >> 16) & 0xff;
          }
          else if(RegisterAddress==40032) //输出无功电能,低16位(虚的,无值)
          {
            buf[i*2]=(measure.Eqto >> 8) & 0xff;
            buf[i*2+1]=measure.Eqto & 0xff;
          }
          else if(RegisterAddress==40033) //输出无功电能，高16位
          {
            buf[i*2]=(measure.Eqto >> 24) & 0xff;
            buf[i*2+1]=measure.Eqto & 0xff;
          }
        }
       else if(RegisterAddress<=40036)
        {
          if(RegisterAddress==40034)  //开关量状态
           {
             buf[i*2]=DI_status[0];
             buf[i*2+1]=DI_status[1];
           }
          else if(RegisterAddress==40035) //继电器状态
           {
             buf[i*2]=DO_status[0];
             buf[i*2+1]=DO_status[1];
           }
          else if(RegisterAddress==40036)  //开关量状态(DI3)
           {
             buf[i*2]=DI_status[2];
             buf[i*2+1]=DI_status[3];
           }
        }
      else if(RegisterAddress<=40039)//三相线电压电流
      {
        Index=RegisterAddress-40037;
        switch(Index)
        {
        case 0:
          ValueInt=(unsigned int)(measure.Uva*100);//AB 线电压 乘100
          buf[i*2]=ValueInt>>8;
          buf[i*2+1]=ValueInt&0xff;
          break;
        case 1:
          ValueInt=(unsigned int)(measure.Uvb*100);//BC 线电压 乘100
          buf[i*2]=ValueInt>>8;
          buf[i*2+1]=ValueInt&0xff;
          break;
        case 2:
          ValueInt=(unsigned int)(measure.Uvc*100);//CA 线电压 乘100
          buf[i*2]=ValueInt>>8;
          buf[i*2+1]=ValueInt&0xff;
          break;
        }
      }
    }
    else if( ( RegisterAddress > 40050 ) && ( RegisterAddress <= 40062 ) )      //分相有功、无功电能,暂时不用
    {
    if( RegisterAddress == 40051 )           //A相有功电能
    {
      buf[ i * 2 ] = ( measure.Epa >> 8 ) & 0xff;
      buf[ i * 2 + 1 ] = measure.Epa & 0xff;
    }
    else if( RegisterAddress == 40052 )
    {
      buf[ i * 2 ] = ( measure.Epa >> 24 ) & 0xff;
      buf[ i * 2 + 1 ] = ( measure.Epa >> 16 ) & 0xff;
    }
    else if( RegisterAddress == 40053 )      //B相有功电能
    {
      buf[ i * 2 ] = ( measure.Epb >> 8 ) & 0xff;
      buf[ i * 2 + 1 ] = measure.Epb & 0xff;
    }
    else if( RegisterAddress == 40054 )
    {
      buf[ i * 2 ] = ( measure.Epb >> 24 ) & 0xff;
      buf[ i * 2 + 1 ] = ( measure.Epb >> 16 ) & 0xff;
    }
    else if( RegisterAddress == 40055 )     //C相有功电能
    {
      buf[ i * 2 ] = ( measure.Epc >> 8 ) & 0xff;
      buf[ i * 2 + 1 ] = measure.Epc & 0xff;
    }
    else if( RegisterAddress == 40056 )
    {
      buf[ i * 2 ] = ( measure.Epc >> 24 ) & 0xff;
      buf[ i * 2 + 1 ] = ( measure.Epc >> 16 ) & 0xff;
    }
    else if( RegisterAddress == 40057 )     //A相无功电能
    {
      buf[ i * 2 ] = ( measure.Eqa >> 8 ) & 0xff;
      buf[ i * 2 + 1 ] = measure.Eqa & 0xff;
    }
    else if( RegisterAddress == 40058 )
    {
      buf[ i * 2 ] = ( measure.Eqa >> 24 ) & 0xff;
      buf[ i * 2 + 1 ] = ( measure.Eqa >> 16 ) & 0xff;
    }
    else if( RegisterAddress == 40059 )     //B相无功电能
    {
      buf[ i * 2 ] = ( measure.Eqb >> 8 ) & 0xff;
      buf[ i * 2 + 1 ] = measure.Eqb & 0xff;
    }
    else if( RegisterAddress == 40060 )
    {
      buf[ i * 2 ] = ( measure.Eqb >> 24 ) & 0xff;
      buf[ i * 2 + 1 ] = ( measure.Eqb >> 16 ) & 0xff;
    }
    else if( RegisterAddress == 40061 )     //C相无功电能
    {
      buf[ i * 2 ] = ( measure.Eqc >> 8 ) & 0xff;
      buf[ i * 2 + 1 ] = measure.Eqc & 0xff;
    }
    else if( RegisterAddress == 40062 )
    {
      buf[ i * 2 ] = ( measure.Eqc >> 24 ) & 0xff;
      buf[ i * 2 + 1 ] = ( measure.Eqc >> 16 ) & 0xff;
    }
    }
    else if( ( RegisterAddress >= REG_ADR_MUL ) && ( RegisterAddress < ( REG_ADR_MUL + 12 ) ) )      //比率校正参数
    {
    Index = RegisterAddress - REG_ADR_MUL;
    buf[ i * 2 ] = gMul[ Index ] >> 8;
    buf[ i * 2 + 1 ] = gMul[ Index ] & 0xff;
    }
    else if( ( RegisterAddress >= REG_ADR_OFF ) && ( RegisterAddress < ( REG_ADR_OFF + 12 ) ) )      //偏移量(虚的,没有用)
    {
    Index = RegisterAddress - REG_ADR_OFF;
    buf[ i * 2 ] = gOff[ Index ] >> 8;
    buf[ i * 2 + 1 ] = gOff[ Index ] & 0xff;
    }
    else if( ( RegisterAddress > 40200 ) && ( RegisterAddress <= 40215 ) )      //读/写寄存器
    {
    if( RegisterAddress == REG_ADR_BAUD )
    {
      buf[ i * 2 ] = 0x00;
      buf[ i * 2 + 1 ] = para.baud;
    }
    else if( RegisterAddress == REG_ADR_ADDRESS )
    {
      buf[ i * 2 ] = 0x00;
      buf[ i * 2 + 1 ] = para.addr;
    }
    else if( RegisterAddress == REG_ADR_CT )
    {
      buf[ i * 2 ] = para.CT >> 8;
      buf[ i * 2 + 1 ] = para.CT & 0xff;
    }
    else if( RegisterAddress == REG_ADR_PT )
    {
      buf[ i * 2 ] = para.PT >> 8;
      buf[ i * 2 + 1 ] = para.PT & 0xff;
    }
    else if( RegisterAddress == REG_ADR_MODE )
    {
      buf[ i * 2 ] = para.sysmode >> 8;
      buf[ i * 2 + 1 ] = para.sysmode & 0xff;
    }
    else if (RegisterAddress==REG_ADR_V_prec)
    {
      buf[i*2] = 0x00;
      buf[i*2+1] = para.V_prec;
    }
    else if (RegisterAddress==REG_ADR_A_prec)
    {
      buf[i*2] = (para.A_prec>>8)&0XFF;
      buf[i*2+1] = para.A_prec&0XFF;
    }
    else 
    {
      buf[ i * 2 ] = 0x00;
      buf[ i * 2 + 1 ] = 0x00;
    }
    }
    else
    {
      buf[ i * 2 ] = 0x00;
      buf[ i * 2 + 1 ] = 0x00;
    }
  }
}

/**********************************************
*功能：写数据
**********************************************/
void SaveRegister(unsigned int StartAddress,unsigned int Numbers,unsigned char *buf)
{
  unsigned int temp=0;
  unsigned int i=0;
  unsigned int RegisterAddress=0;
  WDI_Clr();
  for(i=0;i<Numbers;i++) {
    RegisterAddress=StartAddress+i;
    temp=buf[i*2];
    temp=temp*256;
    temp=temp+buf[i*2+1]; //以 字进行操作
    if( ( RegisterAddress >= REG_ADR_MUL ) && ( RegisterAddress < ( REG_ADR_MUL + 12 ) ) ) {
        if( ( temp <= 1100 ) && ( temp >= 900 ) ){    
          gMul[ RegisterAddress - REG_ADR_MUL ] = temp;
          gRegisterWriteFlagMul = LOGIC_TRUE;
        }
    }
    else if( ( RegisterAddress >= REG_ADR_OFF ) && ( RegisterAddress < ( REG_ADR_OFF + 12 ) ) ) {  //(虚的,没有用)
        if( ( temp <= 2000 ) || ( temp >= 5 ) )    {
          gOff[ RegisterAddress - REG_ADR_OFF ] = temp;
          gRegisterWriteFlagOff = LOGIC_TRUE;
        }
    }
    else if( RegisterAddress == REG_ADR_BAUD ){
        if( temp < 5 ){
          para.baud = temp & 0xff;
          SetBaud( para.baud );
          gRegisterWriteFlagCom = LOGIC_TRUE;
        }
    }
    else if( RegisterAddress == REG_ADR_ADDRESS ){
        if( ( temp != 0 ) || ( temp != 0xff ) ){
          para.addr = temp & 0xff;
          gRegisterWriteFlagCom = LOGIC_TRUE;
        }
    }
    else if( RegisterAddress == REG_ADR_MODE ){
        if( temp < 2 ){
          para.sysmode = temp & 0xff;
          gRegisterWriteFlagCom = LOGIC_TRUE;
        }
    }
    else if( RegisterAddress == REG_ADR_CT ){
        if( temp <= 10000 ){
          para.CT = temp;
          gRegisterWriteFlagCom = LOGIC_TRUE;
        }
    }
    else if( RegisterAddress == REG_ADR_PT ){
        if( temp <= 10000 ){
        para.PT = temp;
        gRegisterWriteFlagCom = LOGIC_TRUE;
        }
      }
      else if( RegisterAddress == REG_ADR_CLR ){
          if(temp == 0){
              DN_Sample[0] = 0;
              DN_Sample[1] = 0; 
              gRegisterWriteFlagDianNeng = LOGIC_TRUE;
          }
      } 
            else if (RegisterAddress == REG_ADR_V_prec)
            {
                if (temp < SET_V_prec_hi)
                {
                    para.V_prec = temp;
                    EEPROM_Write(EEP_ADR_COM_VPREC, *((unsigned char*)(&para.V_prec)));
                    Delay_ms (2);
                }
            }
            else if (RegisterAddress == REG_ADR_A_prec)
            {
                if (temp < SET_A_prec_hi)
                {
                    para.A_prec = temp;
                    EE_Page_Write(EEP_ADR_COM_APREC,2, ((unsigned char*)(&para.A_prec)));//EEPROM_Write(EEP_ADR_COM_APREC, *((unsigned char*)(&para.A_prec)));
                    Delay_ms (2);
                }
            }
      else if( RegisterAddress == REG_RY1_O ){
          if(temp == 0){ClrDO1();}
          else{SetDO1();}
      }
      else if( RegisterAddress == REG_RY2_O ){
          if(temp == 0){ClrDO2();}
          else{SetDO2();}
      }
 }
}

/*********************************************
* 串口 操作处理
*********************************************/
void Com0_Drive()
{
    unsigned int Crc=0;
    unsigned int RegisterStartAddress=0;
    unsigned int RegisterNumbers=0;
    WDI_Clr();
    if(Uart_Read_Flag==1)
    {
        Uart_Read_Flag=0;
        Uart_Length_Flag=0;
        RegisterStartAddress = Uart_Rx_Buf[2]*256+Uart_Rx_Buf[3];
        RegisterNumbers = Uart_Rx_Buf[4]*256+Uart_Rx_Buf[5];
  
        Uart_Tx_Buf[0]=Uart_Rx_Buf[0];
        Uart_Tx_Buf[1]=Uart_Rx_Buf[1];
        Uart_Tx_Buf[2]=RegisterNumbers * 2;
        LoadRegister( RegisterStartAddress, RegisterNumbers, &Uart_Tx_Buf[ 3 ] );
    
        Crc = CRC16( Uart_Tx_Buf, 3 + RegisterNumbers * 2 );
        if(CRC16flag==CRC16HiLo)
        {
            Uart_Tx_Buf[3 + RegisterNumbers * 2 ]=( unsigned char ) (Crc >> 8 );
            Uart_Tx_Buf[4 + RegisterNumbers * 2 ]=( unsigned char )  Crc ;
        }
        else if(CRC16flag==CRC16LoHi)
        {
            Uart_Tx_Buf[4 + RegisterNumbers * 2 ]=( unsigned char ) (Crc >> 8 );
            Uart_Tx_Buf[3 + RegisterNumbers * 2 ]=( unsigned char )  Crc ;
        }
    
        Uart_Tx_Cnt=5 + RegisterNumbers * 2;
        SetCom0Send;
        Delay_ms(20);                                                //add by zhu for net 2010.05.18
        for(unsigned int j=0;j<Uart_Tx_Cnt;j++)
        {
            TXBUF0=Uart_Tx_Buf[j];
            while((UTCTL0&0x01)==0);
        }
        Uart_Rx_Cnt=0;
        Uart_Rx_Length=0;
        Com0Receive_Enable();//串口接收使能
        SetCom0Receive;
    }

    if(Uart_Write_Flag==1)
    {
        Uart_Write_Flag=0;
        Uart_Length_Flag=0;
        RegisterStartAddress = Uart_Rx_Buf[ 2 ]*256+Uart_Rx_Buf[ 3 ];
        RegisterNumbers = Uart_Rx_Buf[ 4 ]*256+Uart_Rx_Buf[ 5 ];

        if(Uart_Rx_Buf[0]!=0xff)
        {
            Uart_Tx_Buf[0]=Uart_Rx_Buf[0];
            Uart_Tx_Buf[1]=0x10;
            Uart_Tx_Buf[2]=Uart_Rx_Buf[2];
            Uart_Tx_Buf[3]=Uart_Rx_Buf[3];
            Uart_Tx_Buf[4]=Uart_Rx_Buf[4];
            Uart_Tx_Buf[5]=Uart_Rx_Buf[5];
            Crc = CRC16( Uart_Tx_Buf, 6 );
            if(CRC16flag==CRC16HiLo)
            {
                Uart_Tx_Buf[6]=( unsigned char ) ( Crc >> 8 );
                Uart_Tx_Buf[7]=( unsigned char ) ( Crc );
            }
            else if(CRC16flag==CRC16LoHi)
            {
                Uart_Tx_Buf[7]=( unsigned char ) ( Crc >> 8 );
                Uart_Tx_Buf[6]=( unsigned char ) ( Crc );
            }
      
            Uart_Tx_Cnt=8;
            SetCom0Send;
            Delay_ms(20);                                                //add by zhu for net 2010.05.18
            for(unsigned int j=0;j<Uart_Tx_Cnt;j++)
            {
                TXBUF0=Uart_Tx_Buf[j];
                while((UTCTL0&0x01)==0);
            }
        }
        SaveRegister( RegisterStartAddress, RegisterNumbers, &Uart_Rx_Buf[ 7 ] );
        RegisterWriteDriver();
        Uart_Rx_Cnt=0;
        Uart_Rx_Length=0;
        RegisterWriteDriver();
        Com0Receive_Enable();//串口接收使能
        SetCom0Receive;
    }

    if(Com0_Pack_Error==1)
    {//串口0接收错误处理
        Com0_Pack_Error=0;//错误标志清零
        Uart_Rx_Cnt=0;
        Uart_Tx_Cnt=0;
        Uart_Rx_Length=0;
        Uart_Length_Flag=0;
        Uart_Read_Flag=0;
        Uart_Write_Flag=0;
        for(unsigned char i=0;i<100;i++)
        {
          Uart_Rx_Buf[i]=0;
          Uart_Tx_Buf[i]=0;
        }
        Com0Receive_Enable();//串口接收使能
        SetCom0Receive;  
    }
    Com0Receive_Enable();
    SetCom0Receive; 
}
//unsigned int clk1sFlg=0;

void main( void )
{
    Osc_Init();
    LED_RUN_Init();
    WDI_Init();
    Adc12Init();
    ADC12Channel();
    Adc12Open();
    Adc12Start();
    TimeB_Init();
    TimeB_AddMode();
    FCompareUA_Init();
    FCompareUA_Start(); 

    unsigned char i = 0;
    for(i = 0; i < 5; i++)
    {
        Delay_ms(15);
    }
    LCD_Init();
    Delay_ms(5);    
    LCD_All();    
    Delay_ms(5);
    LCD_Read(dispBuf,40);
    LCD_All();    
    for(i = 0; i < 30; i++)
    {
        Delay_ms(20);
    }
    LCD_Clr();    
    
    Button_Init();
    DIO_Init();
    i2c_init();
    WDI_Clr();
    i2c_init();
    ReadRegister();
     
    Uart_Init(para.baud);
    SetCom0Receive;
    
    P5OUT&=~LCD_LED;
    _BIS_SR(GIE);
    //lin 080912
    WDT_timer_init();
    //lin 080917
    analog_sw_ctrl_init();

    while(1)
    {
        if (clkCnt > 31)
        {
            clkCnt = 0;
            clk500msFlg = 1;
        }

        if (clkFlg)
        {
            clkFlg = 0;
            WDI_Clr();

            if (keyTmp)
            {                          
                if (++keyFlg > 6)
                {
                    if ((!(P2IN&BIT0))&&(!(P2IN&BIT2)))
                    {        // 按BIT0与BIT2从密码及设置菜单退出到ParaMenu;
                        keyValue = KEY_EXIT;
                    }
                    else if ((!(P2IN&BIT1))&&(!(P2IN&BIT2)))
                    {        // 按BIT1与BIT2进入显示收内容页面;
                        keyValue = KEY_PRINT;
                    }
                    else
                    {
                        keyValue = keyTmp;
                    }
                    keyTmp = 0; keyFlg = 0;keyExitCnt = 0;                    
                }
            }
            Sample_Process();
        }

        if (clk500msFlg == 1)
        {
            clk500msFlg = 0;
            //lin 080918
            if (menuStatus != PARA)
            {
                if (++keyExitCnt > 120)
                {    //60秒没按键退出SET
                    IntoParaMenu();
                    keyExitCnt = 0;
                }
            }
            Frequency_Process();
               DIO_Chk();

               MenuProcess();
            P3OUT ^= LED_RUN;
            //DO_Out();
            #ifdef DI_CHK_KEEP
                DI_delayDeal();
            #endif
        }

        /*if(clk1sFlg > 60)
        {
            clk1sFlg=0;
            MenuProcess();
        }*/
    }
}

void DO_Out(void)
{
  if((measure.Uva > para.OverVol)||(measure.Uvb > para.OverVol)||(measure.Uvc > para.OverVol)){
    SetDO1();
  }
  else if((measure.Uva < (para.OverVol - 6))||(measure.Uvb < (para.OverVol - 6))||(measure.Uvc < (para.OverVol - 6))){
     ClrDO1();
  }
  if((measure.Ia*100 > para.LowVol)||(measure.Ib*100 > para.LowVol)||(measure.Ic*100 > para.LowVol)){
    SetDO2();
  }
  else if((measure.Ia < (para.LowVol+30))||(measure.Ib < (para.LowVol+30))||(measure.Ic < (para.LowVol+30))){
    ClrDO2();
  }
}
unsigned char AdcStarflag=0;
#pragma vector=TIMERB0_VECTOR
__interrupt void Timer_B (void)
{                                                      
  Adc12Close();//关闭转换
  smpl.AD_value[0][smpl.cnt]=ADC12MEM0;
  smpl.AD_value[1][smpl.cnt]=ADC12MEM2;
  smpl.AD_value[2][smpl.cnt]=ADC12MEM4;
  smpl.AD_value[3][smpl.cnt]=ADC12MEM1;
  smpl.AD_value[4][smpl.cnt]=ADC12MEM3;
  smpl.AD_value[5][smpl.cnt]=ADC12MEM5;
  smpl.cnt++;
  if(smpl.cnt>=SMPL_MAX)
  {
    smpl.cnt=0;
    Adc12Close();
    TimeB_Stop();
    TimeB_Clear();
    smpl.flg=1;
  }
  else
  {
    TBCCR0=ADC_TBCCR0;
    Adc12Open();
    Adc12Start();

  }
}
#pragma vector=TIMERA0_VECTOR
__interrupt void TimerA0_ISR()
{

  Frequency_Sample_Value[Frequency_Count]=TACCR0;
  Frequency_Count++;
  if(Frequency_Count>10)
  {
    FCompareUA_Stop();
    Frequency_Flag=1; 
  }
  else
  {
    Frequency_Flag=0;
  }
}
#pragma vector=TIMERA1_VECTOR
__interrupt void TimerA1_ISR()
{
    asm("NOP");
}
#pragma vector=PORT2_VECTOR
__interrupt void Port2_ISR()
{
    if(P2IFG&BIT0){    //按Button1(SET)
        P2IFG&=~BIT0;
        keyTmp = KEY_MODE;    
        return;
    }    
    
    if(P2IFG&BIT1){    //按Button2(+)
        P2IFG&=~BIT1;
        keyTmp = KEY_UP;    
        return;
    }
    
    if(P2IFG&BIT2){    //按Button3(-)
        P2IFG&=~BIT2;    
        keyTmp = KEY_DOWN;
        return;
    }
}

#pragma vector= WDT_VECTOR
__interrupt void WDT_ISR()
{
    clkFlg = 1;
    clkCnt++;
    clk_en_cnt++;
    clk1sFlg++;
    if (RcvTimeoutCount<25) {
        RcvTimeoutCount++;
    }
}

#pragma vector=UART0RX_VECTOR
__interrupt void Uart0RX_ISR()
{
    Uart_Rx_Buf[Uart_Rx_Cnt]= RXBUF0;
  
    //lin 080904 加下面一语句
    _BIS_SR(GIE);

    if (RcvTimeoutCount >= 25) {
        Uart_Rx_Cnt = 0;
    }
    RcvTimeoutCount = 0;
/*
    switch(Uart_Rx_Buf[0])
    {
        case para.addr: // 需为常量
        case 0x00:
        case 0xFF:
            {
                Uart_Rx_Cnt++;
                switch(Uart_Rx_Cnt)
                {
                    case 2:  // 功能码判断
                        {
                            if ((Uart_Rx_Buf[1]!=0x03)&&(Uart_Rx_Buf[1]!=0x10))
                            {
                                Uart_Rx_Cnt=0;
                                Com0_Pack_Error=1;
                            }
                        }
                        break;
                    case 6:  // 判断长度
                        {
                            if ((Uart_Rx_Buf[4]*256+Uart_Rx_Buf[5])>47) // (100-5)/2
                            {
                                Uart_Rx_Cnt=0;
                                Com0_Pack_Error=1;
                            }
                        }
                        break;
                    default :
                        break;
                }

                if (Uart_Rx_Buf[1]==0x03) // 读取功能码
                {
                    if (Uart_Rx_Cnt>7)
                    {
                        unsigned int CRC;
                        unsigned int recvCRC1, recvCRC2;
                        Com0Receive_Disable();//禁止串口接收

                        CRC=CRC16(Uart_Rx_Buf,6);
                        recvCRC1=Uart_Rx_Buf[6]*256+Uart_Rx_Buf[7];
                        recvCRC2=Uart_Rx_Buf[7]*256+Uart_Rx_Buf[6];
                        if(CRC==recvCRC1)
                        {
                            CRC16flag = CRC16HiLo;
                            Uart_Read_Flag=1;
                        }
                        else if(CRC==recvCRC2)
                        {
                            CRC16flag = CRC16LoHi;
                            Uart_Read_Flag=1;
                        }
                        else
                        {
                            Uart_Rx_Cnt=0;
                            Com0_Pack_Error=1;   
                        }
                    }
                }
                else if (Uart_Rx_Buf[1]==0x10) // 写入功能码
                {
                    if (Uart_Rx_Cnt>6)
                    {
                        Uart_Rx_Length = Uart_Rx_Buf[6];
                        if (Uart_Rx_Length != 2*(Uart_Rx_Buf[4]*256+Uart_Rx_Buf[5]))
                        {
                            Uart_Rx_Cnt=0;
                            Com0_Pack_Error=1;  
                        }
                        else
                        {
                            if(Uart_Rx_Cnt>Uart_Rx_Length+8)
                            {
                                Com0Receive_Disable();//禁止串口接收
                                unsigned int CRC;
                                unsigned int recvCRC1, recvCRC2;
                                CRC=CRC16(Uart_Rx_Buf,Uart_Rx_Length+7);
                                recvCRC1=Uart_Rx_Buf[Uart_Rx_Length+7]*256+Uart_Rx_Buf[Uart_Rx_Length+8];
                                recvCRC2=Uart_Rx_Buf[Uart_Rx_Length+8]*256+Uart_Rx_Buf[Uart_Rx_Length+7];
                                if(CRC==recvCRC1)
                                {
                                    CRC16flag = CRC16HiLo;
                                    Uart_Write_Flag=1;
                                }
                                else if(CRC==recvCRC2)
                                {
                                    CRC16flag = CRC16LoHi;
                                    Uart_Write_Flag=1;
                                }
                                else
                                {
                                    Uart_Rx_Cnt=0;
                                    Com0_Pack_Error=1;   
                                }
                            }
                        }
                    }
                }
            }
            break;

        default :
            {
                Uart_Rx_Cnt=0;
                Com0_Pack_Error=1;        //错误标志置1
            }
            break;
    }
*/

    if((Uart_Rx_Buf[0]!=para.addr)&&(Uart_Rx_Buf[0]!=0x00)&&(Uart_Rx_Buf[0]!=0xff))
    {
        Uart_Rx_Cnt=0;
        Com0_Pack_Error=1;        //错误标志置1
    }
    else if((Uart_Rx_Cnt==1)&&(Uart_Rx_Buf[1] !=0x03)&&(Uart_Rx_Buf[1] !=0x10))
    { // 干扰字节 如果与地址、功能码相同~~ 处理方式:往前移一个字节  03  03 03 00 00 00 01 85 E8
        if(Uart_Rx_Buf[1]==para.addr)
        {
            Uart_Rx_Buf[0]=Uart_Rx_Buf[1];
            Uart_Rx_Cnt=1;
        }
        else
        {
            Uart_Rx_Cnt=0;
        }
    }
    else
    {
        Uart_Rx_Cnt++;
        if(Uart_Rx_Cnt>1)
        {
            if(Uart_Rx_Buf[1]==0x03)//读数据命令
            {
                if (Uart_Length_Flag==0)
                {
                    if (Uart_Rx_Cnt>7)
                    {
                        Uart_Rx_Length=Uart_Rx_Buf[4]*256+Uart_Rx_Buf[5];
                        if ((Uart_Rx_Length>47)||(Uart_Rx_Length==0)) // (100-5)/2
                        {
                            Uart_Rx_Cnt=0;
                            Com0_Pack_Error=1;   
                        }
                        else
                        {
                            Uart_Length_Flag = 1;
                        }
                    }
                }
                if (Uart_Length_Flag==1)
                {
                    unsigned int CRC;
                    unsigned int recvCRC1, recvCRC2;
                    Com0Receive_Disable();//禁止串口接收

                    CRC=CRC16(Uart_Rx_Buf,6);
                    recvCRC1=Uart_Rx_Buf[6]*256+Uart_Rx_Buf[7];
                    recvCRC2=Uart_Rx_Buf[7]*256+Uart_Rx_Buf[6];
                    if(CRC==recvCRC1)
                    {
                        CRC16flag = CRC16HiLo;
                        Uart_Read_Flag=1;
                    }
                    else if(CRC==recvCRC2)
                    {
                        CRC16flag = CRC16LoHi;
                        Uart_Read_Flag=1;
                    }
                    else
                    {
                        Uart_Rx_Cnt=0;
                        Com0_Pack_Error=1;   
                    }
                }
            }
            else if(Uart_Rx_Buf[1]==0x10)//写数据命令
            {
                if(Uart_Length_Flag==0)
                {
                    if(Uart_Rx_Cnt>6)
                    {
                        Uart_Rx_Length=Uart_Rx_Buf[6];
                        if((Uart_Rx_Length>91)||(Uart_Rx_Length==0)) // 100-9
                        {
                            Uart_Rx_Cnt=0;
                            Com0_Pack_Error=1;   
                        }
                        else
                        {
                            Uart_Length_Flag=1;
                        }
                    }
                }
                if(Uart_Length_Flag==1)
                {
                    if(Uart_Rx_Cnt>Uart_Rx_Length+8)
                    {
                        Com0Receive_Disable();//禁止串口接收
                        unsigned int CRC;
                        unsigned int recvCRC1, recvCRC2;
                        CRC=CRC16(Uart_Rx_Buf,Uart_Rx_Length+7);
                        recvCRC1=Uart_Rx_Buf[Uart_Rx_Length+7]*256+Uart_Rx_Buf[Uart_Rx_Length+8];
                        recvCRC2=Uart_Rx_Buf[Uart_Rx_Length+8]*256+Uart_Rx_Buf[Uart_Rx_Length+7];
                        if(CRC==recvCRC1)
                        {
                            CRC16flag = CRC16HiLo;
                            Uart_Write_Flag=1;
                        }
                        else if(CRC==recvCRC2)
                        {
                            CRC16flag = CRC16LoHi;
                            Uart_Write_Flag=1;
                        }
                        else
                        {
                            Uart_Rx_Cnt=0;
                            Com0_Pack_Error=1;   
                        }
                    }
                }
            }
            else
            {
                Uart_Rx_Cnt=0;
                Uart_Length_Flag=0;
                Com0_Pack_Error=1; 
                Com0Receive_Disable();
            }
        }
    }

    Com0_Drive();
    WDI_Clr();//硬件狗清零  
}


