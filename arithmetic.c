#include <msp430x14x.h>
#include "osc.h"
#include "math.h"
#include "dio.h"
#include "uart.h"
#include "main.h"
#include "glb_var.h"


#define OFFSETCOUNTER   80

// UA 采样数据处理
float VoltageA_Process (void)
{
    unsigned int vol16;
    unsigned long vol32 = 0;
    float AVoltage_Sum=0;
    static unsigned int UA_Sample_Average[10];
    static unsigned char UA_Sample_Average_Cnt=0;
    static unsigned char UA_Sample_Average_Flag=0,UAoff_set_cnt = 100;
    static float UA_Temp=0;
    static float UA_Temp_Var=0;
    static unsigned char UA_Count=0;  
    static float UA_Temp_Offset=0;
    for(unsigned char i=0;i<SMPL_MAX;i++) {
		vol32 += smpl.AD_value[0][i];
	}

    smpl.average[0] = vol32/SMPL_MAX;
    for (unsigned char i=0;i<SMPL_MAX;i++) {
        if (smpl.AD_value[0][i]> smpl.average[0]) {
            vol16 = smpl.AD_value[0][i] - smpl.average[0];
		} else {
			vol16 = smpl.average[0] - smpl.AD_value[0][i];
		}
        AVoltage_Sum = AVoltage_Sum + (unsigned long)vol16*(unsigned long)vol16;
	}

    AVoltage_Sum = AVoltage_Sum/SMPL_MAX;
    AVoltage_Sum=sqrt(AVoltage_Sum);

#ifdef REF_INTERNAL
	AVoltage_Sum=AVoltage_Sum*VOL_SMPL_SCALE*2.5;
#else                   
	AVoltage_Sum=AVoltage_Sum*VOL_SMPL_SCALE*3;
#endif

    AVoltage_Sum=AVoltage_Sum/4096;     

    if (fabs(UA_Temp_Var-AVoltage_Sum)>10) {
        UA_Sample_Average_Flag=0;
        UA_Sample_Average_Cnt=0;
        UA_Count=0;
        UAoff_set_cnt=100;
        UA_Temp_Offset=0;
        UA_Temp_Var=AVoltage_Sum;
	} else {
        if (UA_Sample_Average_Flag==0) {
            UA_Temp = 0.5*(AVoltage_Sum+UA_Temp_Var);
            vol16 = (unsigned int)UA_Temp*10; 
            for (unsigned char i  =0; i < 10; i++) {
                UA_Sample_Average[i] = vol16;
			}
            UA_Sample_Average_Flag = 1;
            UAoff_set_cnt=100;
            UA_Temp_Offset=0;
            UA_Temp_Var=UA_Temp;
		} else {
            UA_Sample_Average[UA_Sample_Average_Cnt]=(unsigned int)AVoltage_Sum*10;
			UA_Sample_Average_Cnt++;
			if (UA_Sample_Average_Cnt>9) {
                    UA_Sample_Average_Cnt = 0;
			}            
            UA_Temp=0;
            for(unsigned char j=0;j<10;j++) {
            	UA_Temp=UA_Temp+UA_Sample_Average[j];
			}            
               UA_Temp /= 100;
               //UA_Temp = (UA_Temp + UA_Temp_Var)/2;
               if((fabs(UA_Temp-UA_Temp_Var)>0.01))
                {
                   if((fabs(UA_Temp-UA_Temp_Var)>2))
                    {
                    UA_Count++;
                    if(UA_Count>10)
                        {
                        UA_Sample_Average_Flag=0;
                        UA_Sample_Average_Cnt=0;
                        UA_Count=0;
                        }
                    UA_Temp_Var=UA_Temp_Var*0.95+UA_Temp*0.05;
                    UAoff_set_cnt=100;
                    UA_Temp_Offset=0;
                    }
                else
                    {
                    UA_Count=0;
                    if(UA_Temp > UA_Temp_Var)
                        {
                        UAoff_set_cnt++;
                        UA_Temp_Offset=UA_Temp_Offset+UA_Temp-UA_Temp_Var;
                        }
                    else if(UA_Temp < UA_Temp_Var)
                        {
                        UAoff_set_cnt--;
                        UA_Temp_Offset=UA_Temp_Offset+UA_Temp-UA_Temp_Var;
                        }
                    else
                        {
                        UAoff_set_cnt=100;
                        UA_Temp_Offset=0;
                        }
                    if(UAoff_set_cnt>=(100+OFFSETCOUNTER))
                        {
                        UA_Temp_Var=UA_Temp_Var+(UA_Temp_Offset)/(UAoff_set_cnt-100);
                        UAoff_set_cnt=100;
                        UA_Temp_Offset=0;
                        }
                    else if(UAoff_set_cnt<=(100-OFFSETCOUNTER))
                        {
                        UA_Temp_Var=UA_Temp_Var+(UA_Temp_Offset)/(100-UAoff_set_cnt);
                        UAoff_set_cnt=100;
                        UA_Temp_Offset=0;
                        }
                    }
                }
            else
                {
                UA_Temp_Var=UA_Temp;
                UA_Count=0;
                UAoff_set_cnt=100;
                UA_Temp_Offset=0;
                }
            }
        }
    //UA_Temp_Var=UA_Temp;
    //UA_Temp=UA_Temp_Var;
    return UA_Temp_Var;
}

