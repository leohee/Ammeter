#ifndef AD_H
#define AD_H

#define UA  BIT0
#define UB  BIT1
#define UC  BIT2
#define IA  BIT3
#define IB  BIT4
#define IC  BIT5

extern void Adc12Init (void);
extern void ADC12Channel (void);
extern void Adc12Open (void);
extern void Adc12Close (void);
extern void Adc12Start (void);
extern void ADC12Int (void);

#endif

