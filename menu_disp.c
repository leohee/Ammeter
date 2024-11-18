
#include <msp430x14x.h>
#include "display_maps.h"
#include "menu_disp.h"
#include "glb_var.h"
#include "main.h"
#include "iic.h"
#include "osc.h"
#include <string.h>

unsigned char Unit_ = 0;         //单位

extern const unsigned char charTbl[];
extern const unsigned char digitalRamMaps[];
extern unsigned char Uart_Rx_Buf[100];
extern void SetBaud(unsigned char Baud);


const unsigned char baudTbl[][5] = {
    " 1200"," 2400"," 4800"," 9600","19200"
};

const unsigned char clrTbl[][3] = {
    " NO","YES"
};

void dot1DispDeal(unsigned char dot)
{
    if(dot == 0){disp_dot1();}
    else if(dot == 1){disp_dot3();}
    else if(dot == 2){disp_dot5();}
    else{disp_dot7();}
}
void dot2DispDeal(unsigned char dot)
{
    if(dot == 0){disp_dot2();}
    else if(dot == 1){disp_dot4();}
    else if(dot == 2){disp_dot6();}
    else{disp_dot8();}
}

unsigned int digitDispDeal(float val,unsigned char dot)
{
    if(val < 100){
        val *= 100;dot1DispDeal(dot);
        Unit_ = 0;
    }else if(val < 1000){
        val *= 10; dot2DispDeal(dot);
        Unit_ = 0;
    }else if(val < 10000){
        Unit_ = 0;
    }else if(val < 100000){
        val /= 10; dot1DispDeal(dot);
        symbol_K();Unit_ = 1;
    }else if(val < 1000000){
        val /= 100; dot2DispDeal(dot);
        symbol_K(); Unit_ = 1;
    }else if(val < 10000000){
        val /= 1000;
        symbol_K();Unit_ = 1;
    }else if(val < 100000000){
        val /= 10000;dot1DispDeal(dot);
        symbol_M(); Unit_ = 2;
    }else if(val < 1000000000){
        val /= 100000; dot2DispDeal(dot);
        symbol_M(); Unit_ = 2;
    }else if(val < 10000000000){
        val /= 1000000; 
        symbol_M();Unit_ = 2;
    }else if(val < 100000000000){
        val /= 10000000; dot1DispDeal(dot);
        symbol_M();symbol_K();
        Unit_ = 3;
    }else if(val < 1000000000000){
        val /= 100000000; dot2DispDeal(dot);
        symbol_M();symbol_K();
        Unit_ = 3;
    }else {
        val /= 1000000000; 
        symbol_M();symbol_K();
        Unit_ = 3;
    }
    return((unsigned int)val);
}
unsigned int digitDispDeal_(float val,unsigned char dot)
{
    if(Unit_ == 0){
        if(val < 100){
            val *= 100;dot1DispDeal(dot);
        }else if(val < 1000){
            val *= 10; dot2DispDeal(dot);
        }
    }
    else if(Unit_ == 1){
        if(val < 100000){
            val /= 10; dot1DispDeal(dot);
        }else if(val < 1000000){
            val /= 100; dot2DispDeal(dot);
        }else if(val < 10000000){
            val /= 1000;
        }
    } 
    else if(Unit_ == 2){
        if(val < 100000000){
            val /= 10000;dot1DispDeal(dot);
        }else if(val < 1000000000){
            val /= 100000; dot2DispDeal(dot);
        }else if(val < 10000000000){
            val /= 1000000; 
        }
    }
    else if(Unit_ == 3){
        if(val < 100000000000){
            val /= 10000000;dot1DispDeal(dot);
        }else if(val < 1000000000000){
            val /= 100000000; dot2DispDeal(dot);
        }else if(val < 10000000000000){
            val /= 1000000000; 
        }
    }
   
    return((unsigned int)val);
}

unsigned long LdigitDispDeal(float val,unsigned char dot)
{
    if(val < 1000000){
        val *= 100;dot1DispDeal(dot);
        Unit_ = 0;
    }else if(val < 10000000){
        val *= 10; dot2DispDeal(dot);
        Unit_ = 0;
    }else if(val < 100000000){
        Unit_ = 0;
    }else if(val < 1000000000){
        val /= 10; dot1DispDeal(dot);
        symbol_K();Unit_ = 1;
    }else if(val < 10000000000){
        val /= 100; dot2DispDeal(dot);
        symbol_K(); Unit_ = 1;
    }else if(val < 100000000000){
        val /= 1000;
        symbol_K();Unit_ = 1;
    }else if(val < 1000000000000){
        val /= 10000;dot1DispDeal(dot);
        symbol_M(); Unit_ = 2;
    }
    return((unsigned long)val);
}
unsigned long LdigitDispDeal_(float val,unsigned char dot)
{
    if(Unit_ == 0){
        if(val < 1000000){
            val *= 100;dot1DispDeal(dot);
        }else if(val < 10000000){
            val *= 10; dot2DispDeal(dot);
        }
    }
    else if(Unit_ == 1){
        if(val < 1000000000){
            val /= 10; dot1DispDeal(dot);
        }else if(val < 10000000000){
            val /= 100; dot2DispDeal(dot);
        }else if(val < 100000000000){
            val /= 1000;
        }
    } 
    else if(Unit_ == 2){
        if(val < 1000000000000){
            val /= 10000;dot1DispDeal(dot);
        }
     }
    return((unsigned long)val);
   
}

void Display_URms(float UA,float UB,float UC,float uabc)
{
    unsigned char buf[16]={0};
    
    clr_disp_buf();
    
    symbol_V();
    symbol_L1();
    symbol_L2();
    symbol_L3();
    symbol_Avg();
    symbol_T1();symbol_T2();
    symbol_T3();symbol_T4();

    if((UA >= UB)&&(UA >= UC)){
        uintToBCD(digitDispDeal(UA,0),buf);        
        uintToBCD(digitDispDeal_(UB,1),buf+4);        
        uintToBCD(digitDispDeal_(UC,2),buf+8);
    }
    else if((UB >= UA)&&(UB >= UC)){
        uintToBCD(digitDispDeal(UB,1),buf+4);    
        uintToBCD(digitDispDeal_(UA,0),buf);            
        uintToBCD(digitDispDeal_(UC,2),buf+8);
    }
    else{        
        uintToBCD(digitDispDeal(UC,2),buf+8);
        uintToBCD(digitDispDeal_(UA,0),buf);    
        uintToBCD(digitDispDeal_(UB,1),buf+4);    
    }
    uintToBCD(digitDispDeal_(uabc,3),buf+12);
    
    buf_string(0,16, buf);
}

//线电压显示
void Display_URmsv(float UAB,float UBC,float UCA,float uabc)
{
    unsigned char buf[16]={0};
    clr_disp_buf();          //清全部显示缓存;
    
    symbol_V();
    symbol_L12();
    symbol_L23();
    symbol_L31();
    symbol_Avg();
    symbol_T1();symbol_T2();
    symbol_T3();symbol_T4();

    if((UAB >= UBC)&&(UAB >= UCA)){
        uintToBCD(digitDispDeal(UAB,0),buf);    
        uintToBCD(digitDispDeal_(UBC,1),buf+4);
        uintToBCD(digitDispDeal_(UCA,2),buf+8);
    }
    else if((UBC >= UAB)&&(UBC >= UCA)){
        uintToBCD(digitDispDeal(UBC,1),buf+4);
        uintToBCD(digitDispDeal_(UAB,0),buf);    
        uintToBCD(digitDispDeal_(UCA,2),buf+8);
    }
    else{
        uintToBCD(digitDispDeal(UCA,2),buf+8);
        uintToBCD(digitDispDeal_(UAB,0),buf);    
        uintToBCD(digitDispDeal_(UBC,1),buf+4);
    }
    uintToBCD(digitDispDeal_(uabc,3),buf+12);
    
    buf_string(0,16, buf);    
}

