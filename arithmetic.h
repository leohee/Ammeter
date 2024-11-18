#ifndef ARITHMETIC_H
#define ARITHMETIC_H


#define CALC_CNT 10
#define VAL_ERR 20
#define CUR_ERR 0.3
#define ui_ERR 250
#define CUR_OFFSET 0.15

float VoltageA_Process();    //UA 采样数据处理
float VoltageB_Process();   //UB 采样数据处理
float VoltageC_Process();    //UC 采样数据处理
float CurrentA_Process();    //IA 采样数据处理
float CurrentB_Process();    //IB 采样数据处理
float CurrentC_Process();    //IC 采样数据处理
float CurrentN_Process();    //IC 采样数据处理
float Pa_deal(void);
float Pb_deal(void);
float Pc_deal(void);
void u_i_adjust_deal(void);
void u_i_ofset_deal(void);


#endif

