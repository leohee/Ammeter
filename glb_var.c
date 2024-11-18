
#define __GLB_VAR_C__

#include "glb_var.h"        

unsigned char DIkeepFlg1 = 0,DIkeepFlg2 = 0,DIkeepFlg3 = 0,DIkeepFlg4 = 0;
unsigned char DIkeepCnt1 = 0,DIkeepCnt2 = 0,DIkeepCnt3 = 0,DIkeepCnt4 = 0;

unsigned char menuStatus = 0;
unsigned char menuPara = 0;
unsigned char paraDispCnt = 0;

unsigned char menuPass = 0;
unsigned char menuSet = 0;
unsigned char menuLay = 0;
unsigned char keyValue = 0;
unsigned char keyTmp = 0;
unsigned char keyFlg = 0;

unsigned char u_i_adjust_flg=0;
unsigned char u_i_adjust_cnt=0;

unsigned char DI_status[4]={0};
unsigned char DO_status[2]={0};

unsigned char dispBuf[48]={0};

struct UI_Sample smpl;
struct measure_real_value measure;

struct SetPara para;
struct SetPara tmpPara;

unsigned long gDianNeng[6]={0};

unsigned long DN_Sample[2]={0};