//显示电流频率
void Display_IRms(float IA,float IB,float IC,float IN)//以A为单位
{
    unsigned char buf[16]={0};
    clr_disp_buf();
    symbol_A();
    symbol_L1();
    symbol_L2();
    symbol_L3();
    symbol_N();
    symbol_T1();symbol_T2();
    symbol_T3();symbol_T4();

    if((IA >= IB)&&(IA >= IC)){
        uintToBCD(digitDispDeal(IA,0),buf);        
        uintToBCD(digitDispDeal_(IB,1),buf+4);        
        uintToBCD(digitDispDeal_(IC,2),buf+8);
    }
    else if((IB >= IA)&&(IB >= IC)){
        uintToBCD(digitDispDeal(IB,1),buf+4);
        uintToBCD(digitDispDeal_(IA,0),buf);
        uintToBCD(digitDispDeal_(IC,2),buf+8);
    }
     else{
        uintToBCD(digitDispDeal(IC,2),buf+8);
        uintToBCD(digitDispDeal_(IA,0),buf);
        uintToBCD(digitDispDeal_(IB,1),buf+4);
    }
    uintToBCD(digitDispDeal_(IN,3),buf+12);
    
    buf_string(0,16, buf);   
}

/*********************************************
* 显示功率因素
**********************************************/
void Display_Pf(unsigned int PFA,unsigned int PFB,unsigned int PFC,unsigned int pft)
{
    unsigned char buf[8]={0};
    clr_disp_buf();
    symbol_PF();
    symbol_L1();
    symbol_L2();
    symbol_L3();
    symbol_E(); 
    symbol_T1(); symbol_T2();
    symbol_T3(); symbol_T4();
    
    uintToBCD(PFA,buf);
    buf_string(1,3, buf+1);
    disp_dot1();    
    
    uintToBCD(PFB,buf);
    buf_string(5,3, buf+1);
    disp_dot3();    
    
    uintToBCD(PFC,buf);
    buf_string(9,3, buf+1);
    disp_dot5();
    
    uintToBCD(pft,buf);
    buf_string(13,3, buf+1);
    disp_dot7();
}
/*****************************************
* 功能:显示视在功率
*****************************************/
void Display_Power_S(float SA,float SB,float SC,float St)
{
    unsigned char buf[16]={0};
    clr_disp_buf();
    symbol_VA();
    symbol_L1();
    symbol_L2();
    symbol_L3();
    symbol_E();
    symbol_T1();symbol_T2();
    symbol_T3();symbol_T4();    
    uintToBCD(digitDispDeal(St,3),buf+12);
    uintToBCD(digitDispDeal_(SA,0),buf);        
    uintToBCD(digitDispDeal_(SB,1),buf+4);        
    uintToBCD(digitDispDeal_(SC,2),buf+8);
    
    buf_string(0,16, buf);   

}

/****************************************
*功能:显示有功功率
****************************************/
void Display_Power_P(float PA,float PB,float PC,float Pt)
{
    unsigned char buf[16]={0};
    clr_disp_buf();

    symbol_W();
    symbol_L1();
    symbol_L2();
    symbol_L3();
    symbol_E();
    symbol_T1();symbol_T2();
    symbol_T3();symbol_T4();
   
    uintToBCD(digitDispDeal(Pt,3),buf+12);
    uintToBCD(digitDispDeal_(PA,0),buf);        
    uintToBCD(digitDispDeal_(PB,1),buf+4);        
    uintToBCD(digitDispDeal_(PC,2),buf+8);
    
    buf_string(0,16, buf);   
}



/****************************************
*功能:显示无功功率
****************************************/
void Display_Power_Q(float QA,float QB,float QC,float Qt)
{
    unsigned char buf[16]={0};
    clr_disp_buf();

    symbol_var();
    symbol_L1();
    symbol_L2();
    symbol_L3(); 
    symbol_E();
    symbol_T1();symbol_T2();
    symbol_T3();symbol_T4();
   
    uintToBCD(digitDispDeal(Qt,3),buf+12);
    uintToBCD(digitDispDeal_(QA,0),buf);        
    uintToBCD(digitDispDeal_(QB,1),buf+4);    
    uintToBCD(digitDispDeal_(QC,2),buf+8);    
    buf_string(0,16, buf);   
}
void Display_Wvar(float Pt,float Qt)//以A为单位
{
    unsigned char buf[16]={0};
    clr_disp_buf();
    symbol_var();
    symbol_W();
   // symbol_K();
    symbol_E();
    symbol_T2(); symbol_T4();
    if(Pt>Qt){
       ulongToBCD(LdigitDispDeal(Pt,1),buf+8);
       ulongToBCD(LdigitDispDeal_(Qt,3),buf);  
    }
    else{
       ulongToBCD(LdigitDispDeal(Qt,3),buf);
       ulongToBCD(LdigitDispDeal_(Pt,1),buf+8);
    }
    //ulongToBCD(Pt/10,buf+8);
   // ulongToBCD(Qt/10,buf);   
    //disp_dot3();
    //disp_dot7();
    buf_string(0,16, buf);    
}
void Display_HzPf(float PFt,float Frq)//以A为单位
{
    unsigned char i,buf[16]={0};
    clr_disp_buf();
    symbol_E();
    symbol_T2(); symbol_T4();
    for(i = 0; i < 16; i++) buf[i] = ' ';
        buf[1] = 'H';
        buf[2] = 'Z';
        buf[9] = 'P';
        buf[10] = 'F';
    uintToBCD(Frq,buf+4);
    uintToBCD(PFt,buf+12);
        buf[12] = ' ';
    disp_dot3();
    disp_dot7();
        buf_string(0,16, buf);   
}

/**************************************
* 显示电能
**************************************/
void Display_Epqt(unsigned long KWh,unsigned long Kvarh)
{
    unsigned char buf[16]={0};
    clr_disp_buf();
    symbol_T2();symbol_T4();
    symbol_KWh();    
    symbol_Kvarh();
    
    ulongToBCD(KWh,buf+8);
    ulongToBCD(Kvarh,buf);
    disp_dot4();
    disp_dot8();
    buf_string(0,16, buf);
}    
// ----------------------------------------
 //addr数字量输入
 //----------------------------------------
 void  Display_ADDR(unsigned int addr)
 {
     unsigned char buf[8]={0};
    clr_disp_buf();
     symbol_Addr();
    
    uintToBCD(addr,buf);
    buf_string(5,3, buf+1);
 }
// ----------------------------------------
 //PT
 //----------------------------------------
void Display_PT(unsigned int PT)
{
     unsigned char buf[8];
    clr_disp_buf();
    symbol_PT();    
        
    ulongToBCD(PT,buf);
    buf_string(3,5, buf+3);
}
// ----------------------------------------
//CT
//----------------------------------------
void Display_CT(unsigned int CT)
{
    unsigned char buf[8]={0};
    clr_disp_buf();    
    symbol_CT();
            
    ulongToBCD(CT,buf);
    buf_string(3,5, buf+3);
}


/**************************************
*功能: 显示波特率
***************************************/
void Display_Baud(unsigned char baud)
{
    unsigned char buf[5]={0},i=0;
    //clr_disp_buf();
    symbol_Baud();
    for(i = 0; i < 5; i++) buf[i] = baudTbl[baud][i];
    buf_string(11,5, buf);        
}

