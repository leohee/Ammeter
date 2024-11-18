#ifndef DISPLAY_MAPS__
#define DISPLAY_MAPS__

#define MAX_DISP_NUM 18
#define MAX_DISP_RAM 48

#define symbol_Vm()      dispBuf[27] |= 1
#define symbol_Am()      dispBuf[47] |= 8
#define symbol_PFm()     dispBuf[27] |= 1;dispBuf[47] |= 8
#define symbol_Wm()      dispBuf[29] |= 8                    //W
#define symbol_E()       dispBuf[29] |= 1

#define symbol_L1()      dispBuf[24] |= 8
#define symbol_2()       dispBuf[25] |= 1
#define symbol_L12()     dispBuf[24] |= 8;dispBuf[25] |= 1

#define symbol_L2()      dispBuf[31] |= 4
#define symbol_3()       dispBuf[31] |= 2
#define symbol_L23()     dispBuf[31] |= 6

#define symbol_L3()      dispBuf[26] |= 1
#define symbol_1()       dispBuf[26] |= 2
#define symbol_L31()     dispBuf[26] |= 3

#define symbol_V()       dispBuf[27] |= 1
#define symbol_A()       dispBuf[27] |= 2
#define symbol_VA()      dispBuf[27] |= 3
//#define symbol_KVA()   dispBuf[26] |= 8;dispBuf[27] |= 3   //KVA
//#define symbol_KW()    dispBuf[27] |= 4;dispBuf[29] |= 8    //KW
#define symbol_W()       dispBuf[29] |= 8                    //W
//#define symbol_Kvar()  dispBuf[26] |= 8;dispBuf[27] |= 11      //Kvar
#define symbol_var()     dispBuf[27] |= 11   //;dispBuf[27] |= 2    //var
#define symbol_KWh()     dispBuf[27] |= 4;dispBuf[29] |= 12    //KWh
#define symbol_Kvarh()   dispBuf[26] |= 8;dispBuf[27] |= 11;\
                         dispBuf[31] |= 8    //Kvar
#define symbol_Hz()      dispBuf[28] |= 4    //Hz
#define symbol_PF()      dispBuf[29] |= 2    //PF
#define symbol_Avg()     dispBuf[28] |= 8    //Avg
#define symbol_N()       dispBuf[28] |= 2    //N
#define symbol_THD()     dispBuf[30] |= 8    //THD
#define symbol_percent() dispBuf[30] |= 4  // %

#define symbol_Addr()   dispBuf[30] |= 1    //Addr
#define symbol_Baud()   dispBuf[30] |= 2    //baud
#define symbol_Pwd()    dispBuf[28] |= 1    //pwd
#define symbol_TX()         //TX
#define symbol_T1()     dispBuf[41] |= 8    //T1
#define symbol_T2()     dispBuf[26] |= 4    //T2
#define symbol_T3()     dispBuf[4] |= 1        //T3
#define symbol_T4()     dispBuf[31] |= 1    //T4

#define symbol_SET()    dispBuf[24] |= 2    //SET
#define symbol_dmd()    dispBuf[24] |= 1    //dmd
#define symbol_CT()     dispBuf[24] |= 4    //CT
#define symbol_PT()     dispBuf[25] |= 2    //PT
#define symbol_K()      dispBuf[35] |= 8    //K
#define symbol_M()      dispBuf[33] |= 8    //M
#define symbol_DI()     dispBuf[25] |= 8    //DI
#define symbol_DO()     dispBuf[4] |= 2        //DO
#define symbol_DI1on()     dispBuf[1] |= 8;dispBuf[3] |= 4;dispBuf[3] &= ~8    //DI10
#define symbol_DI1off()    dispBuf[1] |= 8;dispBuf[3] |= 8;dispBuf[3] &= ~4    //DI11
#define symbol_DI2on()     dispBuf[2] |= 8;dispBuf[3] |= 1;dispBuf[3] &= ~2    //DI20
#define symbol_DI2off()    dispBuf[2] |= 8;dispBuf[3] |= 2;dispBuf[3] &= ~1    //DI21
#define symbol_DI3on()     dispBuf[2] |= 2;dispBuf[2] |= 1;dispBuf[2] &= ~4    //DI30
#define symbol_DI3off()    dispBuf[2] |= 2;dispBuf[2] |= 4;dispBuf[2] &= ~1    //DI31
#define symbol_DO1on()     dispBuf[4] |= 8;dispBuf[5] |= 1;dispBuf[4] &= ~4    //DO10
#define symbol_DO1off()    dispBuf[4] |= 8;dispBuf[4] |= 4;dispBuf[5] &= ~1    //DO11
#define symbol_DO2on()     dispBuf[5] |= 4;dispBuf[5] |= 8;dispBuf[5] &= ~2    //DO20
#define symbol_DO2off()    dispBuf[5] |= 4;dispBuf[5] |= 2;dispBuf[5] &= ~8    //DO22


#define disp_dot1()     dispBuf[37] |= 8
#define disp_dot2()     dispBuf[39] |= 8
#define disp_dot3()     dispBuf[43] |= 8
#define disp_dot4()     dispBuf[45] |= 8
#define disp_dot5()     dispBuf[23] |= 8
#define disp_dot6()     dispBuf[21] |= 8
#define disp_dot7()     dispBuf[15] |= 8
#define disp_dot8()     dispBuf[13] |= 8


void clr_disp_buf(void);
void clr_string (unsigned char col,unsigned char leng);
void buf_string (unsigned char col,unsigned char leng, unsigned char *str);
void disp_string (unsigned char col,unsigned char leng);
void uintToBCD (unsigned int dat,unsigned char *ptr);
void ulongToBCD (unsigned long dat,unsigned char *ptr);
void hideZeroDisp (unsigned char *ptr,unsigned char size);
void disp_refurbish (void);
unsigned char disp_flash (unsigned char flg,unsigned char cursor,
	unsigned char col,unsigned char len,unsigned char *str);



#endif