// UB 采样数据处理
float VoltageB_Process (void)
{
    unsigned int vol16;
    unsigned long vol32 = 0;
    float BVoltage_Sum=0;
    static signed int UB_Sample_Average[10]; 
    static unsigned char UB_Sample_Average_Cnt=0;
    static unsigned char UB_Sample_Average_Flag=0,UBoff_set_cnt = 100;
    static float UB_Temp=0;
    static float UB_Temp_Var=0;
    static unsigned char UB_Count=0; 
    static float UB_Temp_Offset=0;
    for(unsigned char i=0;i<SMPL_MAX;i++){
        vol32 += smpl.AD_value[1][i];
    }
    smpl.average[1] = vol32/SMPL_MAX;
    
    for(unsigned char i=0;i<SMPL_MAX;i++) {
        if(smpl.AD_value[1][i] > smpl.average[1]){
            vol16 = smpl.AD_value[1][i] - smpl.average[1];
        }
        else{
            vol16 = smpl.average[1] - smpl.AD_value[1][i];
        }
        BVoltage_Sum=BVoltage_Sum + (unsigned long)vol16*(unsigned long)vol16;
    }
    BVoltage_Sum=BVoltage_Sum/SMPL_MAX;
    BVoltage_Sum=sqrt(BVoltage_Sum);
     #ifdef REF_INTERNAL
        BVoltage_Sum=BVoltage_Sum*VOL_SMPL_SCALE*2.5;
        #else                   
        BVoltage_Sum=BVoltage_Sum*VOL_SMPL_SCALE*3;
     #endif  
    BVoltage_Sum=BVoltage_Sum/4096; 
    
   // return Voltage_Sum;

    if(fabs(UB_Temp_Var-BVoltage_Sum)>10)
        {
        UB_Sample_Average_Flag=0;
        UB_Sample_Average_Cnt=0;
        UB_Count=0;
        UBoff_set_cnt=100;
        UB_Temp_Offset=0;
        UB_Temp_Var=BVoltage_Sum;
        }
    else
        {
        if(UB_Sample_Average_Flag==0) 
            {
            UB_Temp = 0.5*(BVoltage_Sum+UB_Temp_Var);
            vol16 = (unsigned int)UB_Temp*10; 
            for(unsigned char i  =0; i < 10; i++)
                {
                UB_Sample_Average[i] = vol16;
                }
            UB_Sample_Average_Flag = 1;
            UBoff_set_cnt=100;
            UB_Temp_Offset=0;
            UB_Temp_Var=UB_Temp;
            }
        else
            {
            UB_Sample_Average[UB_Sample_Average_Cnt]=(unsigned int)BVoltage_Sum*10;
              UB_Sample_Average_Cnt++;
              if(UB_Sample_Average_Cnt>9)
                {
                    UB_Sample_Average_Cnt = 0;
                    }            
            UB_Temp=0;
            for(unsigned char j=0;j<10;j++)
            {
            UB_Temp=UB_Temp+UB_Sample_Average[j];
                }            
               UB_Temp /= 100;
               //UA_Temp = (UA_Temp + UA_Temp_Var)/2;
               if((fabs(UB_Temp-UB_Temp_Var)>0.01))
                {
                   if((fabs(UB_Temp-UB_Temp_Var)>2))
                    {
                    UB_Count++;
                    if(UB_Count>10)
                        {
                        UB_Sample_Average_Flag=0;
                        UB_Sample_Average_Cnt=0;
                        UB_Count=0;
                        }
                    UB_Temp_Var=UB_Temp_Var*0.95+UB_Temp*0.05;
                    UBoff_set_cnt=100;
                    UB_Temp_Offset=0;
                    }
                else
                    {
                    UB_Count=0;
                    if(UB_Temp > UB_Temp_Var)
                        {
                        UBoff_set_cnt++;
                        UB_Temp_Offset=UB_Temp_Offset+UB_Temp-UB_Temp_Var;
                        }
                    else if(UB_Temp < UB_Temp_Var)
                        {
                        UBoff_set_cnt--;
                        UB_Temp_Offset=UB_Temp_Offset+UB_Temp-UB_Temp_Var;
                        }
                    else
                        {
                        UBoff_set_cnt=100;
                        UB_Temp_Offset=0;
                        }
                    if(UBoff_set_cnt>=(100+OFFSETCOUNTER))
                        {
                        UB_Temp_Var=UB_Temp_Var+(UB_Temp_Offset)/(UBoff_set_cnt-100);
                        UBoff_set_cnt=100;
                        UB_Temp_Offset=0;
                        }
                    else if(UBoff_set_cnt<=(100-OFFSETCOUNTER))
                        {
                        UB_Temp_Var=UB_Temp_Var+(UB_Temp_Offset)/(100-UBoff_set_cnt);
                        UBoff_set_cnt=100;
                        UB_Temp_Offset=0;
                        }
                    }
                }
            else
                {
                UB_Temp_Var=UB_Temp;
                UB_Count=0;
                UBoff_set_cnt=100;
                UB_Temp_Offset=0;
                }
            }
        }
    //UA_Temp_Var=UA_Temp;
    //UA_Temp=UA_Temp_Var;
    return UB_Temp_Var;
}