// ----------------------------------------
 //DI数字量输入
 //----------------------------------------
 void  Display_DIO(void)
 {    
 static int DI_O_COUNTER=0;
 static char DI_status_buf[4]={0};
 static char DO_status_buf[2]={0};
 if(DI_O_COUNTER++>=22)
     DI_O_COUNTER=0;
 if((DI_O_COUNTER<10)||(DI_status[0] !=DI_status_buf[0])||(DI_status[1] !=DI_status_buf[1])\
     ||(DI_status[2] !=DI_status_buf[2])||(DI_status[3] !=DI_status_buf[3]))
     {
     if(DI_O_COUNTER>=10)
         {
         DI_O_COUNTER=0;
         DI_status_buf[0] =DI_status[0];
        DI_status_buf[1] =DI_status[1];
        DI_status_buf[2] =DI_status[2];
        DI_status_buf[3] =DI_status[3];
         }
    symbol_DI();
//    symbol_DO();
    
    if(DI_status[0]) {symbol_DI1on();}
    else {symbol_DI1off();}
    if(DI_status[1]) {symbol_DI2on();}
    else {symbol_DI2off();}
    if(DI_status[2]) {symbol_DO1on();}
    else {symbol_DO1off();}
    if(DI_status[3]) {symbol_DO2on();}
    else {symbol_DO2off();}
     }
 else if(((DI_O_COUNTER>11)&&(DI_O_COUNTER<21))||(DO_status[0] !=DO_status_buf[0])||(DO_status[1] !=DO_status_buf[1]))
     {
     if((DO_status[0] !=DO_status_buf[0])||(DO_status[1] !=DO_status_buf[1]))
         {
         DI_O_COUNTER=12;
         DO_status_buf[0]=DO_status[0];
        DO_status_buf[1]=DO_status[1];
         }
    symbol_DO();
    if(DO_status[0]) {symbol_DO1on();}
    else {symbol_DO1off();}
    if(DO_status[1]) {symbol_DO2on();}
    else {symbol_DO2off();}
     }
 /*
    symbol_DI();
    symbol_DO();
    
    if(DI_status[0]) {symbol_DI1on();}
    else {symbol_DI1off();}
    if(DI_status[1]) {symbol_DI2on();}
    else {symbol_DI2off();}
    if(DI_status[2]) {symbol_DI3on();}
    else {symbol_DI3off();}
    if(DO_status[0]) {symbol_DO1on();}
    else {symbol_DO1off();}
    if(DO_status[1]) {symbol_DO2on();}
    else {symbol_DO2off();}
    */
 }
void Display_LOV(void)
{
    unsigned char buf[16]={0};    
    clr_disp_buf();      //清全部显示缓存;
    uintToBCD(para.OverVol,buf+4);
    uintToBCD(para.LowVol,buf+12);
    buf[0] = 'O'; buf[1] = 'V'; buf[2] = ' ';  buf[3] = 'U'; buf[4] = ' '; 
    buf[8] = 'O'; buf[9] = 'V'; buf[10] = ' '; buf[11] = 'A'; buf[12] = ' ';
    buf_string(0,16, buf);    
        disp_dot7();    
}
void Display_PT_adjust(void)
{
    unsigned char buf[16]={0};    
    clr_disp_buf();      //清全部显示缓存;
    buf[0] = 'P';  buf[1] = ' '; buf[2] = ' '; buf[3] = 'A';
    buf[4] = 'J';     buf[5] = 'U';    buf[6] = 'S';  buf[7] = 'T'; 
    buf[8] = 'O';  buf[9] = 'K'; 
    buf_string(0,10, buf);        
    buf_string(0,8, buf);    
}
void Display_u_i_adjust(void)
{
    unsigned char buf[16]={0};    
    clr_disp_buf();      //清全部显示缓存;
    buf[0] = 'U';  buf[1] = 'I'; buf[2] = ' '; buf[3] = 'A';
    buf[4] = 'J';     buf[5] = 'U';    buf[6] = 'S';  buf[7] = 'T'; 
    buf[8] = 'O';  buf[9] = 'K'; 
    buf_string(0,10, buf);    
}
void Display_Sysmode(unsigned char dat)
{
    unsigned char buf[16]={0};    
    unsigned char i;
    for(i=0;i<16;i++)
        buf[i] = ' ';
    clr_disp_buf();      //清全部显示缓存;
    buf[0] = 'S'; buf[1] = 'Y'; buf[2] = 'S';  buf[3] = ' '; buf[4] = 'T'; 
    buf[5] = 'Y'; buf[6] = 'P'; buf[7] = 'E';
    if(dat==0)
        {
        buf[13] = '3'; buf[14] = 'P'; buf[15] = 'N';
        disp_dot8();
        }
    else if(dat==1)
        {
        buf[13] = '3'; buf[14] = 'P'; buf[15] = '1';
        disp_dot8();
        }
    else if(dat==2)
        {
        buf[13] = ' '; buf[14] = '2'; buf[15] = 'P';
        }
    else if(dat==3)
        {
        buf[13] = ' '; buf[14] = '1'; buf[15] = 'P';
        }
    else
        {
        buf[13] = '3'; buf[14] = 'P'; buf[15] = 'N';
        }        
    buf_string(0,16, buf);    
}
void ParaMenuDisplay(void)
{
    unsigned char buf[4]={0},page=0;
    if(para.sysmode==1)
    {
        if(menuPara==1)
        {
            if(keyValue == KEY_DOWN)
                menuPara++;
            else
                menuPara--;
        }
        if(menuPara>0)
            page=menuPara-1;
        else
            page=menuPara;
    }
    else if(para.sysmode==3)
    {
        if(menuPara==0)
        {
            if(keyValue == KEY_UP)
                menuPara=PARA_MENU_NUM-1;
            else
                menuPara=1;
        }
        if(menuPara>0)
            page=menuPara-1;
    }
    else
        page=menuPara;

    switch (menuPara)
    {
        case 0:
            Display_URmsv(measure.Uva*para.PT,measure.Uvb*para.PT,measure.Uvc*para.PT,measure.Uvt*para.PT);    //显示线电压
            break;
        case 1:
            Display_URms(measure.Ua*para.PT,measure.Ub*para.PT,measure.Uc*para.PT,measure.Ut*para.PT);    //显示相电压
            break;
        case 2:
            Display_IRms(measure.Ia*para.CT,measure.Ib*para.CT,measure.Ic*para.CT,measure.In*para.CT);                    //
            break;
        //----------------------------------------
        case 3:
            Display_Pf(measure.Pfa*100,measure.Pfb*100,measure.Pfc*100,measure.Pft*100);                 //显示功率因素
            break;
        case 4:
            Display_HzPf(measure.Pft*100,measure.Frq*100);
            break; 
        //------------------------------------------------    
        case 5:
            Display_Power_S(measure.Sa*para.CT*para.PT*1000, measure.Sb*para.CT*para.PT*1000,
            measure.Sc*para.CT*para.PT*1000,measure.St*para.CT*para.PT*1000);
            break;
        case 6:
            Display_Power_P(measure.Pa*para.CT*para.PT*1000,measure.Pb*para.CT*para.PT*1000,
            measure.Pc*para.CT*para.PT*1000,measure.Pt*para.CT*para.PT*1000);
            break;
        case 7:
            Display_Power_Q(measure.Qa*para.CT*para.PT*1000,measure.Qb*para.CT*para.PT*1000,
            measure.Qc*para.CT*para.PT*1000,measure.Qt*para.CT*para.PT*1000);
            break;
        //-------------------------------------------------
        case 8:
            Display_Wvar(measure.Pt*para.CT*para.PT*1000,measure.Qt*para.CT*para.PT*1000);
            break;   
           
        case 9:
            Display_Epqt(measure.Ept,measure.Eqt);                           //显示总有功电度,总无功电度
            break;
        //-----------------------------------------------                                  
        case 10:
            Display_ADDR(para.addr);
            Display_Baud(para.baud);
            break;
        case 11:
            Display_PT(para.PT);
            break;
        case 12:
            Display_CT(para.CT);
            break;    
        case 13:
            Display_Sysmode(para.sysmode);
            break;
        case 14:
            Display_LOV();
            break;
        default:
            menuPara = 0;
            break;
    }
    uintToBCD(page+1,buf);        
    buf_string(16,2, buf+2);        //显示页码;
    Display_DIO();                  
    disp_refurbish();               //全屏刷新送显;
}
//==================================
unsigned int set_value(unsigned int value,unsigned char cursor,
                        unsigned int max,unsigned int mini)
{
    switch(cursor){
        case    0:
            if(keyValue == KEY_UP){
                if(value < max) value++;                
            }
            else if(keyValue == KEY_DOWN){
                if(value > mini) value--;
            }
            break;
        case    1:
            if(keyValue == KEY_UP){
                if(value < (max-10)) value += 10;
                else value = max;
            }
            else if(keyValue == KEY_DOWN){
                if(value > (mini+10)) value -= 10;
                else value = mini;
            }
            break;
        case    2:
            if(keyValue == KEY_UP){
                if(value < (max-100)) value += 100;
                else value = max;
            }
            else if(keyValue == KEY_DOWN){
                if(value > (mini+100)) value -= 100;
                else value = mini;
            }
            break;
        case    3:
            if(keyValue == KEY_UP){
                if(value < (max-1000)) value += 1000;
                else value = max;
            }
            else if(keyValue == KEY_DOWN){
                if(value > (mini+1000)) value -= 1000;
                else value = mini;
            }
            break;
        case    4:
            if(keyValue == KEY_UP){
                if(value < (max-10000)) value += 10000;
                else value = max;
            }
            else if(keyValue == KEY_DOWN){
                if(value > (mini+10000)) value -= 10000;
                else value = mini;
            }
            break;
        default:
            break;
    }
    return(value);
}

