#include <msp430x14x.h>
#include "display_maps.h"
#include "glb_var.h"
#include "lcd.h"



/*===================================================
*             SEGx  SEGx+1
* COM1  D0      A      
* COM2  D1      B     
* COM3  D2      F     
* COM4  D3      G     
* COM5  D0      C      
* COM6  D1      E     
* COM7  D2      D     
* COM8  D3           
*         A
*       ——
     F |_G_|B
     E |   |C
        —— .
        D
extern unsigned char dispBuf[64]; 
extern const unsigned char charTbl[];
extern const unsigned char digitalRamMaps[];
====================================================*/

const unsigned char digitalTbl[] = {
//     0     1    2    3    4    5    6    7    8    9  
     0x77,0x12,0x6b,0x5b,0x1e,0x5d,0x7d,0x13,0x7F,0x5F 
};

const unsigned char bcdTbl[] = {
//     0   1    2    3    4   5    6    7   8    9   A   B    C    D   E   F 
     0x77,0x12,0x6b,0x5b,0x1e,0x5d,0x7d,0x13,0x7F,0x5F,0x3F,0x7C,0x65,0x7A,0x6D,0x2D 
};

const unsigned char charsTbl[] = {
//    AKU  BLV  CMW  DNX  EOY  FPZ  GQ   HR   IS    JT    
    0x3f,0x7c,0x65,0x7a,0x6d,0x2d,0x5f,0x3E,0x12,0x52,
    0x2c,0x64,0x37,0x38,0x78,0x2f,0x1f,0x28,0x5d,0x6c,
    0x76,0x70,0x76,0x3e,0x5e,0x6b    
};

const unsigned char digitalRamMaps[] = {
    32,34,36,38,40,42,44,46,0,22,20,18,16,14,12,10,6,8
};

void disp_string(unsigned char col,unsigned char leng)
{
    unsigned char i,addr;
    for(i = leng; i > 0; i--){
        if(col >= MAX_DISP_NUM) break;
        addr = digitalRamMaps[col];
        LCDByte_Write(addr,dispBuf[addr]);
        LCDByte_Write(addr+1,dispBuf[addr+1]);
        col++;
    }    
}
void buf_string(unsigned char col,unsigned char leng, unsigned char *str) //数码送显示缓存
{
    unsigned char i,addr;
    for(i = leng; i > 0; i--){
        if(col >= MAX_DISP_NUM) break;
        if(*str < 10){
            addr = digitalRamMaps[col];
            dispBuf[addr] |= 0x0f&digitalTbl[*str];
            dispBuf[addr+1] |= digitalTbl[*str]>>4;
            str++; col++;
        }
        else if(*str == ' '){
            str++; col++;
        }
        else if((*str > 0x2f)&&(*str < 0x3a)){
            *str -= 0x30;
            addr = digitalRamMaps[col];
            dispBuf[addr] |= 0x0f&digitalTbl[*str];
            dispBuf[addr+1] |= digitalTbl[*str]>>4;
            str++; col++;
        }
        else if((*str > 0x40)&&(*str < 0x5b)){
            *str -= 0x41;
            addr = digitalRamMaps[col];
            dispBuf[addr] |= 0x0f&charsTbl[*str];
            dispBuf[addr+1] |= charsTbl[*str]>>4;
            str++; col++;
        }
        else{
            str++; col++;
        }
    }        
}
void clr_string(unsigned char col,unsigned char leng)
{
    unsigned char i,addr;
    for(i = leng; i > 0; i--){
        if(col >= MAX_DISP_NUM) break;
        addr = digitalRamMaps[col];
        dispBuf[addr] = 0;
        dispBuf[addr+1] = 0;
        col++;
    }
}
void clr_disp_buf(void)
{
    unsigned char i;
    for(i = 0; i < MAX_DISP_RAM; i++){
        dispBuf[i] = 0;
    }
}

void uintToBCD(unsigned int dat,unsigned char *ptr)  //处理小于10000的数
{
    unsigned char i;
    unsigned int div= 1000;
    if(dat >= 10000) dat /= 10;
    for(i = 0; i < 4; i++){
        *ptr = dat/div;
        dat %= div;
        div /= 10;
        ptr++;
    }    
}

void ulongToBCD(unsigned long dat,unsigned char *ptr)
{
    unsigned char i;
    unsigned long div = 10000000;
    for(i = 0; i < 8; i++){
        *ptr = dat/div;
        dat %= div;
        div /= 10;
        ptr++;
    }    
}
unsigned char disp_flash(unsigned char flg,unsigned char cursor,
                        unsigned char col,unsigned char len,unsigned char *str)
{
    if(flg){
        clr_string(col,len);
        buf_string(col,len,str);
        disp_string(col,len);
        flg = 0;
    }
    else{
        clr_string(7-cursor,1);
        disp_string(col,len);
        flg = 1;
    }    
    return(flg);        
}
    
void hideZeroDisp(unsigned char *ptr,unsigned char size)
{
    unsigned char i;
    for(i = size-1; i > 0; i--){
        if(*ptr == digitalTbl[0]){
            *ptr = 0;
            ptr++;
        }
        else{
            return;
        }
    }
}
void disp_refurbish(void)
{
    LCD_Update(dispBuf,MAX_DISP_RAM);
}    