// UC 采样数据处理
float VoltageC_Process (void)
{
      unsigned int vol16;
    unsigned long vol32 = 0;
    float CVoltage_Sum=0;
    static unsigned int UC_Sample_Average[10];
    static unsigned char UC_Sample_Average_Cnt=0;
    static unsigned char UC_Sample_Average_Flag=0,UCoff_set_cnt = 100;
    static float UC_Temp=0;
    static float UC_Temp_Var=0;
    static unsigned char UC_Count=0;  
    static float UC_Temp_Offset=0;
    for(unsigned char i=0;i<SMPL_MAX;i++){
        vol32 = vol32 + smpl.AD_value[2][i];
    }
    smpl.average[2] = vol32/SMPL_MAX;
    
    for(unsigned char i=0;i<SMPL_MAX;i++){
        if(smpl.AD_value[2][i]> smpl.average[2]){
            vol16 = smpl.AD_value[2][i] - smpl.average[2];
        }
        else{
            vol16 = smpl.average[2] - smpl.AD_value[2][i];
        }
        CVoltage_Sum=CVoltage_Sum+(unsigned long)vol16*(unsigned long)vol16;
    }
    CVoltage_Sum=CVoltage_Sum/SMPL_MAX;
    CVoltage_Sum=sqrt(CVoltage_Sum);
     #ifdef REF_INTERNAL
        CVoltage_Sum=CVoltage_Sum*VOL_SMPL_SCALE*2.5;
        #else                   
        CVoltage_Sum=CVoltage_Sum*VOL_SMPL_SCALE*3;
     #endif  
    CVoltage_Sum=CVoltage_Sum/4096; 
    
//    return Voltage_Sum;
    if(fabs(UC_Temp_Var-CVoltage_Sum)>10)
        {
        UC_Sample_Average_Flag=0;
        UC_Sample_Average_Cnt=0;
        UC_Count=0;
        UCoff_set_cnt=100;
        UC_Temp_Offset=0;
        UC_Temp_Var=CVoltage_Sum;
        }
    else
        {
        if(UC_Sample_Average_Flag==0) 
            {
            UC_Temp = 0.5*(CVoltage_Sum+UC_Temp_Var);
            vol16 = (unsigned int)UC_Temp*10; 
            for(unsigned char i  =0; i < 10; i++)
                {
                UC_Sample_Average[i] = vol16;
                }
            UC_Sample_Average_Flag = 1;
            UCoff_set_cnt=100;
            UC_Temp_Offset=0;
            UC_Temp_Var=UC_Temp;
            }
        else
            {
            UC_Sample_Average[UC_Sample_Average_Cnt]=(unsigned int)CVoltage_Sum*10;
              UC_Sample_Average_Cnt++;
              if(UC_Sample_Average_Cnt>9)
                {
                    UC_Sample_Average_Cnt = 0;
                    }            
            UC_Temp=0;
            for(unsigned char j=0;j<10;j++)
            {
            UC_Temp=UC_Temp+UC_Sample_Average[j];
                }            
               UC_Temp /= 100;
               //UC_Temp = (UC_Temp + UC_Temp_Var)/2;
               if((fabs(UC_Temp-UC_Temp_Var)>0.01))
                {
                   if((fabs(UC_Temp-UC_Temp_Var)>2))
                    {
                    UC_Count++;
                    if(UC_Count>10)
                        {
                        UC_Sample_Average_Flag=0;
                        UC_Sample_Average_Cnt=0;
                        UC_Count=0;
                        }
                    UC_Temp_Var=UC_Temp_Var*0.95+UC_Temp*0.05;
                    UCoff_set_cnt=100;
                    UC_Temp_Offset=0;
                    }
                else
                    {
                    UC_Count=0;
                    if(UC_Temp > UC_Temp_Var)
                        {
                        UCoff_set_cnt++;
                        UC_Temp_Offset=UC_Temp_Offset+UC_Temp-UC_Temp_Var;
                        }
                    else if(UC_Temp < UC_Temp_Var)
                        {
                        UCoff_set_cnt--;
                        UC_Temp_Offset=UC_Temp_Offset+UC_Temp-UC_Temp_Var;
                        }
                    else
                        {
                        UCoff_set_cnt=100;
                        UC_Temp_Offset=0;
                        }
                    if(UCoff_set_cnt>=(100+OFFSETCOUNTER))
                        {
                        UC_Temp_Var=UC_Temp_Var+(UC_Temp_Offset)/(UCoff_set_cnt-100);
                        UCoff_set_cnt=100;
                        UC_Temp_Offset=0;
                        }
                    else if(UCoff_set_cnt<=(100-OFFSETCOUNTER))
                        {
                        UC_Temp_Var=UC_Temp_Var+(UC_Temp_Offset)/(100-UCoff_set_cnt);
                        UCoff_set_cnt=100;
                        UC_Temp_Offset=0;
                        }
                    }
                }
            else
                {
                UC_Temp_Var=UC_Temp;
                UC_Count=0;
                UCoff_set_cnt=100;
                UC_Temp_Offset=0;
                }
            }
        }
    //UA_Temp_Var=UA_Temp;
    //UA_Temp=UA_Temp_Var;
    return UC_Temp_Var;
}