void IntoParaMenu(void)
{
    //unsigned char i,j;
    menuStatus = PARA;
    //paraDispFlg = 0;
    menuLay = 0;
}
//////////////////////////////////////////////////////////////////////////
//
void IntoSetMenu(void)
{    
    unsigned char buf[16] = "PASS0000";
    menuStatus = SET;
    menuLay = 0;
    clr_disp_buf();
    symbol_SET();
    if(menuSet == SET_PASS){
        buf_string(0,4,buf);
        uintToBCD(para.password,buf); 
        buf_string(4,4,buf);  
    }
    else if(menuSet == SET_CT){
        symbol_CT();
        ulongToBCD(para.CT,buf);
        buf_string(3,5,buf+3); 
    }
    else if(menuSet == SET_PT){
        symbol_PT();
        ulongToBCD(para.PT,buf);
        buf_string(3,5,buf+3);  
    }
    else if(menuSet == SET_ADDR){
        symbol_Addr();
        uintToBCD(para.addr,buf);
        buf_string(5,3,buf+1); 
    }
    else if(menuSet == SET_BAUD){
        symbol_Baud();
        for(unsigned char i = 0; i < 5; i++) buf[i] = baudTbl[para.baud][i];
        buf_string(3,5,buf); 
    }
    else if(menuSet == SET_OVER_V){
        buf[0] = 'O';buf[1] = 'V';buf[2] = 'E';buf[3] = 'R';
        buf[4] = ' ';buf[5] = 'V';buf[6] = 'O';buf[7] = 'L';
        uintToBCD(para.OverVol,buf+12); buf[12] = ' ';
        buf[8] = ' ';buf[9] = ' ';buf[10] = ' ';buf[11] = ' ';
        buf_string(0,16,buf); 
    }
    else if(menuSet == SET_LOW_V){
        buf[0] = 'O';buf[1] = 'V';buf[2] = 'E';buf[3] = 'R';
        buf[4] = ' ';buf[5] = 'C';buf[6] = 'U';buf[7] = 'R';
        uintToBCD(para.LowVol,buf+12); buf[12] = ' ';
        buf[8] = ' ';buf[9] = ' ';buf[10] = ' ';buf[11] = ' ';
        buf_string(0,16,buf); 
                disp_dot7();
    }
    else if(menuSet == SET_SOFT){
        clr_disp_buf();
        buf[0] = 'S';buf[1] = 'O';buf[2] = 'F';buf[3] = 'T';
        uintToBCD(VERSION, buf+4);buf[4] = 'V';
        ulongToBCD(UPDATE, buf+8);
        buf_string(0,16,buf);
        disp_dot3();
    }
    else if(menuSet == SET_SYSMODE){
        Display_Sysmode(para.sysmode);
    }
    uintToBCD(menuSet+1,buf);
    buf_string(16,2, buf+2);
    disp_refurbish();
}

void IntoSuperSetMenu(void)
{    
    unsigned char buf[16] = "PASS0000";
    menuStatus = SUPER_SET;
    menuLay = 0;
    clr_disp_buf();
    symbol_SET();
    if(menuSet == SUPER_SET_PASS){
        buf_string(0,4,buf);
        uintToBCD(para.password,buf); 
        buf_string(4,4,buf);  
    }
    else if(menuSet == SUPER_SET_CT){
        symbol_CT();
        ulongToBCD(para.CT,buf);
        buf_string(3,5,buf+3); 
    }
    else if(menuSet == SUPER_SET_PT){
        symbol_PT();
        ulongToBCD(para.PT,buf);
        buf_string(3,5,buf+3);  
    }
    else if(menuSet == SUPER_SET_ADDR){
        symbol_Addr();
        uintToBCD(para.addr,buf);
        buf_string(5,3,buf+1); 
    }
    else if(menuSet == SUPER_SET_BAUD){
        symbol_Baud();
        for(unsigned char i = 0; i < 5; i++) buf[i] = baudTbl[para.baud][i];
        buf_string(3,5,buf); 
    }
    else if(menuSet == SUPER_SET_OVER_V){
        buf[0] = 'O';buf[1] = 'V';buf[2] = 'E';buf[3] = 'R';
        buf[4] = ' ';buf[5] = 'V';buf[6] = 'O';buf[7] = 'L';
        uintToBCD(para.OverVol,buf+12); buf[12] = ' ';
        buf[8] = ' ';buf[9] = ' ';buf[10] = ' ';buf[11] = ' ';
        buf_string(0,16,buf); 
    }
    else if(menuSet == SUPER_SET_LOW_V){
        buf[0] = 'O';buf[1] = 'V';buf[2] = 'E';buf[3] = 'R';
        buf[4] = ' ';buf[5] = 'C';buf[6] = 'U';buf[7] = 'R';
        uintToBCD(para.LowVol,buf+12); buf[12] = ' ';
        buf[8] = ' ';buf[9] = ' ';buf[10] = ' ';buf[11] = ' ';
        buf_string(0,16,buf); 
                disp_dot7();
    }
    else if (menuSet == SUPER_SET_V_PREC)
    {
        unsigned char i = 0;
        symbol_percent();
        for (i=0; i<16; i++)
        {
            buf[i] = ' ';
        }
        buf[4] = 'V'; buf[5] = 'P';buf[6] = 'R'; buf[7] = 'E';
        uintToBCD(para.V_prec, buf+12);buf[12] = ' ';buf[13] = ' ';
        buf_string(0, 16,buf);
    }
    else if (menuSet == SUPER_SET_A_PREC)
    {
        unsigned char i = 0;
        symbol_percent();
        for (i=0; i<16; i++)
        {
            buf[i] = ' ';
        }
        buf[4] = 'A'; buf[5] = 'P';buf[6] = 'R'; buf[7] = 'E';
        uintToBCD(para.A_prec, buf+12);buf[12] = ' ';
        buf_string(0, 16,buf);
        
    }
    else if (menuSet == SUPER_SET_CLR)
    {
        buf[0] = ' '; buf[1] = ' '; buf[2] = ' '; buf[3] = ' ';
        buf[4] = 'C'; buf[5] = 'L'; buf[6] = 'R'; buf[7] = ' ';
        buf[8] = ' '; buf[9] = ' '; buf[10] = ' '; buf[11] = ' ';
        buf[12] = ' ';
        for (unsigned char i = 0; i < 3; i++)
        {
            buf[13+i] = clrTbl[para.clr][i];
        }
        buf_string(0,16,buf); 
    }
    else if(menuSet == SUPER_SET_SOFT){
        clr_disp_buf();
        buf[0] = 'S';buf[1] = 'O';buf[2] = 'F';buf[3] = 'T';
        uintToBCD(VERSION, buf+4);buf[4] = 'V'; 
        ulongToBCD(UPDATE, buf+8);
        buf_string(0,16,buf);
        disp_dot3();
    }
    uintToBCD(menuSet+1,buf);
    buf_string(16,2, buf+2);
    disp_refurbish();
}

