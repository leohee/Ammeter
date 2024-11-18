#include <msp430x14x.h>
#include "ad.h"
#include "main.h"

void Adc12Init (void)
{
	P6SEL|=UA+UB+UC+IA+IB+IC;//P6.0~P6.5 as Analog 
	P6DIR&=~(UA+UB+UC+IA+IB+IC);//P6.0~P6.5 as Input
	ADC12CTL0&=~ENC;

#ifdef REF_INTERNAL
	//AD Sample Frequcey equal 1M/4=250K
	ADC12CTL0=MSC+REFON+REF2_5V+SHT1_0+SHT0_1;
#else
	//AD Sample Frequcey equal 1M/4=250K
	ADC12CTL0=MSC+SHT1_0+SHT0_1;
#endif

	// ADC12 Clk Select From SMCLK and divide by 8,Fadclk=8M/8=1M //多通道单次转换
	ADC12CTL1=CSTARTADD_0+SHS_0+SHP+ADC12DIV_1+ADC12SSEL_2+CONSEQ_1;
  
}
void ADC12Channel (void)
{
#ifdef REF_INTERNAL
	ADC12MCTL0|=INCH_0+SREF_1;
	ADC12MCTL1|=INCH_1+SREF_5;
	ADC12MCTL2|=INCH_2+SREF_1;
	ADC12MCTL3|=INCH_3+SREF_5;
	ADC12MCTL4|=INCH_4+SREF_1;
	ADC12MCTL5|=INCH_5+SREF_5+EOS;//转换通道结束处
#else  
	ADC12MCTL0|=INCH_0+SREF_2;
	ADC12MCTL1|=INCH_1+SREF_2;
	ADC12MCTL2|=INCH_2+SREF_2;
	ADC12MCTL3|=INCH_3+SREF_2;
	ADC12MCTL4|=INCH_4+SREF_2;
	ADC12MCTL5|=INCH_5+SREF_2+EOS;
#endif  
}

void Adc12Open (void)
{
	ADC12CTL0|=ADC12ON;
	ADC12CTL0|=ENC;//允许转换
}
void Adc12Close (void)
{
	ADC12CTL0&=~ADC12ON;
	//ADC12CTL0&=~ENC;//不允许转换
}

void Adc12Start (void)
{
	ADC12CTL0|=ENC+ADC12SC;//转换开始
}

void ADC12Int (void)
{
	ADC12IE|=0x10;//通道5AD转换完成后产生中断
}