// IA 采样数据处理
float CurrentA_Process (void)
{
    unsigned int vol16;
    unsigned long vol32 = 0;
    float ACurrent_Sum=0;
    static unsigned int IA_Sample_Average[10];
    static unsigned char IA_Sample_Average_Cnt=0;
    static unsigned char IA_Sample_Average_Flag=0,IAoff_set_cnt = 100;
    static float IA_Temp=0;
    static float IA_Temp_Var=0;
    static unsigned char IA_Count=0;  
    static float IA_Temp_Offset=0;
    for(unsigned char i=0;i<SMPL_MAX;i++)
        {
        vol32 += smpl.AD_value[3][i];
        }
    smpl.average[3] = vol32/SMPL_MAX;
    for(unsigned char i=0;i<SMPL_MAX;i++)
        {
        if(smpl.AD_value[3][i]> smpl.average[3])
            {
            vol16 = smpl.AD_value[3][i] - smpl.average[3];
            }
        else
            {
            vol16 = smpl.average[3] - smpl.AD_value[3][i];
            }
        ACurrent_Sum = ACurrent_Sum + (unsigned long)vol16*(unsigned long)vol16;
        }
    ACurrent_Sum = ACurrent_Sum/SMPL_MAX;
    ACurrent_Sum=sqrt(ACurrent_Sum);
    #ifdef REF_INTERNAL
		ACurrent_Sum=ACurrent_Sum*(2.5*CUR_SMPL_SCALE);
	#else               	
		ACurrent_Sum=ACurrent_Sum*(3*CUR_SMPL_SCALE);
	#endif  
   		ACurrent_Sum=ACurrent_Sum/4096;
   // return ACurrent_Sum/110;    
    if(fabs(IA_Temp_Var-ACurrent_Sum)>22)
        {
        IA_Sample_Average_Flag=0;
        IA_Sample_Average_Cnt=0;
        IA_Count=0;
        IAoff_set_cnt=100;
        IA_Temp_Offset=0;
        IA_Temp_Var=ACurrent_Sum;
        }
    else
        {
        if(IA_Sample_Average_Flag==0) 
            {
            IA_Temp = 0.5*(ACurrent_Sum+IA_Temp_Var);
            vol16 = (unsigned int)IA_Temp*10; 
            for(unsigned char i  =0; i < 10; i++)
                {
                IA_Sample_Average[i] = vol16;
                }
            IA_Sample_Average_Flag = 1;
            IAoff_set_cnt=100;
            IA_Temp_Offset=0;
            IA_Temp_Var=IA_Temp;
            }
        else
            {
            IA_Sample_Average[IA_Sample_Average_Cnt]=(unsigned int)ACurrent_Sum*10;
              IA_Sample_Average_Cnt++;
              if(IA_Sample_Average_Cnt>9)
                {
                    IA_Sample_Average_Cnt = 0;
                    }            
            IA_Temp=0;
            for(unsigned char j=0;j<10;j++)
            {
            IA_Temp=IA_Temp+IA_Sample_Average[j];
                }            
               IA_Temp /= 100;
               IA_Temp = (IA_Temp + IA_Temp_Var)/2;
               if((fabs(IA_Temp-IA_Temp_Var)>0.11))
                {
                   if((fabs(IA_Temp-IA_Temp_Var)>1.1))
                    {
                    IA_Count++;
                    if(IA_Count>10)
                        {
                        IA_Sample_Average_Flag=0;
                        IA_Sample_Average_Cnt=0;
                        IA_Count=0;
                        }
                    if(IA_Temp>IA_Temp_Var)
                        IA_Temp_Var +=0.11;
                    else
                        IA_Temp_Var -=0.11;
                    //IA_Temp_Var=IA_Temp_Var*0.95+IA_Temp*0.05;
                    IAoff_set_cnt=100;
                    IA_Temp_Offset=0;
                    }
                else
                    {
                    IA_Count=0;
                    if(IA_Temp > IA_Temp_Var)
                        {
                        IAoff_set_cnt++;
                        IA_Temp_Offset=IA_Temp_Offset+IA_Temp-IA_Temp_Var;
                        }
                    else if(IA_Temp < IA_Temp_Var)
                        {
                        IAoff_set_cnt--;
                        IA_Temp_Offset=IA_Temp_Offset+IA_Temp-IA_Temp_Var;
                        }
                    else
                        {
                        IAoff_set_cnt=100;
                        IA_Temp_Offset=0;
                        }
                    if(IAoff_set_cnt>=(100+OFFSETCOUNTER))
                        {
                        IA_Temp_Var=IA_Temp_Var+(IA_Temp_Offset)/(IAoff_set_cnt-100);
                        IAoff_set_cnt=100;
                        IA_Temp_Offset=0;
                        }
                    else if(IAoff_set_cnt<=(100-OFFSETCOUNTER))
                        {
                        IA_Temp_Var=IA_Temp_Var+(IA_Temp_Offset)/(100-IAoff_set_cnt);
                        IAoff_set_cnt=100;
                        IA_Temp_Offset=0;
                        }
                    }
                }
            else
                {
                IA_Temp_Var=IA_Temp;
                IA_Count=0;
                IAoff_set_cnt=100;
                IA_Temp_Offset=0;
                }
            }
        }
    return (IA_Temp_Var/110);
}