void IntoPasswordMenu(void)
{
    unsigned char buf[] = "PASS0000";
    menuStatus = PASSWORD;
    menuLay = 0;
    clr_disp_buf();        //清全显存;
    buf_string(0,8,buf);   //字串送显存;
    symbol_T1();
    disp_refurbish();     //全屏送显;
}

void ParaMenuDeal(void)
{
    if(keyValue == KEY_MODE)
    {
        IntoPasswordMenu();
        return;
    }
    else if(keyValue == KEY_DOWN)
    {    
        if(++menuPara >= PARA_MENU_NUM)
        {
            menuPara = 0;
        }
            //paraDispFlg = 0;
    }
    else if(keyValue == KEY_UP)
    {    //A,B,C,HZ转换
        if(--menuPara > PARA_MENU_NUM)
        {
            menuPara = PARA_MENU_NUM-1;
        }
           //paraDispFlg = 0;
    }
    ParaMenuDisplay();
}

void SetMenuDeal(void)
{    
    static unsigned char cursor = 0;
    static unsigned char flash = 0;
    static unsigned char buf[8]={0};
    unsigned char tmp[5]={0};
    if(menuLay > 0)
    {
        switch(menuSet)
        {
            case SET_PASS:
                if(keyValue == KEY_MODE)
                {
                    if(cursor >= 3)
                    {
                        para.password = tmpPara.password;
                        //EE_Page_Write( EEP_ADR_COM+7, 2, (unsigned char*)(&tmpPara.password) );
                        EEPROM_Write(EEP_ADR_COM+7,tmpPara.password);
                        Delay_ms(2);
                        EEPROM_Write(EEP_ADR_COM+8,tmpPara.password/256);
                        cursor = 0;
                        menuLay = 0;
                        flash = 1;
                    }
                    else
                    {
                        cursor++;
                    }
                }
                else if((keyValue == KEY_UP)||(keyValue == KEY_DOWN))
                {
                    if(tmpPara.password > 9999){tmpPara.password = 0;}
                    tmpPara.password = set_value(tmpPara.password,cursor,9999,0);
                       flash = 1;
                }
                uintToBCD(tmpPara.password,buf);
                flash = disp_flash(flash,cursor,4,4,buf);        //display
                break;
            case SET_ADDR:
                if(keyValue == KEY_MODE)
                {
                    if(cursor >= 2)
                    {
                        para.addr = tmpPara.addr;
                        EEPROM_Write(EEP_ADR_COM,tmpPara.addr);
                        cursor = 0;
                        menuLay = 0;
                        flash = 1;
                    }
                    else
                    {
                        cursor++;
                    }
                }
                else if((keyValue == KEY_UP)||(keyValue == KEY_DOWN))
                {
                    if(tmpPara.addr > 254){tmpPara.addr = 1;}
                    tmpPara.addr = set_value(tmpPara.addr,cursor,254,1);
                       flash = 1;
                }
                uintToBCD(tmpPara.addr,buf);
                flash = disp_flash(flash,cursor,5,3,buf+1);        //FLASH效果 display
                break;
            case SET_BAUD:
                if(keyValue == KEY_MODE)
                {
                    para.baud = tmpPara.baud;
                    SetBaud( para.baud );
                    EEPROM_Write(EEP_ADR_COM+1,tmpPara.baud);
                    cursor = 0;
                    menuLay = 0;
                    flash = 1;
                }
                else if(keyValue == KEY_UP)
                {
                    if(++tmpPara.baud > 4){tmpPara.baud = 0;}
                       flash = 1;
                }
                else if(keyValue == KEY_DOWN)
                {
                    if(--tmpPara.baud > 4){tmpPara.baud = 4;}
                       flash = 1;
                }
                clr_string(3,5);
                if(flash)
                {
                    for(unsigned char i = 0; i < 5; i++) tmp[i] = baudTbl[tmpPara.baud][i];
                    buf_string(3,5,tmp); 
                    disp_string(3,5);
                    flash = 0;
                }
                else
                {
                    disp_string(3,5);
                    flash = 1;
                }                
                break;
            case SET_PT:
               if(keyValue == KEY_MODE)
               {
                    if(cursor >= 4)
                    {
                        para.PT = tmpPara.PT;
                        EEPROM_Write(EEP_ADR_COM+5,tmpPara.PT);
                        Delay_ms(2);
                        EEPROM_Write(EEP_ADR_COM+6,tmpPara.PT/256);
                        cursor = 0;
                        menuLay = 0;
                        flash = 1;
                    }
                    else
                    {
                        cursor++;
                    }
                }
                else if((keyValue == KEY_UP)||(keyValue == KEY_DOWN))
                {
                    if(tmpPara.PT > MAX_PT)
                    {
                        tmpPara.PT = 1;
                    }
                    tmpPara.PT = set_value(tmpPara.PT,cursor,MAX_PT,1);
                       flash = 1;
                }
                ulongToBCD(tmpPara.PT,buf);
                flash = disp_flash(flash,cursor,3,5,buf+3);        //display
                break;
            case SET_CT:
                if(keyValue == KEY_MODE)
                {
                    if(cursor >= 4)
                    {
                        para.CT = tmpPara.CT;
                        EEPROM_Write(EEP_ADR_COM+3,tmpPara.CT);
                        Delay_ms(2);
                        EEPROM_Write(EEP_ADR_COM+4,tmpPara.CT/256);
                        cursor = 0;
                        menuLay = 0;
                        flash = 1;
                    }
                    else
                    {
                        cursor++;
                    }
                }
                else if((keyValue == KEY_UP)||(keyValue == KEY_DOWN))
                {
                    if(tmpPara.CT > MAX_CT)
                    {
                        tmpPara.CT = 1;
                    }
                    tmpPara.CT = set_value(tmpPara.CT,cursor,MAX_CT,1);
                       flash = 1;
                }
                ulongToBCD(tmpPara.CT,buf);
                flash = disp_flash(flash,cursor,3,5,buf+3);        //display
                   break;
            case SET_OVER_V:
                if(keyValue == KEY_MODE)
                {
                    if(cursor >= 2)
                    {
                        para.OverVol = tmpPara.OverVol;
                        EEPROM_Write(EEP_ADR_COM+9,*((uint8*)(&tmpPara.OverVol)));
                        Delay_ms(2);
                        EEPROM_Write(EEP_ADR_COM+10,*((uint8*)(&tmpPara.OverVol)+1));
                        cursor = 0;
                        menuLay = 0;
                        flash = 1;
                    }
                    else
                    {
                        cursor++;
                    }
                }
                else if((keyValue == KEY_UP)||(keyValue == KEY_DOWN))
                {
                    if(tmpPara.OverVol > 460)
                    {
                        tmpPara.OverVol = 380;
                    }
                    tmpPara.OverVol = set_value(tmpPara.OverVol,cursor,460,380);
                       flash = 1;
                }
                uintToBCD(tmpPara.OverVol,buf);
                if(flash)
                {
                    clr_string(13,3);
                    buf_string(13,3,buf+1);
                    disp_string(13,3);
                    flash = 0;
                }
                else
                {
                    clr_string(15-cursor,1);
                    disp_string(13,3);
                    flash = 1;
                }    
                //flash = disp_flash(flash,cursor,13,3,buf+1);        //display
                   break;
            case SET_LOW_V:
                if(keyValue == KEY_MODE)
                {
                    if(cursor >= 2)
                    {
                        para.LowVol = tmpPara.LowVol;
                        EEPROM_Write(EEP_ADR_COM+11,*((uint8*)(&tmpPara.LowVol)));
                        Delay_ms(2);
                        EEPROM_Write(EEP_ADR_COM+12,*((uint8*)(&tmpPara.LowVol)+1));
                        cursor = 0;
                        menuLay = 0;
                        flash = 1;
                    }
                    else
                    {
                        cursor++;
                    }
                }
                else if((keyValue == KEY_UP)||(keyValue == KEY_DOWN))
                {
                    if(tmpPara.LowVol > 700){tmpPara.LowVol = 500;}
                    tmpPara.LowVol = set_value(tmpPara.LowVol,cursor,700,200);
                       flash = 1;
                }
                uintToBCD(tmpPara.LowVol,buf);
                if(flash)
                {
                    clr_string(13,3);
                    buf_string(13,3,buf+1);
                                        disp_dot7();
                    disp_string(13,3);
                    flash = 0;
                }
                else
                {
                    clr_string(15-cursor,1);
                    disp_string(13,3);
                    flash = 1;
                }    
                //flash = disp_flash(flash,cursor,12,3,buf+1);        //display
                   break;
/*            case SET_V_PREC:
                if (keyValue == KEY_MODE)
                {
                    if (cursor >= 1)
                    {
                        para.V_prec = tmpPara.V_prec;
                        EEPROM_Write (EEP_ADR_COM_VPREC, tmpPara.V_prec);
                        cursor = 0;
                        menuLay = 0;
                        flash = 1;
                    }
                    else
                    {
                        cursor++;
                    }
                }
                else if ((keyValue == KEY_UP)||(keyValue == KEY_DOWN))
                {
                    if(tmpPara.V_prec > SET_V_prec_hi)
                    {
                        tmpPara.V_prec = 0;
                    }
                    tmpPara.V_prec = set_value (tmpPara.V_prec, cursor, SET_V_prec_hi, 0);
                    flash = 1;
                }
                uintToBCD(tmpPara.V_prec,buf);
                if(flash){
                    clr_string(14,2);
                    buf_string(14,2,buf+2);
                    disp_string(14,2);
                    flash = 0;
                }
                else{
                    clr_string(15-cursor,1);
                    disp_string(14,2);
                    flash = 1;
                }   
                break;
            
            
            case SET_A_PREC:
                if (keyValue == KEY_MODE)
                {
                    if (cursor >= 1)
                    {
                        para.A_prec = tmpPara.A_prec;
                        EEPROM_Write (EEP_ADR_COM_APREC, tmpPara.A_prec);
                        cursor = 0;
                        menuLay = 0;
                        flash = 1;
                    }
                    else
                    {
                        cursor++;
                    }
                }
                else if ((keyValue == KEY_UP)||(keyValue == KEY_DOWN))
                {
                    if(tmpPara.A_prec > SET_A_prec_hi)
                    {
                        tmpPara.A_prec = 0;
                    }
                    tmpPara.A_prec = set_value (tmpPara.A_prec, cursor, SET_A_prec_hi, 0);
                    flash = 1;
                }
                uintToBCD(tmpPara.A_prec,buf);
                if(flash){
                    clr_string(14,2);
                    buf_string(14,2,buf+2);
                    disp_string(14,2);
                    flash = 0;
                }
                else{
                    clr_string(15-cursor,1);
                    disp_string(14,2);
                    flash = 1;
                }
                break;
*/
            case SET_SOFT:
                if(keyValue == KEY_MODE){
                  cursor = 0;
                  menuLay = 0;
                  flash = 1;}
                //buf[4] = 'V';buf[5] = '3';buf[6] = '.';buf[7] = '2';
                uintToBCD(VERSION, buf+4);buf[4] = 'V';
                disp_dot3();
                flash = disp_flash(flash,cursor,4,4,&buf[4]);        //display
                break;

            case SET_SYSMODE:
                if(keyValue == KEY_MODE){
                    para.sysmode= tmpPara.sysmode;
                    EEPROM_Write(EEP_ADR_COM+2,tmpPara.sysmode);
                    cursor = 0;
                    menuLay = 0;
                    flash = 1;
                }
                else if(keyValue == KEY_UP){
                    if(++tmpPara.sysmode > 4){tmpPara.sysmode = 0;}
                       flash = 1;
                }
                else if(keyValue == KEY_DOWN){
                    if(--tmpPara.sysmode > 4){tmpPara.sysmode = 4;}
                       flash = 1;
                }
                clr_string(13,3);
                if(flash){
                    for(unsigned char i = 0; i < 5; i++) tmp[i] = baudTbl[tmpPara.baud][i];
                    Display_Sysmode(tmpPara.sysmode);//buf_string(3,5,tmp); 
                    disp_string(13,3);
                    flash = 0;
                }
                else{
                    disp_string(13,3);
                    flash = 1;
                }                
                break;

            default:
                 menuSet = SET_PASS;
                 break;
        }
    }
    else{
        if(keyValue == KEY_MODE){
            menuLay = 1;
            flash = 1;
            cursor = 0;
        }
        else if(keyValue == KEY_UP){
            if(--menuSet > SET_MENU_NUM) menuSet = SET_MENU_NUM;
            IntoSetMenu();                                         //显示器相应键值对应的设置菜单;
        }
        else if(keyValue == KEY_DOWN){
            if(++menuSet > SET_MENU_NUM) menuSet = 0;
            IntoSetMenu();
        }
    }
}

