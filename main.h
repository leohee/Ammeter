#ifndef MAIN_H
#define MAIN_H

#define CT_SMPL
#define U_I_RATIO_ADJUST

//#define REF_INTERNAL // 基准选择:内部/外部


//#define DI_CHK_KEEP

#define uint8 unsigned char
//

#define MAX_PT 10000
#define MAX_CT 10000

#ifdef CT_SMPL
    #define VOL_SMPL_SCALE 460        //电压比例 变比为200/(0.001*430)
#else
    #define VOL_SMPL_SCALE 501        //电压比例 分压比为2/(1000+2)=1/501
#endif

#define CUR_SMPL_SCALE 1000        //电流缩放比例 电流互感器参数 5A/5MA
#define DEFAULT_VPREC     20        //归0电压v
#define DEFAULT_APREC     200        //归0电流ma

#define LOGIC_TRUE              0x01
#define LOGIC_FALSE             0x00

#define REG_ADR_BAUD        (40205)
#define REG_ADR_ADDRESS              (40201)
#define REG_ADR_MODE              (40203)
#define REG_ADR_CT                (40202)
#define REG_ADR_PT                (40204)

#define REG_ADR_CLR                (40206)        //清电能
#define REG_ADR_V_prec          (40207) // 电压最低值百分比
#define REG_ADR_A_prec          (40208) // 电流最低值百分比
#define REG_ADR_MUL             (40101)
#define REG_ADR_OFF        (40151)
//081210
#define REG_RY1_O                (40209)        //DO1 ctrl
#define REG_RY2_O                (40214)        //DO2 ctrl

//EEPROM 地址
#define EEP_ADR_FLAG        (0x000)
#define EEP_ADR_COM            (0x020)
#define EEP_ADR_MUL            (0x040)
#define EEP_ADR_OFF            (0x060)
#define EEP_ADR_DIANNENG    (0x080)
#define EEP_ADDR_energy        (0x0A0)
#define EEP_ADR_COM_VPREC      (0x0E0)
#define EEP_ADR_COM_APREC      (0x0E1)


#define mima_num_address1    EEP_ADR_BACKUP_OFFSET-1
#define mima_num_address2    EEP_ADR_BACKUP_OFFSET-2
#define mima_num_address3    EEP_ADR_BACKUP_OFFSET-3
#define mima_num_address4    EEP_ADR_BACKUP_OFFSET-4

#define EEP_ADR_BACKUP_OFFSET    (0x100)

/*****************************************
* 开关量定义
******************************************/
extern unsigned char DI1_State;
extern unsigned char DI2_State;
extern unsigned char DI3_State;

extern unsigned char K1_State;
extern unsigned char K2_State;

/*****************************************
* 电能变量定义
******************************************/
extern unsigned long gEpt;    // 总有功电能
extern unsigned long gEqt;    // 总无功电能
extern unsigned long gInEpt;  // 输入有功电能
extern unsigned long gInEqt;  // 输入无功电能
extern unsigned long gOutEpt; // 输出有功电能
extern unsigned long gOutEqt; // 输出无功电能

/**********************
*EEPROM
**********************/
extern unsigned char gRegisterWriteFlagCom;
extern unsigned char gRegisterWriteFlagMul;
extern unsigned char gRegisterWriteFlagOff;
extern unsigned char gRegisterWriteFlagEpt;
extern unsigned char gRegisterWriteFlagEqt;
extern unsigned char gRegisterWriteFlagDianNeng;

extern unsigned char gRegisterWriteStatus;
extern unsigned int gRegWriteLoopCnt;
extern unsigned int gE2Address;
extern unsigned char gE2Size;

extern unsigned char gE2Buf[];


extern unsigned int gMode;
extern unsigned int gMul[12];
extern unsigned int gOff[12];
extern unsigned long gDianNeng[];

#endif