// IB
float CurrentB_Process (void)
{
    unsigned int vol16;
    unsigned long vol32 = 0;
    float BCurrent_Sum=0;
    static unsigned int IB_Sample_Average[10];
    static unsigned char IB_Sample_Average_Cnt=0;
    static unsigned char IB_Sample_Average_Flag=0,IBoff_set_cnt = 100;
    static float IB_Temp=0;
    static float IB_Temp_Var=0;
    static unsigned char IB_Count=0;  
    static float IB_Temp_Offset=0;
    for(unsigned char i=0;i<SMPL_MAX;i++)
        {
        vol32 += smpl.AD_value[4][i];
        }
    smpl.average[4] = vol32/SMPL_MAX;
    for(unsigned char i=0;i<SMPL_MAX;i++)
        {
        if(smpl.AD_value[4][i]> smpl.average[4])
            {
            vol16 = smpl.AD_value[4][i] - smpl.average[4];
            }
        else
            {
            vol16 = smpl.average[4] - smpl.AD_value[4][i];
            }
        BCurrent_Sum = BCurrent_Sum + (unsigned long)vol16*(unsigned long)vol16;
        }
    BCurrent_Sum = BCurrent_Sum/SMPL_MAX;
    BCurrent_Sum=sqrt(BCurrent_Sum);
    #ifdef REF_INTERNAL
		BCurrent_Sum=BCurrent_Sum*2.5*CUR_SMPL_SCALE;
	#else               	
		BCurrent_Sum=BCurrent_Sum*3*CUR_SMPL_SCALE;
	#endif  
		BCurrent_Sum=BCurrent_Sum/4096;     
//    return Voltage_Sum;    
    if(fabs(IB_Temp_Var-BCurrent_Sum)>22)
        {
        IB_Sample_Average_Flag=0;
        IB_Sample_Average_Cnt=0;
        IB_Count=0;
        IBoff_set_cnt=100;
        IB_Temp_Offset=0;
        IB_Temp_Var=BCurrent_Sum;
        }
    else
        {
        if(IB_Sample_Average_Flag==0) 
            {
            IB_Temp = 0.5*(BCurrent_Sum+IB_Temp_Var);
            vol16 = (unsigned int)IB_Temp*10; 
            for(unsigned char i  =0; i < 10; i++)
                {
                IB_Sample_Average[i] = vol16;
                }
            IB_Sample_Average_Flag = 1;
            IBoff_set_cnt=100;
            IB_Temp_Offset=0;
            IB_Temp_Var=IB_Temp;
            }
        else
            {
            IB_Sample_Average[IB_Sample_Average_Cnt]=(unsigned int)BCurrent_Sum*10;
              IB_Sample_Average_Cnt++;
              if(IB_Sample_Average_Cnt>9)
                {
                    IB_Sample_Average_Cnt = 0;
                    }            
            IB_Temp=0;
            for(unsigned char j=0;j<10;j++)
            {
            IB_Temp=IB_Temp+IB_Sample_Average[j];
                }            
               IB_Temp /= 100;
               //UA_Temp = (UA_Temp + UA_Temp_Var)/2;
               if((fabs(IB_Temp-IB_Temp_Var)>0.11))
                {
                   if((fabs(IB_Temp-IB_Temp_Var)>1.1))
                    {
                    IB_Count++;
                    if(IB_Count>10)
                        {
                        IB_Sample_Average_Flag=0;
                        IB_Sample_Average_Cnt=0;
                        IB_Count=0;
                        }
                    if(IB_Temp>IB_Temp_Var)
                        IB_Temp_Var +=0.11;
                    else
                        IB_Temp_Var -=0.11;
                    //IB_Temp_Var=IB_Temp_Var*0.95+IB_Temp*0.05;
                    IBoff_set_cnt=100;
                    IB_Temp_Offset=0;
                    }
                else
                    {
                    IB_Count=0;
                    if(IB_Temp > IB_Temp_Var)
                        {
                        IBoff_set_cnt++;
                        IB_Temp_Offset=IB_Temp_Offset+IB_Temp-IB_Temp_Var;
                        }
                    else if(IB_Temp < IB_Temp_Var)
                        {
                        IBoff_set_cnt--;
                        IB_Temp_Offset=IB_Temp_Offset+IB_Temp-IB_Temp_Var;
                        }
                    else
                        {
                        IBoff_set_cnt=100;
                        IB_Temp_Offset=0;
                        }
                    if(IBoff_set_cnt>=(100+OFFSETCOUNTER))
                        {
                        IB_Temp_Var=IB_Temp_Var+(IB_Temp_Offset)/(IBoff_set_cnt-100);
                        IBoff_set_cnt=100;
                        IB_Temp_Offset=0;
                        }
                    else if(IBoff_set_cnt<=(100-OFFSETCOUNTER))
                        {
                        IB_Temp_Var=IB_Temp_Var+(IB_Temp_Offset)/(100-IBoff_set_cnt);
                        IBoff_set_cnt=100;
                        IB_Temp_Offset=0;
                        }
                    }
                }
            else
                {
                IB_Temp_Var=IB_Temp;
                IB_Count=0;
                IBoff_set_cnt=100;
                IB_Temp_Offset=0;
                }
            }
        }
    return (IB_Temp_Var/110);
}