void SuperSetMenuDeal(void)
{    
    static unsigned char cursor = 0;
    static unsigned char flash = 0;
    static unsigned char buf[8]={0};
    unsigned char tmp[5]={0};
    if(menuLay > 0)
    {
        switch(menuSet)
        {
            case SUPER_SET_PASS:
                if(keyValue == KEY_MODE)
                {
                    if(cursor >= 3)
                    {
                        para.password = tmpPara.password;
                        //EE_Page_Write( EEP_ADR_COM+7, 2, (unsigned char*)(&tmpPara.password) );
                        EEPROM_Write(EEP_ADR_COM+7,tmpPara.password);
                        Delay_ms(2);
                        EEPROM_Write(EEP_ADR_COM+8,tmpPara.password/256);
                        cursor = 0;
                        menuLay = 0;
                        flash = 1;
                    }
                    else{
                        cursor++;
                    }
                }
                else if((keyValue == KEY_UP)||(keyValue == KEY_DOWN)){
                    if(tmpPara.password > 9999){tmpPara.password = 0;}
                    tmpPara.password = set_value(tmpPara.password,cursor,9999,0);
                       flash = 1;
                }
                uintToBCD(tmpPara.password,buf);
                flash = disp_flash(flash,cursor,4,4,buf);        //display
                break;
            case SUPER_SET_ADDR:
                if(keyValue == KEY_MODE){
                    if(cursor >= 2){
                        para.addr = tmpPara.addr;
                        EEPROM_Write(EEP_ADR_COM,tmpPara.addr);
                        cursor = 0;
                        menuLay = 0;
                        flash = 1;
                    }
                    else{
                        cursor++;
                    }
                }
                else if((keyValue == KEY_UP)||(keyValue == KEY_DOWN)){
                    if(tmpPara.addr > 254){tmpPara.addr = 1;}
                    tmpPara.addr = set_value(tmpPara.addr,cursor,254,1);
                       flash = 1;
                }
                uintToBCD(tmpPara.addr,buf);
                flash = disp_flash(flash,cursor,5,3,buf+1);        //FLASH效果 display
                break;
            case SUPER_SET_BAUD:
                if(keyValue == KEY_MODE){
                    para.baud = tmpPara.baud;
                    SetBaud( para.baud );
                    EEPROM_Write(EEP_ADR_COM+1,tmpPara.baud);
                    cursor = 0;
                    menuLay = 0;
                    flash = 1;
                }
                else if(keyValue == KEY_UP){
                    if(++tmpPara.baud > 4){tmpPara.baud = 0;}
                       flash = 1;
                }
                else if(keyValue == KEY_DOWN){
                    if(--tmpPara.baud > 4){tmpPara.baud = 4;}
                       flash = 1;
                }
                clr_string(3,5);
                if(flash){
                    for(unsigned char i = 0; i < 5; i++) tmp[i] = baudTbl[tmpPara.baud][i];
                    buf_string(3,5,tmp); 
                    disp_string(3,5);
                    flash = 0;
                }
                else{
                    disp_string(3,5);
                    flash = 1;
                }                
                break;
            case SUPER_SET_PT:
               if(keyValue == KEY_MODE){
                    if(cursor >= 4){
                        para.PT = tmpPara.PT;
                        EEPROM_Write(EEP_ADR_COM+5,tmpPara.PT);
                        Delay_ms(2);
                        EEPROM_Write(EEP_ADR_COM+6,tmpPara.PT/256);
                        cursor = 0;
                        menuLay = 0;
                        flash = 1;
                    }
                    else{
                        cursor++;
                    }
                }
                else if((keyValue == KEY_UP)||(keyValue == KEY_DOWN)){
                    if(tmpPara.PT > MAX_PT){tmpPara.PT = 1;}
                    tmpPara.PT = set_value(tmpPara.PT,cursor,MAX_PT,1);
                       flash = 1;
                }
                ulongToBCD(tmpPara.PT,buf);
                flash = disp_flash(flash,cursor,3,5,buf+3);        //display
                break;
            case SUPER_SET_CT:
                if(keyValue == KEY_MODE){
                    if(cursor >= 4){
                        para.CT = tmpPara.CT;
                        EEPROM_Write(EEP_ADR_COM+3,tmpPara.CT);
                        Delay_ms(2);
                        EEPROM_Write(EEP_ADR_COM+4,tmpPara.CT/256);
                        cursor = 0;
                        menuLay = 0;
                        flash = 1;
                    }
                    else{
                        cursor++;
                    }
                }
                else if((keyValue == KEY_UP)||(keyValue == KEY_DOWN)){
                    if(tmpPara.CT > MAX_CT){tmpPara.CT = 1;}
                    tmpPara.CT = set_value(tmpPara.CT,cursor,MAX_CT,1);
                       flash = 1;
                }
                ulongToBCD(tmpPara.CT,buf);
                flash = disp_flash(flash,cursor,3,5,buf+3);        //display
                   break;
            case SUPER_SET_OVER_V:
                if(keyValue == KEY_MODE){
                    if(cursor >= 2){
                        para.OverVol = tmpPara.OverVol;
                        EEPROM_Write(EEP_ADR_COM+9,*((uint8*)(&tmpPara.OverVol)));
                        Delay_ms(2);
                        EEPROM_Write(EEP_ADR_COM+10,*((uint8*)(&tmpPara.OverVol)+1));
                        cursor = 0;
                        menuLay = 0;
                        flash = 1;
                    }
                    else{
                        cursor++;
                    }
                }
                else if((keyValue == KEY_UP)||(keyValue == KEY_DOWN)){
                    if(tmpPara.OverVol > 460){tmpPara.OverVol = 380;}
                    tmpPara.OverVol = set_value(tmpPara.OverVol,cursor,460,380);
                       flash = 1;
                }
                uintToBCD(tmpPara.OverVol,buf);
                if(flash){
                    clr_string(13,3);
                    buf_string(13,3,buf+1);
                    disp_string(13,3);
                    flash = 0;
                }
                else{
                    clr_string(15-cursor,1);
                    disp_string(13,3);
                    flash = 1;
                }    
                //flash = disp_flash(flash,cursor,13,3,buf+1);        //display
                   break;
            case SUPER_SET_LOW_V:
                if(keyValue == KEY_MODE){
                    if(cursor >= 2){
                        para.LowVol = tmpPara.LowVol;
                        EEPROM_Write(EEP_ADR_COM+11,*((uint8*)(&tmpPara.LowVol)));
                        Delay_ms(2);
                        EEPROM_Write(EEP_ADR_COM+12,*((uint8*)(&tmpPara.LowVol)+1));
                        cursor = 0;
                        menuLay = 0;
                        flash = 1;
                    }
                    else{
                        cursor++;
                    }
                }
                else if((keyValue == KEY_UP)||(keyValue == KEY_DOWN)){
                    if(tmpPara.LowVol > 700){tmpPara.LowVol = 500;}
                    tmpPara.LowVol = set_value(tmpPara.LowVol,cursor,700,200);
                       flash = 1;
                }
                uintToBCD(tmpPara.LowVol,buf);
                if(flash){
                    clr_string(13,3);
                    buf_string(13,3,buf+1);
                                        disp_dot7();
                    disp_string(13,3);
                    flash = 0;
                }
                else{
                    clr_string(15-cursor,1);
                    disp_string(13,3);
                    flash = 1;
                }    
                //flash = disp_flash(flash,cursor,12,3,buf+1);        //display
                   break;
            case SUPER_SET_V_PREC:
                if (keyValue == KEY_MODE)
                {
                    if (cursor >= 1)
                    {
                        para.V_prec = tmpPara.V_prec;
                        EEPROM_Write (EEP_ADR_COM_VPREC, tmpPara.V_prec);
                        cursor = 0;
                        menuLay = 0;
                        flash = 1;
                    }
                    else
                    {
                        cursor++;
                    }
                }
                else if ((keyValue == KEY_UP)||(keyValue == KEY_DOWN))
                {
                    if(tmpPara.V_prec > SET_V_prec_hi)
                    {
                        tmpPara.V_prec = 0;
                    }
                    tmpPara.V_prec = set_value (tmpPara.V_prec, cursor, SET_V_prec_hi, 0);
                    flash = 1;
                }
                uintToBCD(tmpPara.V_prec,buf);
                if(flash){
                    clr_string(14,2);
                    buf_string(14,2,buf+2);
                    disp_string(14,2);
                    flash = 0;
                }
                else{
                    clr_string(15-cursor,1);
                    disp_string(14,2);
                    flash = 1;
                }   
                break;
            
            
            case SUPER_SET_A_PREC:
                if (keyValue == KEY_MODE)
                {
                    if (cursor >= 2)
                    {
                        para.A_prec = tmpPara.A_prec;
                        EE_Page_Write(EEP_ADR_COM_APREC, 2,((unsigned char*)(&para.A_prec)));//EEPROM_Write (EEP_ADR_COM_APREC, tmpPara.A_prec);
                        cursor = 0;
                        menuLay = 0;
                        flash = 1;
                    }
                    else
                    {
                        cursor++;
                    }
                }
                else if ((keyValue == KEY_UP)||(keyValue == KEY_DOWN))
                {
                    if(tmpPara.A_prec > SET_A_prec_hi)
                    {
                        tmpPara.A_prec = 0;
                    }
                    tmpPara.A_prec = set_value (tmpPara.A_prec, cursor, SET_A_prec_hi, 0);
                    flash = 1;
                }
                uintToBCD(tmpPara.A_prec,buf);
                if(flash){
                    clr_string(13,3);
                    buf_string(13,3,buf+1);
                    disp_string(13,3);
                    flash = 0;
                }
                else{
                    clr_string(15-cursor,1);
                    disp_string(13,3);
                    flash = 1;
                }
                break;

            case SUPER_SET_CLR:
                if (keyValue == KEY_MODE)
                {
                    para.clr = tmpPara.clr;
                    if (tmpPara.clr == 1)
                    {
                        DN_Sample[0] = 0;
                        DN_Sample[1] = 0;
                        EE_Page_Write( EEP_ADDR_energy, 8, (unsigned char*)(&DN_Sample[0]));
                        Delay_ms(2);
                    }
                    cursor = 0;
                    menuLay = 0;
                    flash = 1;
                }
                else if (keyValue == KEY_UP)
                {
                    if (++tmpPara.clr > 1)
                    {
                        tmpPara.clr = 0;
                    }
                    flash = 1;
                }
                else if (keyValue == KEY_DOWN)
                {
                    if (--tmpPara.clr > 1)
                    {
                        tmpPara.clr = 1;
                    }
                    flash = 1;
                }
                clr_string (13,3);

                if (flash)
                {
                    for (unsigned char i = 0; i < 3; i++)
                    {
                        tmp[i] = clrTbl[tmpPara.clr][i];
                    }
                    buf_string (13, 3, tmp); 
                    disp_string (13, 3);
                    flash = 0;
                }
                else
                {
                    disp_string (13, 3);
                    flash = 1;
                }        
                break;

            case SUPER_SET_SOFT:
                if(keyValue == KEY_MODE){
                  cursor = 0;
                  menuLay = 0;
                  flash = 1;}
                //buf[4] = 'V';buf[5] = '3';buf[6] = '.';buf[7] = '2';
                uintToBCD(VERSION, buf+4);buf[4] = 'V'; 
                disp_dot3();
                flash = disp_flash(flash,cursor,4,4,&buf[4]);        //display
                break;
            case SUPER_SET_SYSMODE:
                if(keyValue == KEY_MODE){
                    para.sysmode= tmpPara.sysmode;
                    EEPROM_Write(EEP_ADR_COM+2,tmpPara.sysmode);
                    cursor = 0;
                    menuLay = 0;
                    flash = 1;
                }
                else if(keyValue == KEY_UP){
                    if(++tmpPara.sysmode > 4){tmpPara.sysmode = 0;}
                       flash = 1;
                }
                else if(keyValue == KEY_DOWN){
                    if(--tmpPara.sysmode > 4){tmpPara.sysmode = 4;}
                       flash = 1;
                }
                clr_string(13,3);
                if(flash){
                    for(unsigned char i = 0; i < 5; i++) tmp[i] = baudTbl[tmpPara.baud][i];
                    Display_Sysmode(tmpPara.sysmode);//buf_string(3,5,tmp); 
                    disp_string(13,3);
                    flash = 0;
                }
                else{
                    disp_string(13,3);
                    flash = 1;
                }                
                break;
            default:
                 menuSet = SET_PASS;
                 break;
        }
    }
    else{
        if(keyValue == KEY_MODE){
            menuLay = 1;
            flash = 1;
            cursor = 0;
        }
        else if(keyValue == KEY_UP){
            if(--menuSet > SUPER_SET_MENU_NUM) menuSet = SUPER_SET_MENU_NUM;
            IntoSuperSetMenu();                                         //显示器相应键值对应的设置菜单;
        }
        else if(keyValue == KEY_DOWN){
            if(++menuSet > SUPER_SET_MENU_NUM) menuSet = 0;
            IntoSuperSetMenu();
        }
    }
}

