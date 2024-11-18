#ifndef LED_H
#define LED_H

#define LED_RUN BIT0

#define LED_RUN_ON  P3OUT|=LED_RUN
#define LED_RUN_OFF P3OUT&=~LED_RUN

extern void LED_RUN_Init (void);


#endif