// IC
float CurrentC_Process (void)
{
    unsigned int vol16;
    unsigned long vol32 = 0;
    float CCurrent_Sum=0;
    static unsigned int IC_Sample_Average[10];
    static unsigned char IC_Sample_Average_Cnt=0;
    static unsigned char IC_Sample_Average_Flag=0,ICoff_set_cnt = 100;
    static float IC_Temp=0;
    static float IC_Temp_Var=0;
    static unsigned char IC_Count=0;  
    static float IC_Temp_Offset=0;
    for(unsigned char i=0;i<SMPL_MAX;i++)
        {
        vol32 += smpl.AD_value[5][i];
        }
    smpl.average[5] = vol32/SMPL_MAX;
    for(unsigned char i=0;i<SMPL_MAX;i++)
        {
        if(smpl.AD_value[5][i]> smpl.average[5])
            {
            vol16 = smpl.AD_value[5][i] - smpl.average[5];
            }
        else
            {
            vol16 = smpl.average[5] - smpl.AD_value[5][i];
            }
        CCurrent_Sum = CCurrent_Sum + (unsigned long)vol16*(unsigned long)vol16;
        }
    CCurrent_Sum = CCurrent_Sum/SMPL_MAX;
    CCurrent_Sum=sqrt(CCurrent_Sum);
    #ifdef REF_INTERNAL
		CCurrent_Sum=CCurrent_Sum*2.5*CUR_SMPL_SCALE;
	#else               	
		CCurrent_Sum=CCurrent_Sum*3*CUR_SMPL_SCALE;
	#endif  
		CCurrent_Sum=CCurrent_Sum/4096;
    if(fabs(IC_Temp_Var-CCurrent_Sum)>22)
        {
        IC_Sample_Average_Flag=0;
        IC_Sample_Average_Cnt=0;
        IC_Count=0;
        ICoff_set_cnt=100;
        IC_Temp_Offset=0;
        IC_Temp_Var=CCurrent_Sum;
        }
    else
        {
        if(IC_Sample_Average_Flag==0) 
            {
            IC_Temp = 0.5*(CCurrent_Sum+IC_Temp_Var);
            vol16 = (unsigned int)IC_Temp*10; 
            for(unsigned char i  =0; i < 10; i++)
                {
                IC_Sample_Average[i] = vol16;
                }
            IC_Sample_Average_Flag = 1;
            ICoff_set_cnt=100;
            IC_Temp_Offset=0;
            IC_Temp_Var=IC_Temp;
            }
        else
            {
            IC_Sample_Average[IC_Sample_Average_Cnt]=(unsigned int)CCurrent_Sum*10;
              IC_Sample_Average_Cnt++;
              if(IC_Sample_Average_Cnt>9)
                {
                    IC_Sample_Average_Cnt = 0;
                    }            
            IC_Temp=0;
            for(unsigned char j=0;j<10;j++)
            {
            IC_Temp=IC_Temp+IC_Sample_Average[j];
                }            
               IC_Temp /= 100;
               //UA_Temp = (UA_Temp + UA_Temp_Var)/2;
               if((fabs(IC_Temp-IC_Temp_Var)>0.11))
                {
                   if((fabs(IC_Temp-IC_Temp_Var)>1.1))
                    {
                    IC_Count++;
                    if(IC_Count>10)
                        {
                        IC_Sample_Average_Flag=0;
                        IC_Sample_Average_Cnt=0;
                        IC_Count=0;
                        }
                    if(IC_Temp>IC_Temp_Var)
                        IC_Temp_Var +=0.11;
                    else
                        IC_Temp_Var -=0.11;
                    //IC_Temp_Var=IC_Temp_Var*0.95+IC_Temp*0.05;
                    ICoff_set_cnt=100;
                    IC_Temp_Offset=0;
                    }
                else
                    {
                    IC_Count=0;
                    if(IC_Temp > IC_Temp_Var)
                        {
                        ICoff_set_cnt++;
                        IC_Temp_Offset=IC_Temp_Offset+IC_Temp-IC_Temp_Var;
                        }
                    else if(IC_Temp < IC_Temp_Var)
                        {
                        ICoff_set_cnt--;
                        IC_Temp_Offset=IC_Temp_Offset+IC_Temp-IC_Temp_Var;
                        }
                    else
                        {
                        ICoff_set_cnt=100;
                        IC_Temp_Offset=0;
                        }
                    if(ICoff_set_cnt>=(100+OFFSETCOUNTER))
                        {
                        IC_Temp_Var=IC_Temp_Var+(IC_Temp_Offset)/(ICoff_set_cnt-100);
                        ICoff_set_cnt=100;
                        IC_Temp_Offset=0;
                        }
                    else if(ICoff_set_cnt<=(100-OFFSETCOUNTER))
                        {
                        IC_Temp_Var=IC_Temp_Var+(IC_Temp_Offset)/(100-ICoff_set_cnt);
                        ICoff_set_cnt=100;
                        IC_Temp_Offset=0;
                        }
                    }
                }
            else
                {
                IC_Temp_Var=IC_Temp;
                IC_Count=0;
                ICoff_set_cnt=100;
                IC_Temp_Offset=0;
                }
            }
        }
    return (IC_Temp_Var/110);
}

