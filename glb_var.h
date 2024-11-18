
#ifndef ___GLB_VAR_H__
#define ___GLB_VAR_H__


#define SMPL_MAX 32

#define VERSION 362
#define UPDATE 20120620

#define SET_V_prec_hi 50   //v
#define SET_A_prec_hi 500 //MA

struct UI_Sample
{
    unsigned int AD_value[6][SMPL_MAX];    //三相电压电流////++需增加average,每个通道各一个
    unsigned char cnt;
    unsigned char flg;
    unsigned int average[7];
};
    
struct measure_real_value
{
    float Pa ;            //A相有功功率 
    float Pb;            //B相有功功率 
    float Pc;            //C相有功功率 
    float Pt;            //合相有功功率 
    float Qa;            //A相无功功率 
    float Qb;            //B相无功功率 
    float Qc;            //C相无功功率 
    float Qt;            //合相无功功率 
    float Sa;            //A相视在功率 
    float Sb;            //B相视在功率 
    float Sc;            //C相视在功率 
    float St;            //合相视在功率 
    float Ua;        //A相电压有效值 
    float Ub;        //B相电压有效值 
    float Uc;        //C相电压有效值 
    float Ut;           // 合相电压有效值 
    float Uva;        //AB相电压有效值 
    float Uvb;        //BC相电压有效值 
    float Uvc;        //CA相电压有效值 
    float Uvt;          // 合相电压有效值 
    float Ia;        //A相电流有效值 
    float Ib;        //B相电流有效值 
    float Ic;        //C相电流有效值 
    float In;           //零相电流有效值 
    float Pfa;            //A相功率因数 
    float Pfb;            //B相功率因数 
    float Pfc;            //C相功率因数 
    float Pft;            //合相功率因数 
    float Frq;            //线频率 
    long Epa;            //A相有功电能 
    long Epb;            //B相有功电能 
    long Epc;            //C相有功电能 
    long Epti;            //输入电能
    long Epto;            //输出电能
    long Ept;            //合相有功电能 
    long Eqa;            //A相无功电能 
    long Eqb;            //B相无功电能 
    long Eqc;            //C相无功电能 
    long Eqti;            //输入电能
    long Eqto;            //输出电能
    long Eqt;            //合相无功电能     
};    

struct SetPara
{    
    unsigned char addr;
    unsigned char baud;
    unsigned char sysmode;
    unsigned int CT;
    unsigned int PT;
    unsigned int password;
    unsigned int OverVol;
    unsigned int LowVol;
    unsigned char V_prec;
    unsigned char A_prec;
    unsigned char clr;
};

extern unsigned char menuStatus;
extern unsigned char menuPara;
extern unsigned char paraDispCnt;

extern unsigned char menuPass;
extern unsigned char menuSet;
extern unsigned char menuLay;
extern unsigned char keyValue;
extern unsigned char keyTmp;
extern unsigned char keyFlg;

extern unsigned char u_i_adjust_flg;
extern unsigned char u_i_adjust_cnt;

extern unsigned char DI_status[];
extern unsigned char DO_status[2];

extern unsigned char dispBuf[48];

extern struct UI_Sample smpl;
extern struct measure_real_value measure;

extern struct SetPara para;
extern struct SetPara tmpPara;

extern unsigned long gDianNeng[6];
extern unsigned long DN_Sample[2];

extern unsigned char DIkeepFlg1;
extern unsigned char DIkeepFlg2;
extern unsigned char DIkeepFlg3;
extern unsigned char DIkeepCnt1;
extern unsigned char DIkeepCnt2;
extern unsigned char DIkeepCnt3;

#endif