unsigned char GetBCD(unsigned char data)
{
    unsigned char temp=0;
    if (data<10)
    {
        temp = data+0x30;
    }
    else if ((data>=10)&&(data<=15))
    {
        temp = data+0x41-10;
    }
    else
    {
        temp = 0x20;
    }

    return temp;
}

void Hex2Bcd(unsigned char hexdata, unsigned char *bcddata)
{
    bcddata[0]=GetBCD((hexdata>>4)&0xF);
    bcddata[1]=GetBCD(hexdata&0xF);
}

void PrintDataDisplay(void)
{
    unsigned char buf[16]={0};
    unsigned char i=0;
    unsigned char hex_buf[16]={0};
    clr_disp_buf();      //清全部显示缓存;
    memcpy(hex_buf, Uart_Rx_Buf, 8);
    for (i=0; i<8; i++)
    {
        Hex2Bcd(hex_buf[i], &buf[2*i]);
    }
    buf_string(0,16, buf);
    disp_refurbish();
}

void PrintDataDeal(void)
{
    if(keyValue == KEY_MODE)
    {
        IntoPasswordMenu();
        return;
    }
    PrintDataDisplay();
}

void PasswordMenuDeal(void)
{
    static unsigned char cursor = 0;
    static unsigned int password = 0;
    static unsigned char buf[6]={0};
    static unsigned char flash = 0;
    if(menuLay)
    {
        if(keyValue == KEY_MODE)
        {
            if(cursor >=3){
                if(password == para.password)
                {
                    tmpPara.PT = para.PT;
                    tmpPara.CT = para.CT;
                    tmpPara.addr = para.addr;
                    tmpPara.password = para.password;
                    tmpPara.baud = para.baud;
                    tmpPara.OverVol = para.OverVol;
                    tmpPara.LowVol = para.LowVol;
                    //tmpPara.V_prec = para.V_prec;
                    //tmpPara.A_prec = para.A_prec;
                    menuSet = SET_PASS;
                    IntoSetMenu();          //密码相符进入到IntoSetMenu;
                }
                else if (password == SUPER_PASS)
                {
                    tmpPara.PT = para.PT;
                    tmpPara.CT = para.CT;
                    tmpPara.addr = para.addr;
                    tmpPara.password = para.password;
                    tmpPara.baud = para.baud;
                    tmpPara.OverVol = para.OverVol;
                    tmpPara.LowVol = para.LowVol;
                    tmpPara.V_prec = para.V_prec;
                    tmpPara.A_prec = para.A_prec;
                    menuSet = SUPER_SET_PASS;
                    IntoSuperSetMenu();
                }
                else{
                    IntoParaMenu();        //密码不对退出到IntoParaMenu;
                }
                return;
            }
            else{
                cursor++;
            }
        }
        else if((keyValue == KEY_UP)||(keyValue == KEY_DOWN))
        {
            password = set_value(password,cursor,9999,0);         //取键值;
            uintToBCD(password,buf);                              //将键值传换成BCD码;
            flash = 1;
        }
        if(flash){
            clr_string(4,4);
            buf_string(4,4,buf);                                 //将键值送显存;
            disp_string(4,4);                                    //显示键值;
            flash = 0;
        }
        else{
            clr_string(7-cursor,1);                             //清光标所在数字位,形成FLASH效果;
            disp_string(4,4);
            flash = 1;
        }            
    }
    else{
        if(keyValue == KEY_MODE){
            menuLay = 1;
            password = 0;
            cursor = 0;
            flash = 1;
            uintToBCD(password,buf);
        }
    }            
}

void MenuProcess(void)              
{
    if((menuStatus != PARA)&&(keyValue == KEY_EXIT)) // 不在参数显示页面，且按下退出键
    {
        IntoParaMenu();
        keyValue = 0;
        return;
    }
    else if ((keyValue==KEY_PRINT)&&(menuStatus == PARA)) // 在参数显示页面，按下组合键+-进入打印界面
    {
        menuStatus = PRINT;
    }
    switch(menuStatus)
    {
        case    PARA:                 
            ParaMenuDeal();             //判断键值并显示相应键值界面;
            break;
        case    PASSWORD:
            PasswordMenuDeal();
            break;
        case    SET:
            SetMenuDeal();
            break;
        case    SUPER_SET:
            SuperSetMenuDeal();
            break;
        case    PRINT:
            PrintDataDeal();
            break;
        default:
            menuStatus = PARA;
            ParaMenuDeal();            
            break;
    }        
    keyValue = 0;
}