// In 采样数据处理(电流零序)
float CurrentN_Process (void)
{
    unsigned int tmp16,IN_Temp_AD_value[SMPL_MAX];
    unsigned long tmp32 = 0;
    float Current_Sum=0;
    static unsigned int IN_Sample_Average[10];
    static unsigned char IN_Sample_Average_Cnt=0;
    static unsigned char IN_Sample_Average_Flag=0,off_set_cnt = 0;
    static float IN_Temp_Var=0;
    static unsigned char IN_Count=0;
    static float IN_Temp=0;
    for(unsigned char i=0;i<SMPL_MAX;i++)
    {
      IN_Temp_AD_value[i]=(smpl.AD_value[3][i]+smpl.AD_value[4][i]+smpl.AD_value[5][i]);
    }
    smpl.average[6]=smpl.average[3]+smpl.average[4]+smpl.average[5];
    tmp32 = 0;
    for(unsigned char i=0;i<SMPL_MAX;i++){
        if(IN_Temp_AD_value[i] > smpl.average[6]){
            tmp16 = IN_Temp_AD_value[i] - smpl.average[6];
        }
        else{
            tmp16 = smpl.average[6] - IN_Temp_AD_value[i];
        }
        tmp32 = tmp32 + (unsigned long)tmp16*(unsigned long)tmp16;
    }                       

    Current_Sum=(float)tmp32/SMPL_MAX;
    Current_Sum=sqrt(Current_Sum);
    Current_Sum=Current_Sum/110;        //电流输出外接电阻为110欧
     #ifdef REF_INTERNAL
        Current_Sum=Current_Sum*CUR_SMPL_SCALE*2.5;
        #else                   
        Current_Sum=Current_Sum*CUR_SMPL_SCALE*3;
     #endif  
    Current_Sum=Current_Sum/4096;
  
    if(fabs(IN_Temp_Var-Current_Sum)>0.2){    //大于10直接过
        IN_Sample_Average_Flag=0;
        IN_Sample_Average_Cnt=0;
        IN_Count=0;
        IN_Temp=Current_Sum;
    }
    else{
        if(IN_Sample_Average_Flag == 0){
            IN_Temp = (IN_Temp_Var+Current_Sum)/2;
            tmp16 = (unsigned int)IN_Temp*1000;
            for(unsigned char i = 0; i < 10; i++){
                IN_Sample_Average[i] = tmp16;
            }
            IN_Sample_Average_Flag = 1;off_set_cnt = 0;
        }
        else if(IN_Sample_Average_Flag == 1){
            IN_Sample_Average[IN_Sample_Average_Cnt]=(unsigned int)Current_Sum*1000;
            IN_Sample_Average_Cnt++;
            if(IN_Sample_Average_Cnt>9){
                IN_Sample_Average_Cnt = 0;
            }            
            tmp32 = 0;
            for(unsigned char j=0;j<10;j++){
                tmp32 = tmp32 + IN_Sample_Average[j];
            }            
               IN_Temp = (float)tmp32/10000;
               IN_Temp = (IN_Temp + IN_Temp_Var)/2;
               if((fabs(IN_Temp-IN_Temp_Var)>0.01)){
                   if((fabs(IN_Temp-IN_Temp_Var)>0.03)){
                    IN_Count++;
                    if(IN_Count>10){
                        IN_Sample_Average_Flag=0;
                        IN_Sample_Average_Cnt=0;
                        IN_Count=0;
                    }
                }else{
                    IN_Count=0;
                }
                if(off_set_cnt < 10) off_set_cnt++;
                IN_Temp=IN_Temp_Var+(IN_Temp-IN_Temp_Var)/off_set_cnt;
            }
            else{
                IN_Count=0;
            }
        }            
    }
    
    IN_Temp_Var=IN_Temp;
    return(IN_Temp);
}

/*===========================================================================
u_smpl,i_smpl:电压,电流采样序列,u_average,iaverage:电压,电流直流分量
==========================================================================*/
float ui_deal (unsigned int *u_smpl,unsigned int *i_smpl,
	long u_average,long i_average)
{
    long ui[SMPL_MAX+1],tmp32 = 0;
    float ui_average = 0;
    for(unsigned char i=0;i<SMPL_MAX;i++){
        ui[i] = ((long)(*u_smpl)-u_average)*((long)(*i_smpl)-i_average);       
        tmp32 += ui[i]; 
        u_smpl++,i_smpl++;
    }
    ui_average = (float)tmp32 / SMPL_MAX;
	 #ifdef REF_INTERNAL
		ui_average=ui_average*VOL_SMPL_SCALE*2.5/4096;	
		ui_average=ui_average*CUR_SMPL_SCALE*2.5/110;
        #else               	
	ui_average=ui_average*VOL_SMPL_SCALE*3/4096;	
	ui_average=ui_average*CUR_SMPL_SCALE*3/110;
	 #endif  
    return(ui_average /= 4096);    
}

float Pa_deal (void)
{
    static unsigned char ui_cnt = 0,ui_flg = 0,off_set_cnt = 0;
    static float ui_buf[6],last_ui = 0;
    float ui_average = 0;  
    
    ui_average = fabs(ui_deal(smpl.AD_value[0],smpl.AD_value[3],smpl.average[0],smpl.average[3]));     
    if(fabs(ui_average-last_ui) > 40){
        ui_cnt = 0;ui_flg = 0;
    }
    else{
        if(ui_flg == 0){
            for(unsigned char i=0;i<6;i++){
                ui_buf[i] = ui_average;
                 }
            ui_flg = 1;off_set_cnt = 0;
            }
        else{
            if(ui_cnt < 6) ui_cnt++;
            else ui_cnt = 0;
            ui_buf[ui_cnt] = ui_average;
            ui_average = 0;
            for(unsigned char i=0;i<6;i++){
                ui_average += ui_buf[i];
                }
            ui_average = ui_average/6;
            if(off_set_cnt < 10) off_set_cnt++;
            ui_average = last_ui + (ui_average-last_ui)/off_set_cnt;
        }
    }
    last_ui = ui_average;
    return (last_ui);
}

float Pb_deal (void)
{
    static unsigned char ui_cnt = 0,ui_flg = 0,off_set_cnt = 0;
    static float ui_buf[6],last_ui = 0;
    float ui_average = 0; 

    ui_average = fabs(ui_deal(smpl.AD_value[1],smpl.AD_value[4],smpl.average[1],smpl.average[4]));
    if(fabs(ui_average-last_ui) > 40){
        ui_cnt = 0;ui_flg = 0;
    }
    else{
        if(ui_flg == 0){
            for(unsigned char i=0;i<6;i++){
                ui_buf[i] = ui_average;
                 }
            ui_flg = 1;off_set_cnt = 0;
            }
        else{
            if(ui_cnt < 6) ui_cnt++;
            else ui_cnt = 0;
            ui_buf[ui_cnt] = ui_average;
            ui_average = 0;
            for(unsigned char i=0;i<6;i++){
                ui_average += ui_buf[i];
                }
            ui_average = ui_average/6;
            if(off_set_cnt < 10) off_set_cnt++;
            ui_average = last_ui + (ui_average-last_ui)/off_set_cnt;
        }
    }
    last_ui = ui_average;
    return (last_ui);
}

float Pc_deal(void)
{
    static unsigned char ui_cnt = 0,ui_flg = 0,off_set_cnt = 0;
    static float ui_buf[6],last_ui = 0;
    float ui_average = 0;

    ui_average = fabs(ui_deal(smpl.AD_value[2],smpl.AD_value[5],smpl.average[2],smpl.average[5]));
    if(fabs(ui_average-last_ui) > 40){
        ui_cnt = 0;ui_flg = 0;
    }
    else{
        if(ui_flg == 0){
            for(unsigned char i=0;i<6;i++){
                ui_buf[i] = ui_average;
                 }
            ui_flg = 1;off_set_cnt = 0;
            }
        else{
            if(ui_cnt < 6) ui_cnt++;
            else ui_cnt = 0;
            ui_buf[ui_cnt] = ui_average;
            ui_average = 0;
            for(unsigned char i=0;i<6;i++){
                ui_average += ui_buf[i];
                }
            ui_average = ui_average/6;
            if(off_set_cnt < 10) off_set_cnt++;
            ui_average = last_ui + (ui_average-last_ui)/off_set_cnt;
        }
    }
    last_ui = ui_average;
    return (last_ui);
}


