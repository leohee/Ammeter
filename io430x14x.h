

#ifndef __IO430X14X
#define __IO430X14X

#ifdef  __IAR_SYSTEMS_ICC__
#ifndef _SYSTEM_BUILD
#pragma system_include
#endif
#endif

#if (((__TID__ >> 8) & 0x7F) != 0x2b)     /* 0x2b = 43 dec */
#error io430x14x.h file for use with ICC430/A430 only
#endif

 
#ifdef __IAR_SYSTEMS_ICC__
#pragma language=extended
#pragma system_include

#ifdef __cplusplus
#define __READ    /* not supported */
#else
#define __READ    const
#endif


/*-------------------------------------------------------------------------
 *   Standard Bits
 *-------------------------------------------------------------------------*/

#define BIT0                (0x0001)
#define BIT1                (0x0002)
#define BIT2                (0x0004)
#define BIT3                (0x0008)
#define BIT4                (0x0010)
#define BIT5                (0x0020)
#define BIT6                (0x0040)
#define BIT7                (0x0080)
#define BIT8                (0x0100)
#define BIT9                (0x0200)
#define BITA                (0x0400)
#define BITB                (0x0800)
#define BITC                (0x1000)
#define BITD                (0x2000)
#define BITE                (0x4000)
#define BITF                (0x8000)

/*-------------------------------------------------------------------------
 *   Status register bits
 *-------------------------------------------------------------------------*/

#define C                   (0x0001)
#define Z                   (0x0002)
#define N                   (0x0004)
#define V                   (0x0100)
#define GIE                 (0x0008)
#define CPUOFF              (0x0010)
#define OSCOFF              (0x0020)
#define SCG0                (0x0040)
#define SCG1                (0x0080)

/* Low Power Modes coded with Bits 4-7 in SR */

#define LPM0_bits           (CPUOFF)
#define LPM1_bits           (SCG0+CPUOFF)
#define LPM2_bits           (SCG1+CPUOFF)
#define LPM3_bits           (SCG1+SCG0+CPUOFF)
#define LPM4_bits           (SCG1+SCG0+OSCOFF+CPUOFF)


#define LPM0      _BIS_SR(LPM0_bits)     /* Enter Low Power Mode 0 */
#define LPM0_EXIT _BIC_SR_IRQ(LPM0_bits) /* Exit Low Power Mode 0 */
#define LPM1      _BIS_SR(LPM1_bits)     /* Enter Low Power Mode 1 */
#define LPM1_EXIT _BIC_SR_IRQ(LPM1_bits) /* Exit Low Power Mode 1 */
#define LPM2      _BIS_SR(LPM2_bits)     /* Enter Low Power Mode 2 */
#define LPM2_EXIT _BIC_SR_IRQ(LPM2_bits) /* Exit Low Power Mode 2 */
#define LPM3      _BIS_SR(LPM3_bits)     /* Enter Low Power Mode 3 */
#define LPM3_EXIT _BIC_SR_IRQ(LPM3_bits) /* Exit Low Power Mode 3 */
#define LPM4      _BIS_SR(LPM4_bits)     /* Enter Low Power Mode 4 */
#define LPM4_EXIT _BIC_SR_IRQ(LPM4_bits) /* Exit Low Power Mode 4 */



/*-------------------------------------------------------------------------
 *   Special Function
 *-------------------------------------------------------------------------*/


__no_init volatile union
{
  unsigned char IE1;  /* Interrupt Enable 1 */ 
  
  struct
  {
    unsigned char WDTIE          : 1; 
    unsigned char OFIE           : 1; 
    unsigned char                : 2; 
    unsigned char NMIIE          : 1; 
    unsigned char ACCVIE         : 1; 
    unsigned char URXIE0         : 1; 
    unsigned char UTXIE0         : 1; 
  } IE1_bit;  
} @ 0x0000;

#define U0IE IE1  /* UART0 Interrupt Enable Register */ 

enum {
  WDTIE               = 0x0001,
  OFIE                = 0x0002,
  NMIIE               = 0x0010,
  ACCVIE              = 0x0020,
  URXIE0              = 0x0040,
  UTXIE0              = 0x0080,
};


__no_init volatile union
{
  unsigned char IFG1;  /* Interrupt Flag 1 */ 
  
  struct
  {
    unsigned char WDTIFG         : 1; 
    unsigned char OFIFG          : 1; 
    unsigned char                : 2; 
    unsigned char NMIIFG         : 1; 
    unsigned char                : 1; 
    unsigned char URXIFG0        : 1; 
    unsigned char UTXIFG0        : 1; 
  } IFG1_bit;  
} @ 0x0002;

#define U0IFG IFG1  /* UART0 Interrupt Flag Register */ 

enum {
  WDTIFG              = 0x0001,
  OFIFG               = 0x0002,
  NMIIFG              = 0x0010,
  URXIFG0             = 0x0040,
  UTXIFG0             = 0x0080,
};


__no_init volatile union
{
  unsigned char ME1;  /* Module Enable 1 */ 
  
  union
  {
    struct
    {
      unsigned char                : 6; 
      unsigned char URXE0          : 1; 
      unsigned char UTXE0          : 1; 
    };
    struct
    {
      unsigned char                : 6; 
      unsigned char USPIE0         : 1; 
    }; 
  } ME1_bit;
} @ 0x0004;

#define U0ME ME1  /* UART0 Module Enable Register */ 

enum {
  URXE0               = 0x0040,
  UTXE0               = 0x0080,
  USPIE0              = 0x0040,
};


__no_init volatile union
{
  unsigned char IE2;  /* Interrupt Enable 2 */ 
  
  struct
  {
    unsigned char                : 4; 
    unsigned char URXIE1         : 1; 
    unsigned char UTXIE1         : 1; 
    unsigned char                : 2; 
  } IE2_bit;  
} @ 0x0001;

#define U1IE IE2  /* UART1 Interrupt Enable Register */ 

enum {
  URXIE1              = 0x0010,
  UTXIE1              = 0x0020,
};


__no_init volatile union
{
  unsigned char IFG2;  /* Interrupt Flag 2 */ 
  
  struct
  {
    unsigned char                : 4; 
    unsigned char URXIFG1        : 1; 
    unsigned char UTXIFG1        : 1; 
    unsigned char                : 2; 
  } IFG2_bit;  
} @ 0x0003;

#define U1IFG IFG2  /* UART1 Interrupt Flag Register */ 

enum {
  URXIFG1             = 0x0010,
  UTXIFG1             = 0x0020,
};


__no_init volatile union
{
  unsigned char ME2;  /* Module Enable 2 */ 
  
  union
  {
    struct
    {
      unsigned char                : 4; 
      unsigned char URXE1          : 1; 
      unsigned char UTXE1          : 1; 
    };
    struct
    {
      unsigned char                : 4; 
      unsigned char USPIE1         : 1; 
    }; 
  } ME2_bit;
} @ 0x0005;

#define U1ME ME2  /* UART1 Module Enable Register */ 

enum {
  URXE1               = 0x0010,
  UTXE1               = 0x0020,
  USPIE1              = 0x0010,
};



/*-------------------------------------------------------------------------
 *   Watchdog Timer
 *-------------------------------------------------------------------------*/


__no_init volatile union
{
  unsigned short WDTCTL;  /* Watchdog Timer Control */ 
  
  struct
  {
    unsigned short WDTIS0        : 1; 
    unsigned short WDTIS1        : 1; 
    unsigned short WDTSSEL       : 1; 
    unsigned short WDTCNTCL      : 1; 
    unsigned short WDTTMSEL      : 1; 
    unsigned short WDTNMI        : 1; 
    unsigned short WDTNMIES      : 1; 
    unsigned short WDTHOLD       : 1; 
    unsigned short               : 8; 
  } WDTCTL_bit;  
} @ 0x0120;


enum {
  WDTIS0              = 0x0001,
  WDTIS1              = 0x0002,
  WDTSSEL             = 0x0004,
  WDTCNTCL            = 0x0008,
  WDTTMSEL            = 0x0010,
  WDTNMI              = 0x0020,
  WDTNMIES            = 0x0040,
  WDTHOLD             = 0x0080,
};

 
#define __MSP430_HAS_WDT__            /* Definition to show that Module is available */
    
#define WDTPW               (0x5A00)
  
/* WDT-interval times [1ms] coded with Bits 0-2 */
/* WDT is clocked by fSMCLK (assumed 1MHz) */
#define WDT_MDLY_32         (WDTPW+WDTTMSEL+WDTCNTCL)                         /* 32ms interval (default) */
#define WDT_MDLY_8          (WDTPW+WDTTMSEL+WDTCNTCL+WDTIS0)                  /* 8ms      */
#define WDT_MDLY_0_5        (WDTPW+WDTTMSEL+WDTCNTCL+WDTIS1)                  /* 0.5ms    */
#define WDT_MDLY_0_064      (WDTPW+WDTTMSEL+WDTCNTCL+WDTIS1+WDTIS0)           /* 0.064ms  */
/* WDT is clocked by fACLK (assumed 32KHz) */
#define WDT_ADLY_1000       (WDTPW+WDTTMSEL+WDTCNTCL+WDTSSEL)                 /* 1000ms   */
#define WDT_ADLY_250        (WDTPW+WDTTMSEL+WDTCNTCL+WDTSSEL+WDTIS0)          /* 250ms    */
#define WDT_ADLY_16         (WDTPW+WDTTMSEL+WDTCNTCL+WDTSSEL+WDTIS1)          /* 16ms     */
#define WDT_ADLY_1_9        (WDTPW+WDTTMSEL+WDTCNTCL+WDTSSEL+WDTIS1+WDTIS0)   /* 1.9ms    */
/* Watchdog mode -> reset after expired time */
/* WDT is clocked by fSMCLK (assumed 1MHz) */
#define WDT_MRST_32         (WDTPW+WDTCNTCL)                                  /* 32ms interval (default) */
#define WDT_MRST_8          (WDTPW+WDTCNTCL+WDTIS0)                           /* 8ms      */
#define WDT_MRST_0_5        (WDTPW+WDTCNTCL+WDTIS1)                           /* 0.5ms    */
#define WDT_MRST_0_064      (WDTPW+WDTCNTCL+WDTIS1+WDTIS0)                    /* 0.064ms  */
/* WDT is clocked by fACLK (assumed 32KHz) */
#define WDT_ARST_1000       (WDTPW+WDTCNTCL+WDTSSEL)                          /* 1000ms   */
#define WDT_ARST_250        (WDTPW+WDTCNTCL+WDTSSEL+WDTIS0)                   /* 250ms    */
#define WDT_ARST_16         (WDTPW+WDTCNTCL+WDTSSEL+WDTIS1)                   /* 16ms     */
#define WDT_ARST_1_9        (WDTPW+WDTCNTCL+WDTSSEL+WDTIS1+WDTIS0)            /* 1.9ms    */
  
/* INTERRUPT CONTROL */
/* These two bits are defined in the Special Function Registers */
/* #define WDTIE               0x01 */
/* #define WDTIFG              0x01 */
 

/*-------------------------------------------------------------------------
 *   Hardware Multiplier
 *-------------------------------------------------------------------------*/


 /* Multiply Unsigned/Operand 1 */ 
__no_init volatile unsigned short MPY @ 0x0130;  
 

 /* Multiply Signed/Operand 1 */ 
__no_init volatile unsigned short MPYS @ 0x0132;  
 

 /* Multiply Unsigned and Accumulate/Operand 1 */ 
__no_init volatile unsigned short MAC @ 0x0134;  
 

 /* Multiply Signed and Accumulate/Operand 1 */ 
__no_init volatile unsigned short MACS @ 0x0136;  
 

 /* Operand 2 */ 
__no_init volatile unsigned short OP2 @ 0x0138;  
 

 /* Result Low Word */ 
__no_init volatile unsigned short RESLO @ 0x013A;  
 

 /* Result High Word */ 
__no_init volatile unsigned short RESHI @ 0x013C;  
 

 /* Sum Extend */ 
__no_init volatile unsigned __READ short SUMEXT @ 0x013E;  
 
 
#define __MSP430_HAS_MPY__            /* Definition to show that Module is available */
 

/*-------------------------------------------------------------------------
 *   Digital I/O Port1/2
 *-------------------------------------------------------------------------*/


__no_init volatile union
{
  unsigned __READ char P1IN;  /* Port 1 Input */ 
  
  struct
  {
    unsigned __READ char P1IN_0  : 1; 
    unsigned __READ char P1IN_1  : 1; 
    unsigned __READ char P1IN_2  : 1; 
    unsigned __READ char P1IN_3  : 1; 
    unsigned __READ char P1IN_4  : 1; 
    unsigned __READ char P1IN_5  : 1; 
    unsigned __READ char P1IN_6  : 1; 
    unsigned __READ char P1IN_7  : 1; 
  } P1IN_bit;  
} @ 0x0020;


enum {
  P1IN_0              = 0x0001,
  P1IN_1              = 0x0002,
  P1IN_2              = 0x0004,
  P1IN_3              = 0x0008,
  P1IN_4              = 0x0010,
  P1IN_5              = 0x0020,
  P1IN_6              = 0x0040,
  P1IN_7              = 0x0080,
};


__no_init volatile union
{
  unsigned char P1OUT;  /* Port 1 Output */ 
  
  struct
  {
    unsigned char P1OUT_0        : 1; 
    unsigned char P1OUT_1        : 1; 
    unsigned char P1OUT_2        : 1; 
    unsigned char P1OUT_3        : 1; 
    unsigned char P1OUT_4        : 1; 
    unsigned char P1OUT_5        : 1; 
    unsigned char P1OUT_6        : 1; 
    unsigned char P1OUT_7        : 1; 
  } P1OUT_bit;  
} @ 0x0021;


enum {
  P1OUT_0             = 0x0001,
  P1OUT_1             = 0x0002,
  P1OUT_2             = 0x0004,
  P1OUT_3             = 0x0008,
  P1OUT_4             = 0x0010,
  P1OUT_5             = 0x0020,
  P1OUT_6             = 0x0040,
  P1OUT_7             = 0x0080,
};


__no_init volatile union
{
  unsigned char P1DIR;  /* Port 1 Direction */ 
  
  struct
  {
    unsigned char P1DIR_0        : 1; 
    unsigned char P1DIR_1        : 1; 
    unsigned char P1DIR_2        : 1; 
    unsigned char P1DIR_3        : 1; 
    unsigned char P1DIR_4        : 1; 
    unsigned char P1DIR_5        : 1; 
    unsigned char P1DIR_6        : 1; 
    unsigned char P1DIR_7        : 1; 
  } P1DIR_bit;  
} @ 0x0022;


enum {
  P1DIR_0             = 0x0001,
  P1DIR_1             = 0x0002,
  P1DIR_2             = 0x0004,
  P1DIR_3             = 0x0008,
  P1DIR_4             = 0x0010,
  P1DIR_5             = 0x0020,
  P1DIR_6             = 0x0040,
  P1DIR_7             = 0x0080,
};


__no_init volatile union
{
  unsigned char P1IFG;  /* Port 1 Interrupt Flag  */ 
  
  struct
  {
    unsigned char P1IFG_0        : 1; 
    unsigned char P1IFG_1        : 1; 
    unsigned char P1IFG_2        : 1; 
    unsigned char P1IFG_3        : 1; 
    unsigned char P1IFG_4        : 1; 
    unsigned char P1IFG_5        : 1; 
    unsigned char P1IFG_6        : 1; 
    unsigned char P1IFG_7        : 1; 
  } P1IFG_bit;  
} @ 0x0023;


enum {
  P1IFG_0             = 0x0001,
  P1IFG_1             = 0x0002,
  P1IFG_2             = 0x0004,
  P1IFG_3             = 0x0008,
  P1IFG_4             = 0x0010,
  P1IFG_5             = 0x0020,
  P1IFG_6             = 0x0040,
  P1IFG_7             = 0x0080,
};


__no_init volatile union
{
  unsigned char P1IES;  /* Port 1 Interrupt Edge Select */ 
  
  struct
  {
    unsigned char P1IES_0        : 1; 
    unsigned char P1IES_1        : 1; 
    unsigned char P1IES_2        : 1; 
    unsigned char P1IES_3        : 1; 
    unsigned char P1IES_4        : 1; 
    unsigned char P1IES_5        : 1; 
    unsigned char P1IES_6        : 1; 
    unsigned char P1IES_7        : 1; 
  } P1IES_bit;  
} @ 0x0024;


enum {
  P1IES_0             = 0x0001,
  P1IES_1             = 0x0002,
  P1IES_2             = 0x0004,
  P1IES_3             = 0x0008,
  P1IES_4             = 0x0010,
  P1IES_5             = 0x0020,
  P1IES_6             = 0x0040,
  P1IES_7             = 0x0080,
};


__no_init volatile union
{
  unsigned char P1IE;  /* Port 1 Interrupt Enable */ 
  
  struct
  {
    unsigned char P1IE_0         : 1; 
    unsigned char P1IE_1         : 1; 
    unsigned char P1IE_2         : 1; 
    unsigned char P1IE_3         : 1; 
    unsigned char P1IE_4         : 1; 
    unsigned char P1IE_5         : 1; 
    unsigned char P1IE_6         : 1; 
    unsigned char P1IE_7         : 1; 
  } P1IE_bit;  
} @ 0x0025;


enum {
  P1IE_0              = 0x0001,
  P1IE_1              = 0x0002,
  P1IE_2              = 0x0004,
  P1IE_3              = 0x0008,
  P1IE_4              = 0x0010,
  P1IE_5              = 0x0020,
  P1IE_6              = 0x0040,
  P1IE_7              = 0x0080,
};


__no_init volatile union
{
  unsigned char P1SEL;  /* Port 1 Selection */ 
  
  struct
  {
    unsigned char P1SEL_0        : 1; 
    unsigned char P1SEL_1        : 1; 
    unsigned char P1SEL_2        : 1; 
    unsigned char P1SEL_3        : 1; 
    unsigned char P1SEL_4        : 1; 
    unsigned char P1SEL_5        : 1; 
    unsigned char P1SEL_6        : 1; 
    unsigned char P1SEL_7        : 1; 
  } P1SEL_bit;  
} @ 0x0026;


enum {
  P1SEL_0             = 0x0001,
  P1SEL_1             = 0x0002,
  P1SEL_2             = 0x0004,
  P1SEL_3             = 0x0008,
  P1SEL_4             = 0x0010,
  P1SEL_5             = 0x0020,
  P1SEL_6             = 0x0040,
  P1SEL_7             = 0x0080,
};


__no_init volatile union
{
  unsigned __READ char P2IN;  /* Port 2 Input */ 
  
  struct
  {
    unsigned __READ char P2IN_0  : 1; 
    unsigned __READ char P2IN_1  : 1; 
    unsigned __READ char P2IN_2  : 1; 
    unsigned __READ char P2IN_3  : 1; 
    unsigned __READ char P2IN_4  : 1; 
    unsigned __READ char P2IN_5  : 1; 
    unsigned __READ char P2IN_6  : 1; 
    unsigned __READ char P2IN_7  : 1; 
  } P2IN_bit;  
} @ 0x0028;


enum {
  P2IN_0              = 0x0001,
  P2IN_1              = 0x0002,
  P2IN_2              = 0x0004,
  P2IN_3              = 0x0008,
  P2IN_4              = 0x0010,
  P2IN_5              = 0x0020,
  P2IN_6              = 0x0040,
  P2IN_7              = 0x0080,
};


__no_init volatile union
{
  unsigned char P2OUT;  /* Port 2 Output */ 
  
  struct
  {
    unsigned char P2OUT_0        : 1; 
    unsigned char P2OUT_1        : 1; 
    unsigned char P2OUT_2        : 1; 
    unsigned char P2OUT_3        : 1; 
    unsigned char P2OUT_4        : 1; 
    unsigned char P2OUT_5        : 1; 
    unsigned char P2OUT_6        : 1; 
    unsigned char P2OUT_7        : 1; 
  } P2OUT_bit;  
} @ 0x0029;


enum {
  P2OUT_0             = 0x0001,
  P2OUT_1             = 0x0002,
  P2OUT_2             = 0x0004,
  P2OUT_3             = 0x0008,
  P2OUT_4             = 0x0010,
  P2OUT_5             = 0x0020,
  P2OUT_6             = 0x0040,
  P2OUT_7             = 0x0080,
};


__no_init volatile union
{
  unsigned char P2DIR;  /* Port 2 Direction */ 
  
  struct
  {
    unsigned char P2DIR_0        : 1; 
    unsigned char P2DIR_1        : 1; 
    unsigned char P2DIR_2        : 1; 
    unsigned char P2DIR_3        : 1; 
    unsigned char P2DIR_4        : 1; 
    unsigned char P2DIR_5        : 1; 
    unsigned char P2DIR_6        : 1; 
    unsigned char P2DIR_7        : 1; 
  } P2DIR_bit;  
} @ 0x002A;


enum {
  P2DIR_0             = 0x0001,
  P2DIR_1             = 0x0002,
  P2DIR_2             = 0x0004,
  P2DIR_3             = 0x0008,
  P2DIR_4             = 0x0010,
  P2DIR_5             = 0x0020,
  P2DIR_6             = 0x0040,
  P2DIR_7             = 0x0080,
};


__no_init volatile union
{
  unsigned char P2IFG;  /* Port 2 Interrupt Flag */ 
  
  struct
  {
    unsigned char P2IFG_0        : 1; 
    unsigned char P2IFG_1        : 1; 
    unsigned char P2IFG_2        : 1; 
    unsigned char P2IFG_3        : 1; 
    unsigned char P2IFG_4        : 1; 
    unsigned char P2IFG_5        : 1; 
    unsigned char P2IFG_6        : 1; 
    unsigned char P2IFG_7        : 1; 
  } P2IFG_bit;  
} @ 0x002B;


enum {
  P2IFG_0             = 0x0001,
  P2IFG_1             = 0x0002,
  P2IFG_2             = 0x0004,
  P2IFG_3             = 0x0008,
  P2IFG_4             = 0x0010,
  P2IFG_5             = 0x0020,
  P2IFG_6             = 0x0040,
  P2IFG_7             = 0x0080,
};


__no_init volatile union
{
  unsigned char P2IES;  /* Port 2 Interrupt Edge Select */ 
  
  struct
  {
    unsigned char P2IES_0        : 1; 
    unsigned char P2IES_1        : 1; 
    unsigned char P2IES_2        : 1; 
    unsigned char P2IES_3        : 1; 
    unsigned char P2IES_4        : 1; 
    unsigned char P2IES_5        : 1; 
    unsigned char P2IES_6        : 1; 
    unsigned char P2IES_7        : 1; 
  } P2IES_bit;  
} @ 0x002C;


enum {
  P2IES_0             = 0x0001,
  P2IES_1             = 0x0002,
  P2IES_2             = 0x0004,
  P2IES_3             = 0x0008,
  P2IES_4             = 0x0010,
  P2IES_5             = 0x0020,
  P2IES_6             = 0x0040,
  P2IES_7             = 0x0080,
};


__no_init volatile union
{
  unsigned char P2IE;  /* Port 2 Interrupt Enable */ 
  
  struct
  {
    unsigned char P2IE_0         : 1; 
    unsigned char P2IE_1         : 1; 
    unsigned char P2IE_2         : 1; 
    unsigned char P2IE_3         : 1; 
    unsigned char P2IE_4         : 1; 
    unsigned char P2IE_5         : 1; 
    unsigned char P2IE_6         : 1; 
    unsigned char P2IE_7         : 1; 
  } P2IE_bit;  
} @ 0x002D;


enum {
  P2IE_0              = 0x0001,
  P2IE_1              = 0x0002,
  P2IE_2              = 0x0004,
  P2IE_3              = 0x0008,
  P2IE_4              = 0x0010,
  P2IE_5              = 0x0020,
  P2IE_6              = 0x0040,
  P2IE_7              = 0x0080,
};


__no_init volatile union
{
  unsigned char P2SEL;  /* Port 2 Selection */ 
  
  struct
  {
    unsigned char P2SEL_0        : 1; 
    unsigned char P2SEL_1        : 1; 
    unsigned char P2SEL_2        : 1; 
    unsigned char P2SEL_3        : 1; 
    unsigned char P2SEL_4        : 1; 
    unsigned char P2SEL_5        : 1; 
    unsigned char P2SEL_6        : 1; 
    unsigned char P2SEL_7        : 1; 
  } P2SEL_bit;  
} @ 0x002E;


enum {
  P2SEL_0             = 0x0001,
  P2SEL_1             = 0x0002,
  P2SEL_2             = 0x0004,
  P2SEL_3             = 0x0008,
  P2SEL_4             = 0x0010,
  P2SEL_5             = 0x0020,
  P2SEL_6             = 0x0040,
  P2SEL_7             = 0x0080,
};

 
#define __MSP430_HAS_PORT1__          /* Definition to show that Module is available */
#define __MSP430_HAS_PORT2__          /* Definition to show that Module is available */
 

/*-------------------------------------------------------------------------
 *   Digital I/O Port3/4
 *-------------------------------------------------------------------------*/


__no_init volatile union
{
  unsigned __READ char P3IN;  /* Port 3 Input */ 
  
  struct
  {
    unsigned __READ char P3IN_0  : 1; 
    unsigned __READ char P3IN_1  : 1; 
    unsigned __READ char P3IN_2  : 1; 
    unsigned __READ char P3IN_3  : 1; 
    unsigned __READ char P3IN_4  : 1; 
    unsigned __READ char P3IN_5  : 1; 
    unsigned __READ char P3IN_6  : 1; 
    unsigned __READ char P3IN_7  : 1; 
  } P3IN_bit;  
} @ 0x0018;


enum {
  P3IN_0              = 0x0001,
  P3IN_1              = 0x0002,
  P3IN_2              = 0x0004,
  P3IN_3              = 0x0008,
  P3IN_4              = 0x0010,
  P3IN_5              = 0x0020,
  P3IN_6              = 0x0040,
  P3IN_7              = 0x0080,
};


__no_init volatile union
{
  unsigned char P3OUT;  /* Port 3 Output */ 
  
  struct
  {
    unsigned char P3OUT_0        : 1; 
    unsigned char P3OUT_1        : 1; 
    unsigned char P3OUT_2        : 1; 
    unsigned char P3OUT_3        : 1; 
    unsigned char P3OUT_4        : 1; 
    unsigned char P3OUT_5        : 1; 
    unsigned char P3OUT_6        : 1; 
    unsigned char P3OUT_7        : 1; 
  } P3OUT_bit;  
} @ 0x0019;


enum {
  P3OUT_0             = 0x0001,
  P3OUT_1             = 0x0002,
  P3OUT_2             = 0x0004,
  P3OUT_3             = 0x0008,
  P3OUT_4             = 0x0010,
  P3OUT_5             = 0x0020,
  P3OUT_6             = 0x0040,
  P3OUT_7             = 0x0080,
};


__no_init volatile union
{
  unsigned char P3DIR;  /* Port 3 Direction */ 
  
  struct
  {
    unsigned char P3DIR_0        : 1; 
    unsigned char P3DIR_1        : 1; 
    unsigned char P3DIR_2        : 1; 
    unsigned char P3DIR_3        : 1; 
    unsigned char P3DIR_4        : 1; 
    unsigned char P3DIR_5        : 1; 
    unsigned char P3DIR_6        : 1; 
    unsigned char P3DIR_7        : 1; 
  } P3DIR_bit;  
} @ 0x001A;


enum {
  P3DIR_0             = 0x0001,
  P3DIR_1             = 0x0002,
  P3DIR_2             = 0x0004,
  P3DIR_3             = 0x0008,
  P3DIR_4             = 0x0010,
  P3DIR_5             = 0x0020,
  P3DIR_6             = 0x0040,
  P3DIR_7             = 0x0080,
};


__no_init volatile union
{
  unsigned char P3SEL;  /* Port 3 Selection */ 
  
  struct
  {
    unsigned char P3SEL_0        : 1; 
    unsigned char P3SEL_1        : 1; 
    unsigned char P3SEL_2        : 1; 
    unsigned char P3SEL_3        : 1; 
    unsigned char P3SEL_4        : 1; 
    unsigned char P3SEL_5        : 1; 
    unsigned char P3SEL_6        : 1; 
    unsigned char P3SEL_7        : 1; 
  } P3SEL_bit;  
} @ 0x001B;


enum {
  P3SEL_0             = 0x0001,
  P3SEL_1             = 0x0002,
  P3SEL_2             = 0x0004,
  P3SEL_3             = 0x0008,
  P3SEL_4             = 0x0010,
  P3SEL_5             = 0x0020,
  P3SEL_6             = 0x0040,
  P3SEL_7             = 0x0080,
};


__no_init volatile union
{
  unsigned __READ char P4IN;  /* Port 4 Input */ 
  
  struct
  {
    unsigned __READ char P4IN_0  : 1; 
    unsigned __READ char P4IN_1  : 1; 
    unsigned __READ char P4IN_2  : 1; 
    unsigned __READ char P4IN_3  : 1; 
    unsigned __READ char P4IN_4  : 1; 
    unsigned __READ char P4IN_5  : 1; 
    unsigned __READ char P4IN_6  : 1; 
    unsigned __READ char P4IN_7  : 1; 
  } P4IN_bit;  
} @ 0x001C;


enum {
  P4IN_0              = 0x0001,
  P4IN_1              = 0x0002,
  P4IN_2              = 0x0004,
  P4IN_3              = 0x0008,
  P4IN_4              = 0x0010,
  P4IN_5              = 0x0020,
  P4IN_6              = 0x0040,
  P4IN_7              = 0x0080,
};


__no_init volatile union
{
  unsigned char P4OUT;  /* Port 4 Output */ 
  
  struct
  {
    unsigned char P4OUT_0        : 1; 
    unsigned char P4OUT_1        : 1; 
    unsigned char P4OUT_2        : 1; 
    unsigned char P4OUT_3        : 1; 
    unsigned char P4OUT_4        : 1; 
    unsigned char P4OUT_5        : 1; 
    unsigned char P4OUT_6        : 1; 
    unsigned char P4OUT_7        : 1; 
  } P4OUT_bit;  
} @ 0x001D;


enum {
  P4OUT_0             = 0x0001,
  P4OUT_1             = 0x0002,
  P4OUT_2             = 0x0004,
  P4OUT_3             = 0x0008,
  P4OUT_4             = 0x0010,
  P4OUT_5             = 0x0020,
  P4OUT_6             = 0x0040,
  P4OUT_7             = 0x0080,
};


__no_init volatile union
{
  unsigned char P4DIR;  /* Port 4 Direction */ 
  
  struct
  {
    unsigned char P4DIR_0        : 1; 
    unsigned char P4DIR_1        : 1; 
    unsigned char P4DIR_2        : 1; 
    unsigned char P4DIR_3        : 1; 
    unsigned char P4DIR_4        : 1; 
    unsigned char P4DIR_5        : 1; 
    unsigned char P4DIR_6        : 1; 
    unsigned char P4DIR_7        : 1; 
  } P4DIR_bit;  
} @ 0x001E;


enum {
  P4DIR_0             = 0x0001,
  P4DIR_1             = 0x0002,
  P4DIR_2             = 0x0004,
  P4DIR_3             = 0x0008,
  P4DIR_4             = 0x0010,
  P4DIR_5             = 0x0020,
  P4DIR_6             = 0x0040,
  P4DIR_7             = 0x0080,
};


__no_init volatile union
{
  unsigned char P4SEL;  /* Port 4 Selection */ 
  
  struct
  {
    unsigned char P4SEL_0        : 1; 
    unsigned char P4SEL_1        : 1; 
    unsigned char P4SEL_2        : 1; 
    unsigned char P4SEL_3        : 1; 
    unsigned char P4SEL_4        : 1; 
    unsigned char P4SEL_5        : 1; 
    unsigned char P4SEL_6        : 1; 
    unsigned char P4SEL_7        : 1; 
  } P4SEL_bit;  
} @ 0x001F;


enum {
  P4SEL_0             = 0x0001,
  P4SEL_1             = 0x0002,
  P4SEL_2             = 0x0004,
  P4SEL_3             = 0x0008,
  P4SEL_4             = 0x0010,
  P4SEL_5             = 0x0020,
  P4SEL_6             = 0x0040,
  P4SEL_7             = 0x0080,
};



/*-------------------------------------------------------------------------
 *   Digital I/O Port5/6
 *-------------------------------------------------------------------------*/


__no_init volatile union
{
  unsigned __READ char P5IN;  /* Port 5 Input */ 
  
  struct
  {
    unsigned __READ char P5IN_0  : 1; 
    unsigned __READ char P5IN_1  : 1; 
    unsigned __READ char P5IN_2  : 1; 
    unsigned __READ char P5IN_3  : 1; 
    unsigned __READ char P5IN_4  : 1; 
    unsigned __READ char P5IN_5  : 1; 
    unsigned __READ char P5IN_6  : 1; 
    unsigned __READ char P5IN_7  : 1; 
  } P5IN_bit;  
} @ 0x0030;


enum {
  P5IN_0              = 0x0001,
  P5IN_1              = 0x0002,
  P5IN_2              = 0x0004,
  P5IN_3              = 0x0008,
  P5IN_4              = 0x0010,
  P5IN_5              = 0x0020,
  P5IN_6              = 0x0040,
  P5IN_7              = 0x0080,
};


__no_init volatile union
{
  unsigned char P5OUT;  /* Port 5 Output */ 
  
  struct
  {
    unsigned char P5OUT_0        : 1; 
    unsigned char P5OUT_1        : 1; 
    unsigned char P5OUT_2        : 1; 
    unsigned char P5OUT_3        : 1; 
    unsigned char P5OUT_4        : 1; 
    unsigned char P5OUT_5        : 1; 
    unsigned char P5OUT_6        : 1; 
    unsigned char P5OUT_7        : 1; 
  } P5OUT_bit;  
} @ 0x0031;


enum {
  P5OUT_0             = 0x0001,
  P5OUT_1             = 0x0002,
  P5OUT_2             = 0x0004,
  P5OUT_3             = 0x0008,
  P5OUT_4             = 0x0010,
  P5OUT_5             = 0x0020,
  P5OUT_6             = 0x0040,
  P5OUT_7             = 0x0080,
};


__no_init volatile union
{
  unsigned char P5DIR;  /* Port 5 Direction */ 
  
  struct
  {
    unsigned char P5DIR_0        : 1; 
    unsigned char P5DIR_1        : 1; 
    unsigned char P5DIR_2        : 1; 
    unsigned char P5DIR_3        : 1; 
    unsigned char P5DIR_4        : 1; 
    unsigned char P5DIR_5        : 1; 
    unsigned char P5DIR_6        : 1; 
    unsigned char P5DIR_7        : 1; 
  } P5DIR_bit;  
} @ 0x0032;


enum {
  P5DIR_0             = 0x0001,
  P5DIR_1             = 0x0002,
  P5DIR_2             = 0x0004,
  P5DIR_3             = 0x0008,
  P5DIR_4             = 0x0010,
  P5DIR_5             = 0x0020,
  P5DIR_6             = 0x0040,
  P5DIR_7             = 0x0080,
};


__no_init volatile union
{
  unsigned char P5SEL;  /* Port 5 Selection */ 
  
  struct
  {
    unsigned char P5SEL_0        : 1; 
    unsigned char P5SEL_1        : 1; 
    unsigned char P5SEL_2        : 1; 
    unsigned char P5SEL_3        : 1; 
    unsigned char P5SEL_4        : 1; 
    unsigned char P5SEL_5        : 1; 
    unsigned char P5SEL_6        : 1; 
    unsigned char P5SEL_7        : 1; 
  } P5SEL_bit;  
} @ 0x0033;


enum {
  P5SEL_0             = 0x0001,
  P5SEL_1             = 0x0002,
  P5SEL_2             = 0x0004,
  P5SEL_3             = 0x0008,
  P5SEL_4             = 0x0010,
  P5SEL_5             = 0x0020,
  P5SEL_6             = 0x0040,
  P5SEL_7             = 0x0080,
};


__no_init volatile union
{
  unsigned __READ char P6IN;  /* Port 6 Input */ 
  
  struct
  {
    unsigned __READ char P6IN_0  : 1; 
    unsigned __READ char P6IN_1  : 1; 
    unsigned __READ char P6IN_2  : 1; 
    unsigned __READ char P6IN_3  : 1; 
    unsigned __READ char P6IN_4  : 1; 
    unsigned __READ char P6IN_5  : 1; 
    unsigned __READ char P6IN_6  : 1; 
    unsigned __READ char P6IN_7  : 1; 
  } P6IN_bit;  
} @ 0x0034;


enum {
  P6IN_0              = 0x0001,
  P6IN_1              = 0x0002,
  P6IN_2              = 0x0004,
  P6IN_3              = 0x0008,
  P6IN_4              = 0x0010,
  P6IN_5              = 0x0020,
  P6IN_6              = 0x0040,
  P6IN_7              = 0x0080,
};


__no_init volatile union
{
  unsigned char P6OUT;  /* Port 6 Output */ 
  
  struct
  {
    unsigned char P6OUT_0        : 1; 
    unsigned char P6OUT_1        : 1; 
    unsigned char P6OUT_2        : 1; 
    unsigned char P6OUT_3        : 1; 
    unsigned char P6OUT_4        : 1; 
    unsigned char P6OUT_5        : 1; 
    unsigned char P6OUT_6        : 1; 
    unsigned char P6OUT_7        : 1; 
  } P6OUT_bit;  
} @ 0x0035;


enum {
  P6OUT_0             = 0x0001,
  P6OUT_1             = 0x0002,
  P6OUT_2             = 0x0004,
  P6OUT_3             = 0x0008,
  P6OUT_4             = 0x0010,
  P6OUT_5             = 0x0020,
  P6OUT_6             = 0x0040,
  P6OUT_7             = 0x0080,
};


__no_init volatile union
{
  unsigned char P6DIR;  /* Port 6 Direction */ 
  
  struct
  {
    unsigned char P6DIR_0        : 1; 
    unsigned char P6DIR_1        : 1; 
    unsigned char P6DIR_2        : 1; 
    unsigned char P6DIR_3        : 1; 
    unsigned char P6DIR_4        : 1; 
    unsigned char P6DIR_5        : 1; 
    unsigned char P6DIR_6        : 1; 
    unsigned char P6DIR_7        : 1; 
  } P6DIR_bit;  
} @ 0x0036;


enum {
  P6DIR_0             = 0x0001,
  P6DIR_1             = 0x0002,
  P6DIR_2             = 0x0004,
  P6DIR_3             = 0x0008,
  P6DIR_4             = 0x0010,
  P6DIR_5             = 0x0020,
  P6DIR_6             = 0x0040,
  P6DIR_7             = 0x0080,
};


__no_init volatile union
{
  unsigned char P6SEL;  /* Port 6 Selection */ 
  
  struct
  {
    unsigned char P6SEL_0        : 1; 
    unsigned char P6SEL_1        : 1; 
    unsigned char P6SEL_2        : 1; 
    unsigned char P6SEL_3        : 1; 
    unsigned char P6SEL_4        : 1; 
    unsigned char P6SEL_5        : 1; 
    unsigned char P6SEL_6        : 1; 
    unsigned char P6SEL_7        : 1; 
  } P6SEL_bit;  
} @ 0x0037;


enum {
  P6SEL_0             = 0x0001,
  P6SEL_1             = 0x0002,
  P6SEL_2             = 0x0004,
  P6SEL_3             = 0x0008,
  P6SEL_4             = 0x0010,
  P6SEL_5             = 0x0020,
  P6SEL_6             = 0x0040,
  P6SEL_7             = 0x0080,
};

 
#define __MSP430_HAS_PORT3__          /* Definition to show that Module is available */
#define __MSP430_HAS_PORT4__          /* Definition to show that Module is available */
#define __MSP430_HAS_PORT5__          /* Definition to show that Module is available */
#define __MSP430_HAS_PORT6__          /* Definition to show that Module is available */
 

/*-------------------------------------------------------------------------
 *   USART 0
 *-------------------------------------------------------------------------*/


__no_init volatile union
{
  unsigned char U0CTL;  /* USART 0 Control */ 
  
  struct
  {
    unsigned char SWRST          : 1;  /* USART Software Reset */ 
    unsigned char MM             : 1;  /* Master Mode off/on */ 
    unsigned char SYNC           : 1;  /* UART / SPI mode */ 
    unsigned char LISTEN         : 1;  /* Listen mode */ 
    unsigned char CHAR           : 1;  /* Data 0:7-bits / 1:8-bits */ 
    unsigned char SPB            : 1;  /* Stop Bits 0:one / 1: two */ 
    unsigned char PEV            : 1;  /* Parity 0:odd / 1:even */ 
    unsigned char PENA           : 1;  /* Parity enable */ 
  } U0CTL_bit;  
} @ 0x0070;


enum {
  SWRST               = 0x0001,
  MM                  = 0x0002,
  SYNC                = 0x0004,
  LISTEN              = 0x0008,
  CHAR                = 0x0010,
  SPB                 = 0x0020,
  PEV                 = 0x0040,
  PENA                = 0x0080,
};


__no_init volatile union
{
  unsigned char U0TCTL;  /* USART 0 Transmit Control */ 
  
  struct
  {
    unsigned char TXEPT          : 1;  /* TX Buffer empty */ 
    unsigned char STC            : 1;  /* SPI: STC enable 0:on / 1:off */ 
    unsigned char TXWAKE         : 1;  /* TX Wake up mode */ 
    unsigned char URXSE          : 1;  /* Receive Start edge select */ 
    unsigned char SSEL0          : 1;  /* Clock Source Select 0 */ 
    unsigned char SSEL1          : 1;  /* Clock Source Select 1 */ 
    unsigned char CKPL           : 1;  /* Clock Polarity */ 
    unsigned char CKPH           : 1;  /* SPI: Clock Phase */ 
  } U0TCTL_bit;  
} @ 0x0071;


enum {
  TXEPT               = 0x0001,
  STC                 = 0x0002,
  TXWAKE              = 0x0004,
  URXSE               = 0x0008,
  SSEL0               = 0x0010,
  SSEL1               = 0x0020,
  CKPL                = 0x0040,
  CKPH                = 0x0080,
};


__no_init volatile union
{
  unsigned char U0RCTL;  /* USART 0 Receive Control */ 
  
  struct
  {
    unsigned char RXERR          : 1;  /* RX Error Error */ 
    unsigned char RXWAKE         : 1;  /* RX Wake up detect */ 
    unsigned char URXWIE         : 1;  /* RX Wake up interrupt enable */ 
    unsigned char URXEIE         : 1;  /* RX Error interrupt enable */ 
    unsigned char BRK            : 1;  /* Break detected */ 
    unsigned char OE             : 1;  /* Overrun Error */ 
    unsigned char PE             : 1;  /* Parity Error */ 
    unsigned char FE             : 1;  /* Frame Error */ 
  } U0RCTL_bit;  
} @ 0x0072;


enum {
  RXERR               = 0x0001,
  RXWAKE              = 0x0002,
  URXWIE              = 0x0004,
  URXEIE              = 0x0008,
  BRK                 = 0x0010,
  OE                  = 0x0020,
  PE                  = 0x0040,
  FE                  = 0x0080,
};


 /* USART 0 Modulation Control */ 
__no_init volatile unsigned char U0MCTL @ 0x0073;  
 

 /* USART 0 Baud Rate 0 */ 
__no_init volatile unsigned char U0BR0 @ 0x0074;  
 

 /* USART 0 Baud Rate 1 */ 
__no_init volatile unsigned char U0BR1 @ 0x0075;  
 

 /* USART 0 Receive Buffer */ 
__no_init volatile unsigned __READ char U0RXBUF @ 0x0076;  
 

 /* USART 0 Transmit Buffer  */ 
__no_init volatile unsigned char U0TXBUF @ 0x0077;  
 
 
#define __MSP430_HAS_UART0__          /* Definition to show that Module is available */

/* Alternate register names */

#define UCTL0               U0CTL     /* USART 0 Control */
#define UTCTL0              U0TCTL    /* USART 0 Transmit Control */
#define URCTL0              U0RCTL    /* USART 0 Receive Control */
#define UMCTL0              U0MCTL    /* USART 0 Modulation Control */
#define UBR00               U0BR0     /* USART 0 Baud Rate 0 */
#define UBR10               U0BR1     /* USART 0 Baud Rate 1 */
#define RXBUF0              U0RXBUF   /* USART 0 Receive Buffer */
#define TXBUF0              U0TXBUF   /* USART 0 Transmit Buffer */
#define UCTL0_              U0CTL     /* USART 0 Control */
#define UTCTL0_             U0TCTL    /* USART 0 Transmit Control */
#define URCTL0_             U0RCTL    /* USART 0 Receive Control */
#define UMCTL0_             U0MCTL    /* USART 0 Modulation Control */
#define UBR00_              U0BR0     /* USART 0 Baud Rate 0 */
#define UBR10_              U0BR1     /* USART 0 Baud Rate 1 */
#define RXBUF0_             U0RXBUF   /* USART 0 Receive Buffer */
#define TXBUF0_             U0TXBUF   /* USART 0 Transmit Buffer */
#define UCTL_0              U0CTL     /* USART 0 Control */
#define UTCTL_0             U0TCTL    /* USART 0 Transmit Control */
#define URCTL_0             U0RCTL    /* USART 0 Receive Control */
#define UMCTL_0             U0MCTL    /* USART 0 Modulation Control */
#define UBR0_0              U0BR0     /* USART 0 Baud Rate 0 */
#define UBR1_0              U0BR1     /* USART 0 Baud Rate 1 */
#define RXBUF_0             U0RXBUF   /* USART 0 Receive Buffer */
#define TXBUF_0             U0TXBUF   /* USART 0 Transmit Buffer */
#define UCTL_0_             U0CTL     /* USART 0 Control */
#define UTCTL_0_            U0TCTL    /* USART 0 Transmit Control */
#define URCTL_0_            U0RCTL    /* USART 0 Receive Control */
#define UMCTL_0_            U0MCTL    /* USART 0 Modulation Control */
#define UBR0_0_             U0BR0     /* USART 0 Baud Rate 0 */
#define UBR1_0_             U0BR1     /* USART 0 Baud Rate 1 */
#define RXBUF_0_            U0RXBUF   /* USART 0 Receive Buffer */
#define TXBUF_0_            U0TXBUF   /* USART 0 Transmit Buffer */
 

/*-------------------------------------------------------------------------
 *   USART 1
 *-------------------------------------------------------------------------*/


__no_init volatile union
{
  unsigned char U1CTL;  /* USART 1 Control */ 
  
  struct
  {
    unsigned char SWRST          : 1;  /* USART Software Reset */ 
    unsigned char MM             : 1;  /* Master Mode off/on */ 
    unsigned char SYNC           : 1;  /* UART / SPI mode */ 
    unsigned char LISTEN         : 1;  /* Listen mode */ 
    unsigned char CHAR           : 1;  /* Data 0:7-bits / 1:8-bits */ 
    unsigned char SPB            : 1;  /* Stop Bits 0:one / 1: two */ 
    unsigned char PEV            : 1;  /* Parity 0:odd / 1:even */ 
    unsigned char PENA           : 1;  /* Parity enable */ 
  } U1CTL_bit;  
} @ 0x0078;




__no_init volatile union
{
  unsigned char U1TCTL;  /* USART 1 Transmit Control */ 
  
  struct
  {
    unsigned char TXEPT          : 1;  /* TX Buffer empty */ 
    unsigned char STC            : 1;  /* SPI: STC enable 0:on / 1:off */ 
    unsigned char TXWAKE         : 1;  /* TX Wake up mode */ 
    unsigned char URXSE          : 1;  /* Receive Start edge select */ 
    unsigned char SSEL0          : 1;  /* Clock Source Select 0 */ 
    unsigned char SSEL1          : 1;  /* Clock Source Select 1 */ 
    unsigned char CKPL           : 1;  /* Clock Polarity */ 
    unsigned char CKPH           : 1;  /* SPI: Clock Phase */ 
  } U1TCTL_bit;  
} @ 0x0079;




__no_init volatile union
{
  unsigned char U1RCTL;  /* USART 1 Receive Control */ 
  
  struct
  {
    unsigned char RXERR          : 1;  /* RX Error Error */ 
    unsigned char RXWAKE         : 1;  /* RX Wake up detect */ 
    unsigned char URXWIE         : 1;  /* RX Wake up interrupt enable */ 
    unsigned char URXEIE         : 1;  /* RX Error interrupt enable */ 
    unsigned char BRK            : 1;  /* Break detected */ 
    unsigned char OE             : 1;  /* Overrun Error */ 
    unsigned char PE             : 1;  /* Parity Error */ 
    unsigned char FE             : 1;  /* Frame Error */ 
  } U1RCTL_bit;  
} @ 0x007A;




 /* USART 1 Modulation Control */ 
__no_init volatile unsigned char U1MCTL @ 0x007B;  
 

 /* USART 1 Baud Rate 0 */ 
__no_init volatile unsigned char U1BR0 @ 0x007C;  
 

 /* USART 1 Baud Rate 1 */ 
__no_init volatile unsigned char U1BR1 @ 0x007D;  
 

 /* USART 1 Receive Buffer */ 
__no_init volatile unsigned __READ char U1RXBUF @ 0x007E;  
 

 /* USART 1 Transmit Buffer */ 
__no_init volatile unsigned char U1TXBUF @ 0x007F;  
 
 
#define __MSP430_HAS_UART1__          /* Definition to show that Module is available */

/* Alternate register names */

#define UCTL1               U1CTL     /* USART 1 Control */
#define UTCTL1              U1TCTL    /* USART 1 Transmit Control */
#define URCTL1              U1RCTL    /* USART 1 Receive Control */
#define UMCTL1              U1MCTL    /* USART 1 Modulation Control */
#define UBR01               U1BR0     /* USART 1 Baud Rate 0 */
#define UBR11               U1BR1     /* USART 1 Baud Rate 1 */
#define RXBUF1              U1RXBUF   /* USART 1 Receive Buffer */
#define TXBUF1              U1TXBUF   /* USART 1 Transmit Buffer */
#define UCTL1_              U1CTL     /* USART 1 Control */
#define UTCTL1_             U1TCTL    /* USART 1 Transmit Control */
#define URCTL1_             U1RCTL    /* USART 1 Receive Control */
#define UMCTL1_             U1MCTL    /* USART 1 Modulation Control */
#define UBR01_              U1BR0     /* USART 1 Baud Rate 0 */
#define UBR11_              U1BR1     /* USART 1 Baud Rate 1 */
#define RXBUF1_             U1RXBUF   /* USART 1 Receive Buffer */
#define TXBUF1_             U1TXBUF   /* USART 1 Transmit Buffer */
#define UCTL_1              U1CTL     /* USART 1 Control */
#define UTCTL_1             U1TCTL    /* USART 1 Transmit Control */
#define URCTL_1             U1RCTL    /* USART 1 Receive Control */
#define UMCTL_1             U1MCTL    /* USART 1 Modulation Control */
#define UBR0_1              U1BR0     /* USART 1 Baud Rate 0 */
#define UBR1_1              U1BR1     /* USART 1 Baud Rate 1 */
#define RXBUF_1             U1RXBUF   /* USART 1 Receive Buffer */
#define TXBUF_1             U1TXBUF   /* USART 1 Transmit Buffer */
#define UCTL_1_             U1CTL     /* USART 1 Control */
#define UTCTL_1_            U1TCTL    /* USART 1 Transmit Control */
#define URCTL_1_            U1RCTL    /* USART 1 Receive Control */
#define UMCTL_1_            U1MCTL    /* USART 1 Modulation Control */
#define UBR0_1_             U1BR0     /* USART 1 Baud Rate 0 */
#define UBR1_1_             U1BR1     /* USART 1 Baud Rate 1 */
#define RXBUF_1_            U1RXBUF   /* USART 1 Receive Buffer */
#define TXBUF_1_            U1TXBUF   /* USART 1 Transmit Buffer */
 

/*-------------------------------------------------------------------------
 *   Timer A3
 *-------------------------------------------------------------------------*/


__no_init volatile union
{
  unsigned __READ short TAIV;  /* Timer A Interrupt Vector Word */ 
  
  struct
  {
    unsigned __READ short        : 1; 
    unsigned __READ short IRQVEC : 3; 
    unsigned __READ short        : 12; 
  } TAIV_bit;  
} @ 0x012E;


enum {
  IRQVEC              = 0x0008,
};


__no_init volatile union
{
  unsigned short TACTL;  /* Timer A Control */ 
  
  struct
  {
    unsigned short TAIFG         : 1;  /* Timer A counter interrupt flag */ 
    unsigned short TAIE          : 1;  /* Timer A counter interrupt enable */ 
    unsigned short TACLR         : 1;  /* Timer A counter clear */ 
    unsigned short               : 1; 
    unsigned short TAMC          : 2;  /* Timer A mode control 0 */ 
    unsigned short TAID          : 2;  /* Timer A clock input divider */ 
    unsigned short TASSEL        : 2;  /* Timer A clock source select */ 
    unsigned short               : 6; 
  } TACTL_bit;  
} @ 0x0160;


enum {
  TAIFG               = 0x0001,
  TAIE                = 0x0002,
  TACLR               = 0x0004,
  TAMC                = 0x0020,
  TAID                = 0x0080,
  TASSEL              = 0x0200,
};


__no_init volatile union
{
  unsigned short TACCTL0;  /* Timer A Capture/Compare Control 0 */ 
  
  struct
  {
    unsigned short CCIFG         : 1;  /* Capture/compare interrupt flag */ 
    unsigned short COV           : 1;  /* Capture/compare overflow flag */ 
    unsigned short OUT           : 1;  /* PWM Output signal if output mode 0 */ 
    unsigned short CCI           : 1;  /* Capture input signal (read) */ 
    unsigned short CCIE          : 1;  /* Capture/compare interrupt enable */ 
    unsigned short OUTMOD        : 3;  /* Output mode */ 
    unsigned short CAP           : 1;  /* Capture mode: 1 /Compare mode : 0 */ 
    unsigned short               : 1; 
    unsigned short SCCI          : 1;  /* Latched capture signal (read) */ 
    unsigned short SCS           : 1;  /* Capture sychronize */ 
    unsigned short CCIS          : 2;  /* Capture input select */ 
    unsigned short CM            : 2;  /* Capture mode */ 
  } TACCTL0_bit;  
} @ 0x0162;


enum {
  CCIFG               = 0x0001,
  COV                 = 0x0002,
  OUT                 = 0x0004,
  CCI                 = 0x0008,
  CCIE                = 0x0010,
  OUTMOD              = 0x0080,
  CAP                 = 0x0100,
  SCCI                = 0x0400,
  SCS                 = 0x0800,
  CCIS                = 0x2000,
  CM                  = 0x8000,
};


__no_init volatile union
{
  unsigned short TACCTL1;  /* Timer A Capture/Compare Control 1 */ 
  
  struct
  {
    unsigned short CCIFG         : 1;  /* Capture/compare interrupt flag */ 
    unsigned short COV           : 1;  /* Capture/compare overflow flag */ 
    unsigned short OUT           : 1;  /* PWM Output signal if output mode 0 */ 
    unsigned short CCI           : 1;  /* Capture input signal (read) */ 
    unsigned short CCIE          : 1;  /* Capture/compare interrupt enable */ 
    unsigned short OUTMOD        : 3;  /* Output mode */ 
    unsigned short CAP           : 1;  /* Capture mode: 1 /Compare mode : 0 */ 
    unsigned short               : 1; 
    unsigned short SCCI          : 1;  /* Latched capture signal (read) */ 
    unsigned short SCS           : 1;  /* Capture sychronize */ 
    unsigned short CCIS          : 2;  /* Capture input select */ 
    unsigned short CM            : 2;  /* Capture mode */ 
  } TACCTL1_bit;  
} @ 0x0164;




__no_init volatile union
{
  unsigned short TACCTL2;  /* Timer A Capture/Compare Control 2 */ 
  
  struct
  {
    unsigned short CCIFG         : 1;  /* Capture/compare interrupt flag */ 
    unsigned short COV           : 1;  /* Capture/compare overflow flag */ 
    unsigned short OUT           : 1;  /* PWM Output signal if output mode 0 */ 
    unsigned short CCI           : 1;  /* Capture input signal (read) */ 
    unsigned short CCIE          : 1;  /* Capture/compare interrupt enable */ 
    unsigned short OUTMOD        : 3;  /* Output mode */ 
    unsigned short CAP           : 1;  /* Capture mode: 1 /Compare mode : 0 */ 
    unsigned short               : 1; 
    unsigned short SCCI          : 1;  /* Latched capture signal (read) */ 
    unsigned short SCS           : 1;  /* Capture sychronize */ 
    unsigned short CCIS          : 2;  /* Capture input select */ 
    unsigned short CM            : 2;  /* Capture mode */ 
  } TACCTL2_bit;  
} @ 0x0166;




 /* Timer A */ 
__no_init volatile unsigned short TAR @ 0x0170;  
 

 /* Timer A Capture/Compare 0 */ 
__no_init volatile unsigned short TACCR0 @ 0x0172;  
 

 /* Timer A Capture/Compare 1 */ 
__no_init volatile unsigned short TACCR1 @ 0x0174;  
 

 /* Timer A Capture/Compare 2 */ 
__no_init volatile unsigned short TACCR2 @ 0x0176;  
 
 
#define __MSP430_HAS_TA3__         /* Definition to show that Module is available */

#define MC_0            (0*0x10u)  /* Timer A mode control: 0 - Stop */
#define MC_1            (1*0x10u)  /* Timer A mode control: 1 - Up to CCR0 */
#define MC_2            (2*0x10u)  /* Timer A mode control: 2 - Continous up */
#define MC_3            (3*0x10u)  /* Timer A mode control: 3 - Up/Down */
#define ID_0            (0*0x40u)  /* Timer A input divider: 0 - /1 */
#define ID_1            (1*0x40u)  /* Timer A input divider: 1 - /2 */
#define ID_2            (2*0x40u)  /* Timer A input divider: 2 - /4 */
#define ID_3            (3*0x40u)  /* Timer A input divider: 3 - /8 */
#define TASSEL_0        (0*0x100u) /* Timer A clock source select: 0 - TACLK */
#define TASSEL_1        (1*0x100u) /* Timer A clock source select: 1 - ACLK  */
#define TASSEL_2        (2*0x100u) /* Timer A clock source select: 2 - SMCLK */
#define TASSEL_3        (3*0x100u) /* Timer A clock source select: 3 - INCLK */

#define OUTMOD_0        (0*0x20u)  /* PWM output mode: 0 - output only */
#define OUTMOD_1        (1*0x20u)  /* PWM output mode: 1 - set */
#define OUTMOD_2        (2*0x20u)  /* PWM output mode: 2 - PWM toggle/reset */
#define OUTMOD_3        (3*0x20u)  /* PWM output mode: 3 - PWM set/reset */
#define OUTMOD_4        (4*0x20u)  /* PWM output mode: 4 - toggle */
#define OUTMOD_5        (5*0x20u)  /* PWM output mode: 5 - Reset */
#define OUTMOD_6        (6*0x20u)  /* PWM output mode: 6 - PWM toggle/set */
#define OUTMOD_7        (7*0x20u)  /* PWM output mode: 7 - PWM reset/set */
#define CCIS_0          (0*0x1000u) /* Capture input select: 0 - CCIxA */
#define CCIS_1          (1*0x1000u) /* Capture input select: 1 - CCIxB */
#define CCIS_2          (2*0x1000u) /* Capture input select: 2 - GND */
#define CCIS_3          (3*0x1000u) /* Capture input select: 3 - Vcc */
#define CM_0            (0*0x4000u) /* Capture mode: 0 - disabled */
#define CM_1            (1*0x4000u) /* Capture mode: 1 - pos. edge */
#define CM_2            (2*0x4000u) /* Capture mode: 1 - neg. edge */
#define CM_3            (3*0x4000u) /* Capture mode: 1 - both edges */

/* Alternate register names */
#define CCTL0           TACCTL0   /* Timer A Capture/Compare Control 0 */
#define CCTL1           TACCTL1   /* Timer A Capture/Compare Control 1 */
#define CCTL2           TACCTL2   /* Timer A Capture/Compare Control 2 */
#define CCR0            TACCR0    /* Timer A Capture/Compare 0 */
#define CCR1            TACCR1    /* Timer A Capture/Compare 1 */
#define CCR2            TACCR2    /* Timer A Capture/Compare 2 */
#define CCTL0_          TACCTL0   /* Timer A Capture/Compare Control 0 */
#define CCTL1_          TACCTL1   /* Timer A Capture/Compare Control 1 */
#define CCTL2_          TACCTL2   /* Timer A Capture/Compare Control 2 */
#define CCR0_           TACCR0    /* Timer A Capture/Compare 0 */
#define CCR1_           TACCR1    /* Timer A Capture/Compare 1 */
#define CCR2_           TACCR2    /* Timer A Capture/Compare 2 */
 

/*-------------------------------------------------------------------------
 *   Timer B7
 *-------------------------------------------------------------------------*/


__no_init volatile union
{
  unsigned __READ short TBIV;  /* Timer B Interrupt Vector Word */ 
  
  struct
  {
    unsigned __READ short        : 1; 
    unsigned __READ short IRQVEC : 3; 
    unsigned __READ short        : 12; 
  } TBIV_bit;  
} @ 0x011E;




__no_init volatile union
{
  unsigned short TBCTL;  /* Timer B Control */ 
  
  struct
  {
    unsigned short TBIFG         : 1;  /* Timer B interrupt flag */ 
    unsigned short TBIE          : 1;  /* Timer B interrupt enable */ 
    unsigned short TBCLR         : 1;  /* Timer B counter clear */ 
    unsigned short               : 1; 
    unsigned short TBMC          : 2;  /* Timer B mode control 1 */ 
    unsigned short TBID          : 2;  /* Timer B clock input divider */ 
    unsigned short TBSSEL        : 2;  /* Timer B clock source select */ 
    unsigned short               : 1; 
    unsigned short TBCNTL        : 2;  /* Timer B Control */ 
    unsigned short TBCLGRP       : 2;  /* Timer B Compare latch load group */ 
    unsigned short               : 1; 
  } TBCTL_bit;  
} @ 0x0180;


enum {
  TBIFG               = 0x0001,
  TBIE                = 0x0002,
  TBCLR               = 0x0004,
  TBMC                = 0x0020,
  TBID                = 0x0080,
  TBSSEL              = 0x0200,
  TBCNTL              = 0x1000,
  TBCLGRP             = 0x4000,
};


__no_init volatile union
{
  unsigned short TBCCTL0;  /* Timer B Capture/Compare Control 0 */ 
  
  struct
  {
    unsigned short CCIFG         : 1;  /* Capture/compare interrupt flag */ 
    unsigned short COV           : 1;  /* Capture/compare overflow flag */ 
    unsigned short OUT           : 1;  /* PWM Output signal if output mode 0 */ 
    unsigned short CCI           : 1;  /* Capture input signal (read) */ 
    unsigned short CCIE          : 1;  /* Capture/compare interrupt enable */ 
    unsigned short OUTMOD        : 3;  /* Output mode 0 */ 
    unsigned short CAP           : 1;  /* Capture mode: 1 /Compare mode : 0 */ 
    unsigned short CLLD          : 2;  /* Compare latch load source */ 
    unsigned short SCS           : 1;  /* Capture sychronize */ 
    unsigned short CCIS          : 2;  /* Capture input select */ 
    unsigned short CM            : 2;  /* Capture mode */ 
  } TBCCTL0_bit;  
} @ 0x0182;


enum {
  CLLD                = 0x0400,
};


__no_init volatile union
{
  unsigned short TBCCTL1;  /* Timer B Capture/Compare Control 1 */ 
  
  struct
  {
    unsigned short CCIFG         : 1;  /* Capture/compare interrupt flag */ 
    unsigned short COV           : 1;  /* Capture/compare overflow flag */ 
    unsigned short OUT           : 1;  /* PWM Output signal if output mode 0 */ 
    unsigned short CCI           : 1;  /* Capture input signal (read) */ 
    unsigned short CCIE          : 1;  /* Capture/compare interrupt enable */ 
    unsigned short OUTMOD        : 3;  /* Output mode 0 */ 
    unsigned short CAP           : 1;  /* Capture mode: 1 /Compare mode : 0 */ 
    unsigned short CLLD          : 2;  /* Compare latch load source */ 
    unsigned short SCS           : 1;  /* Capture sychronize */ 
    unsigned short CCIS          : 2;  /* Capture input select */ 
    unsigned short CM            : 2;  /* Capture mode */ 
  } TBCCTL1_bit;  
} @ 0x0184;




__no_init volatile union
{
  unsigned short TBCCTL2;  /* Timer B Capture/Compare Control 2 */ 
  
  struct
  {
    unsigned short CCIFG         : 1;  /* Capture/compare interrupt flag */ 
    unsigned short COV           : 1;  /* Capture/compare overflow flag */ 
    unsigned short OUT           : 1;  /* PWM Output signal if output mode 0 */ 
    unsigned short CCI           : 1;  /* Capture input signal (read) */ 
    unsigned short CCIE          : 1;  /* Capture/compare interrupt enable */ 
    unsigned short OUTMOD        : 3;  /* Output mode 0 */ 
    unsigned short CAP           : 1;  /* Capture mode: 1 /Compare mode : 0 */ 
    unsigned short CLLD          : 2;  /* Compare latch load source */ 
    unsigned short SCS           : 1;  /* Capture sychronize */ 
    unsigned short CCIS          : 2;  /* Capture input select */ 
    unsigned short CM            : 2;  /* Capture mode */ 
  } TBCCTL2_bit;  
} @ 0x0186;




__no_init volatile union
{
  unsigned short TBCCTL3;  /* Timer B Capture/Compare Control 3 */ 
  
  struct
  {
    unsigned short CCIFG         : 1;  /* Capture/compare interrupt flag */ 
    unsigned short COV           : 1;  /* Capture/compare overflow flag */ 
    unsigned short OUT           : 1;  /* PWM Output signal if output mode 0 */ 
    unsigned short CCI           : 1;  /* Capture input signal (read) */ 
    unsigned short CCIE          : 1;  /* Capture/compare interrupt enable */ 
    unsigned short OUTMOD        : 3;  /* Output mode 0 */ 
    unsigned short CAP           : 1;  /* Capture mode: 1 /Compare mode : 0 */ 
    unsigned short CLLD          : 2;  /* Compare latch load source */ 
    unsigned short SCS           : 1;  /* Capture sychronize */ 
    unsigned short CCIS          : 2;  /* Capture input select */ 
    unsigned short CM            : 2;  /* Capture mode */ 
  } TBCCTL3_bit;  
} @ 0x0188;




__no_init volatile union
{
  unsigned short TBCCTL4;  /* Timer B Capture/Compare Control 4 */ 
  
  struct
  {
    unsigned short CCIFG         : 1;  /* Capture/compare interrupt flag */ 
    unsigned short COV           : 1;  /* Capture/compare overflow flag */ 
    unsigned short OUT           : 1;  /* PWM Output signal if output mode 0 */ 
    unsigned short CCI           : 1;  /* Capture input signal (read) */ 
    unsigned short CCIE          : 1;  /* Capture/compare interrupt enable */ 
    unsigned short OUTMOD        : 3;  /* Output mode 0 */ 
    unsigned short CAP           : 1;  /* Capture mode: 1 /Compare mode : 0 */ 
    unsigned short CLLD          : 2;  /* Compare latch load source */ 
    unsigned short SCS           : 1;  /* Capture sychronize */ 
    unsigned short CCIS          : 2;  /* Capture input select */ 
    unsigned short CM            : 2;  /* Capture mode */ 
  } TBCCTL4_bit;  
} @ 0x018A;




__no_init volatile union
{
  unsigned short TBCCTL5;  /* Timer B Capture/Compare Control 5 */ 
  
  struct
  {
    unsigned short CCIFG         : 1;  /* Capture/compare interrupt flag */ 
    unsigned short COV           : 1;  /* Capture/compare overflow flag */ 
    unsigned short OUT           : 1;  /* PWM Output signal if output mode 0 */ 
    unsigned short CCI           : 1;  /* Capture input signal (read) */ 
    unsigned short CCIE          : 1;  /* Capture/compare interrupt enable */ 
    unsigned short OUTMOD        : 3;  /* Output mode 0 */ 
    unsigned short CAP           : 1;  /* Capture mode: 1 /Compare mode : 0 */ 
    unsigned short CLLD          : 2;  /* Compare latch load source */ 
    unsigned short SCS           : 1;  /* Capture sychronize */ 
    unsigned short CCIS          : 2;  /* Capture input select */ 
    unsigned short CM            : 2;  /* Capture mode */ 
  } TBCCTL5_bit;  
} @ 0x018C;




__no_init volatile union
{
  unsigned short TBCCTL6;  /* Timer B Capture/Compare Control 6 */ 
  
  struct
  {
    unsigned short CCIFG         : 1;  /* Capture/compare interrupt flag */ 
    unsigned short COV           : 1;  /* Capture/compare overflow flag */ 
    unsigned short OUT           : 1;  /* PWM Output signal if output mode 0 */ 
    unsigned short CCI           : 1;  /* Capture input signal (read) */ 
    unsigned short CCIE          : 1;  /* Capture/compare interrupt enable */ 
    unsigned short OUTMOD        : 3;  /* Output mode 0 */ 
    unsigned short CAP           : 1;  /* Capture mode: 1 /Compare mode : 0 */ 
    unsigned short CLLD          : 2;  /* Compare latch load source */ 
    unsigned short SCS           : 1;  /* Capture sychronize */ 
    unsigned short CCIS          : 2;  /* Capture input select */ 
    unsigned short CM            : 2;  /* Capture mode */ 
  } TBCCTL6_bit;  
} @ 0x018E;




 /* Timer B */ 
__no_init volatile unsigned short TBR @ 0x0190;  
 

 /* Timer B Capture/Compare 0 */ 
__no_init volatile unsigned short TBCCR0 @ 0x0192;  
 

 /* Timer B Capture/Compare 1 */ 
__no_init volatile unsigned short TBCCR1 @ 0x0194;  
 

 /* Timer B Capture/Compare 2 */ 
__no_init volatile unsigned short TBCCR2 @ 0x0196;  
 

 /* Timer B Capture/Compare 3 */ 
__no_init volatile unsigned short TBCCR3 @ 0x0198;  
 

 /* Timer B Capture/Compare 4 */ 
__no_init volatile unsigned short TBCCR4 @ 0x019A;  
 

 /* Timer B Capture/Compare 5 */ 
__no_init volatile unsigned short TBCCR5 @ 0x019C;  
 

 /* Timer B Capture/Compare 6 */ 
__no_init volatile unsigned short TBCCR6 @ 0x019E;  
 
 
#define __MSP430_HAS_TB7__            /* Definition to show that Module is available */

#define TBSSEL_0      (0*0x0100u)  /* Clock Source: TBCLK */
#define TBSSEL_1      (1*0x0100u)  /* Clock Source: ACLK  */
#define TBSSEL_2      (2*0x0100u)  /* Clock Source: SMCLK */
#define TBSSEL_3      (3*0x0100u)  /* Clock Source: INCLK */
#define CNTL_0        (0*0x0800u)  /* Counter lenght: 16 bit */
#define CNTL_1        (1*0x0800u)  /* Counter lenght: 12 bit */
#define CNTL_2        (2*0x0800u)  /* Counter lenght: 10 bit */
#define CNTL_3        (3*0x0800u)  /* Counter lenght:  8 bit */
#define SHR_0         (0*0x2000u)  /* Timer B Group: 0 - individually */
#define SHR_1         (1*0x2000u)  /* Timer B Group: 1 - 3 groups (1-2, 3-4, 5-6) */
#define SHR_2         (2*0x2000u)  /* Timer B Group: 2 - 2 groups (1-3, 4-6)*/
#define SHR_3         (3*0x2000u)  /* Timer B Group: 3 - 1 group (all) */
#define TBCLGRP_0     (0*0x2000u)  /* Timer B Group: 0 - individually */
#define TBCLGRP_1     (1*0x2000u)  /* Timer B Group: 1 - 3 groups (1-2, 3-4, 5-6) */
#define TBCLGRP_2     (2*0x2000u)  /* Timer B Group: 2 - 2 groups (1-3, 4-6)*/
#define TBCLGRP_3     (3*0x2000u)  /* Timer B Group: 3 - 1 group (all) */

#define CLLD1         (0x0400)     /* Compare latch load source 1 */
#define CLLD0         (0x0200)     /* Compare latch load source 0 */

#define SLSHR1        (0x0400)     /* Compare latch load source 1 */
#define SLSHR0        (0x0200)     /* Compare latch load source 0 */

#define SLSHR_0       (0*0x0200u)  /* Compare latch load sourec : 0 - immediate */
#define SLSHR_1       (1*0x0200u)  /* Compare latch load sourec : 1 - TBR counts to 0 */
#define SLSHR_2       (2*0x0200u)  /* Compare latch load sourec : 2 - up/down */
#define SLSHR_3       (3*0x0200u)  /* Compare latch load sourec : 3 - TBR counts to TBCTL0 */

#define CLLD_0        (0*0x0200u)  /* Compare latch load sourec : 0 - immediate */
#define CLLD_1        (1*0x0200u)  /* Compare latch load sourec : 1 - TBR counts to 0 */
#define CLLD_2        (2*0x0200u)  /* Compare latch load sourec : 2 - up/down */
#define CLLD_3        (3*0x0200u)  /* Compare latch load sourec : 3 - TBR counts to TBCTL0 */
 

/*-------------------------------------------------------------------------
 *   Basic Clock Module
 *-------------------------------------------------------------------------*/


__no_init volatile union
{
  unsigned char DCOCTL;  /* DCO Clock Frequency Control */ 
  
  struct
  {
    unsigned char MOD0           : 1;  /* Modulation Bit 0 */ 
    unsigned char MOD1           : 1;  /* Modulation Bit 1 */ 
    unsigned char MOD2           : 1;  /* Modulation Bit 2 */ 
    unsigned char MOD3           : 1;  /* Modulation Bit 3 */ 
    unsigned char MOD4           : 1;  /* Modulation Bit 4 */ 
    unsigned char DCO0           : 1;  /* DCO Select Bit 0 */ 
    unsigned char DCO1           : 1;  /* DCO Select Bit 1 */ 
    unsigned char DCO2           : 1;  /* DCO Select Bit 2 */ 
  } DCOCTL_bit;  
} @ 0x0056;


enum {
  MOD0                = 0x0001,
  MOD1                = 0x0002,
  MOD2                = 0x0004,
  MOD3                = 0x0008,
  MOD4                = 0x0010,
  DCO0                = 0x0020,
  DCO1                = 0x0040,
  DCO2                = 0x0080,
};


__no_init volatile union
{
  unsigned char BCSCTL1;  /* Basic Clock System Control 1 */ 
  
  struct
  {
    unsigned char RSEL0          : 1;  /* Range Select Bit 0 */ 
    unsigned char RSEL1          : 1;  /* Range Select Bit 1 */ 
    unsigned char RSEL2          : 1;  /* Range Select Bit 2 */ 
    unsigned char XT5V           : 1;  /* XT5V should always be reset */ 
    unsigned char DIVA0          : 1;  /* ACLK Divider 0 */ 
    unsigned char DIVA1          : 1;  /* ACLK Divider 1 */ 
    unsigned char XTS            : 1;  /* LFXTCLK 0:Low Freq. / 1: High Freq. */ 
    unsigned char XT2OFF         : 1;  /* Enable XT2CLK */ 
  } BCSCTL1_bit;  
} @ 0x0057;


enum {
  RSEL0               = 0x0001,
  RSEL1               = 0x0002,
  RSEL2               = 0x0004,
  XT5V                = 0x0008,
  DIVA0               = 0x0010,
  DIVA1               = 0x0020,
  XTS                 = 0x0040,
  XT2OFF              = 0x0080,
};


__no_init volatile union
{
  unsigned char BCSCTL2;  /* Basic Clock System Control 2 */ 
  
  struct
  {
    unsigned char DCOR           : 1;  /* Enable External Resistor : 1 */ 
    unsigned char DIVS0          : 1;  /* SMCLK Divider 0 */ 
    unsigned char DIVS1          : 1;  /* SMCLK Divider 1 */ 
    unsigned char SELS           : 1;  /* SMCLK Source Select 0:DCOCLK / 1:XT2CLK/LFXTCLK */ 
    unsigned char DIVM0          : 1;  /* CLK Divider 0 */ 
    unsigned char DIVM1          : 1;  /* CLK Divider 1 */ 
    unsigned char SELM0          : 1;  /* MCLK Source Select 0 */ 
    unsigned char SELM1          : 1;  /* MCLK Source Select 1 */ 
  } BCSCTL2_bit;  
} @ 0x0058;


enum {
  DCOR                = 0x0001,
  DIVS0               = 0x0002,
  DIVS1               = 0x0004,
  SELS                = 0x0008,
  DIVM0               = 0x0010,
  DIVM1               = 0x0020,
  SELM0               = 0x0040,
  SELM1               = 0x0080,
};

 
#define __MSP430_HAS_BASIC_CLOCK__     /* Definition to show that Module is available */

#define DIVA_0           (0x00)       /* ACLK Divider 0: /1 */
#define DIVA_1           (0x10)       /* ACLK Divider 1: /2 */
#define DIVA_2           (0x20)       /* ACLK Divider 2: /4 */
#define DIVA_3           (0x30)       /* ACLK Divider 3: /8 */

#define DIVS_0           (0x00)       /* SMCLK Divider 0: /1 */
#define DIVS_1           (0x02)       /* SMCLK Divider 1: /2 */
#define DIVS_2           (0x04)       /* SMCLK Divider 2: /4 */
#define DIVS_3           (0x06)       /* SMCLK Divider 3: /8 */

#define DIVM_0           (0x00)       /* MCLK Divider 0: /1 */
#define DIVM_1           (0x10)       /* MCLK Divider 1: /2 */
#define DIVM_2           (0x20)       /* MCLK Divider 2: /4 */
#define DIVM_3           (0x30)       /* MCLK Divider 3: /8 */

#define SELM_0           (0x00)       /* MCLK Source Select 0: DCOCLK */
#define SELM_1           (0x40)       /* MCLK Source Select 1: DCOCLK */
#define SELM_2           (0x80)       /* MCLK Source Select 2: XT2CLK/LFXTCLK */
#define SELM_3           (0xC0)       /* MCLK Source Select 3: LFXTCLK */
 

/*-------------------------------------------------------------------------
 *   Comparator A
 *-------------------------------------------------------------------------*/


__no_init volatile union
{
  unsigned char CACTL1;  /* Comparator A Control 1 */ 
  
  struct
  {
    unsigned char CAIFG          : 1;  /* Comp. A Interrupt Flag */ 
    unsigned char CAIE           : 1;  /* Comp. A Interrupt Enable */ 
    unsigned char CAIES          : 1;  /* Comp. A Int. Edge Select: 0:rising / 1:falling */ 
    unsigned char CAON           : 1;  /* Comp. A enable */ 
    unsigned char CAREF0         : 1;  /* Comp. A Internal Reference Select 0 */ 
    unsigned char CAREF1         : 1;  /* Comp. A Internal Reference Select 1 */ 
    unsigned char CARSEL         : 1;  /* Comp. A Internal Reference Enable */ 
    unsigned char CAEX           : 1;  /* Comp. A Exchange Inputs */ 
  } CACTL1_bit;  
} @ 0x0059;


enum {
  CAIFG               = 0x0001,
  CAIE                = 0x0002,
  CAIES               = 0x0004,
  CAON                = 0x0008,
  CAREF0              = 0x0010,
  CAREF1              = 0x0020,
  CARSEL              = 0x0040,
  CAEX                = 0x0080,
};


__no_init volatile union
{
  unsigned char CACTL2;  /* Comparator A Control 2 */ 
  
  struct
  {
    unsigned char CAOUT          : 1;  /* Comp. A Output */ 
    unsigned char CAF            : 1;  /* Comp. A Enable Output Filter */ 
    unsigned char P2CA0          : 1;  /* Comp. A Connect External Signal to CA0 : 1 */ 
    unsigned char P2CA1          : 1;  /* Comp. A Connect External Signal to CA1 : 1 */ 
    unsigned char CACTL24        : 1; 
    unsigned char CACTL25        : 1; 
    unsigned char CACTL26        : 1; 
    unsigned char CACTL27        : 1; 
  } CACTL2_bit;  
} @ 0x005A;


enum {
  CAOUT               = 0x0001,
  CAF                 = 0x0002,
  P2CA0               = 0x0004,
  P2CA1               = 0x0008,
  CACTL24             = 0x0010,
  CACTL25             = 0x0020,
  CACTL26             = 0x0040,
  CACTL27             = 0x0080,
};


__no_init volatile union
{
  unsigned char CAPD;  /* Comparator A Port Disable */ 
  
  struct
  {
    unsigned char CAPD0          : 1;  /* Comp. A Disable Input Buffer of Port Register .0 */ 
    unsigned char CAPD1          : 1;  /* Comp. A Disable Input Buffer of Port Register .1 */ 
    unsigned char CAPD2          : 1;  /* Comp. A Disable Input Buffer of Port Register .2 */ 
    unsigned char CAPD3          : 1;  /* Comp. A Disable Input Buffer of Port Register .3 */ 
    unsigned char CAPD4          : 1;  /* Comp. A Disable Input Buffer of Port Register .4 */ 
    unsigned char CAPD5          : 1;  /* Comp. A Disable Input Buffer of Port Register .5 */ 
    unsigned char CAPD6          : 1;  /* Comp. A Disable Input Buffer of Port Register .6 */ 
    unsigned char CAPD7          : 1;  /* Comp. A Disable Input Buffer of Port Register .7 */ 
  } CAPD_bit;  
} @ 0x005B;


enum {
  CAPD0               = 0x0001,
  CAPD1               = 0x0002,
  CAPD2               = 0x0004,
  CAPD3               = 0x0008,
  CAPD4               = 0x0010,
  CAPD5               = 0x0020,
  CAPD6               = 0x0040,
  CAPD7               = 0x0080,
};

 
#define __MSP430_HAS_COMPA__       /* Definition to show that Module is available */

#define CAREF_0          (0x00)    /* Comp. A Int. Ref. Select 0 : Off */
#define CAREF_1          (0x10)    /* Comp. A Int. Ref. Select 1 : 0.25*Vcc */
#define CAREF_2          (0x20)    /* Comp. A Int. Ref. Select 2 : 0.5*Vcc */
#define CAREF_3          (0x30)    /* Comp. A Int. Ref. Select 3 : Vt*/
 

/*-------------------------------------------------------------------------
 *   ADC12
 *-------------------------------------------------------------------------*/


__no_init volatile union
{
  unsigned short ADC12CTL0;  /* ADC12 Control 0 */ 
  
  struct
  {
    unsigned short ADC12SC       : 1;  /* ADC12 Start Conversion */ 
    unsigned short ENC           : 1;  /* ADC12 Enable Conversion */ 
    unsigned short ADC12TOVIE    : 1;  /* ADC12 Timer Overflow interrupt enable */ 
    unsigned short ADC12OVIE     : 1;  /* ADC12 Overflow interrupt enable */ 
    unsigned short ADC12ON       : 1;  /* ADC12 On/enable */ 
    unsigned short REFON         : 1;  /* ADC12 Reference on */ 
    unsigned short REF2_5V       : 1;  /* ADC12 Ref 0:1.5V / 1:2.5V */ 
    unsigned short MSC           : 1;  /* ADC12 Multiple SampleConversion */ 
    unsigned short SHT0          : 4;  /* ADC12 Sample Hold 0 */ 
    unsigned short SHT1          : 4;  /* ADC12 Sample Hold 1 */ 
  } ADC12CTL0_bit;  
} @ 0x01A0;


enum {
  ADC12SC             = 0x0001,
  ENC                 = 0x0002,
  ADC12TOVIE          = 0x0004,
  ADC12OVIE           = 0x0008,
  ADC12ON             = 0x0010,
  REFON               = 0x0020,
  REF2_5V             = 0x0040,
  MSC                 = 0x0080,
  SHT0                = 0x0800,
  SHT1                = 0x8000,
};


__no_init volatile union
{
  unsigned short ADC12CTL1;  /* ADC12 Control 1 */ 
  
  struct
  {
    unsigned short ADC12BUSY     : 1;  /* ADC12 Busy */ 
    unsigned short CONSEQ        : 2;  /* ADC12 Conversion Sequence Select */ 
    unsigned short ADC12SSEL     : 2;  /* ADC12 Clock Source Select */ 
    unsigned short ADC12DIV      : 3;  /* ADC12 Clock Divider Select */ 
    unsigned short ISSH          : 1;  /* ADC12 Invert Sample Hold Signal */ 
    unsigned short SHP           : 1;  /* ADC12 Sample/Hold Pulse Mode */ 
    unsigned short SHS           : 2;  /* ADC12 Sample/Hold Source */ 
    unsigned short CSTARTADD     : 4;  /* ADC12 Conversion Start Address */ 
  } ADC12CTL1_bit;  
} @ 0x01A2;


enum {
  ADC12BUSY           = 0x0001,
  CONSEQ              = 0x0004,
  ADC12SSEL           = 0x0010,
  ADC12DIV            = 0x0080,
  ISSH                = 0x0100,
  SHP                 = 0x0200,
  SHS                 = 0x0800,
  CSTARTADD           = 0x8000,
};


 /* ADC12 Interrupt Flag */ 
__no_init volatile unsigned short ADC12IFG @ 0x01A4;  
 

 /* ADC12 Interrupt Enable */ 
__no_init volatile unsigned short ADC12IE @ 0x01A6;  
 

 /* ADC12 Interrupt Vector Word */ 
__no_init volatile unsigned short ADC12IV @ 0x01A8;  
 

 /* ADC12 Conversion Memory 0 */ 
__no_init volatile unsigned short ADC12MEM0 @ 0x0140;  
 

 /* ADC12 Conversion Memory 1 */ 
__no_init volatile unsigned short ADC12MEM1 @ 0x0142;  
 

 /* ADC12 Conversion Memory 2 */ 
__no_init volatile unsigned short ADC12MEM2 @ 0x0144;  
 

 /* ADC12 Conversion Memory 3 */ 
__no_init volatile unsigned short ADC12MEM3 @ 0x0146;  
 

 /* ADC12 Conversion Memory 4 */ 
__no_init volatile unsigned short ADC12MEM4 @ 0x0148;  
 

 /* ADC12 Conversion Memory 5 */ 
__no_init volatile unsigned short ADC12MEM5 @ 0x014A;  
 

 /* ADC12 Conversion Memory 6 */ 
__no_init volatile unsigned short ADC12MEM6 @ 0x014C;  
 

 /* ADC12 Conversion Memory 7 */ 
__no_init volatile unsigned short ADC12MEM7 @ 0x014E;  
 

 /* ADC12 Conversion Memory 8 */ 
__no_init volatile unsigned short ADC12MEM8 @ 0x0150;  
 

 /* ADC12 Conversion Memory 9 */ 
__no_init volatile unsigned short ADC12MEM9 @ 0x0152;  
 

 /* ADC12 Conversion Memory 10 */ 
__no_init volatile unsigned short ADC12MEM10 @ 0x0154;  
 

 /* ADC12 Conversion Memory 11 */ 
__no_init volatile unsigned short ADC12MEM11 @ 0x0156;  
 

 /* ADC12 Conversion Memory 12 */ 
__no_init volatile unsigned short ADC12MEM12 @ 0x0158;  
 

 /* ADC12 Conversion Memory 13 */ 
__no_init volatile unsigned short ADC12MEM13 @ 0x015A;  
 

 /* ADC12 Conversion Memory 14 */ 
__no_init volatile unsigned short ADC12MEM14 @ 0x015C;  
 

 /* ADC12 Conversion Memory 15 */ 
__no_init volatile unsigned short ADC12MEM15 @ 0x015E;  
 

__no_init volatile union
{
  unsigned char ADC12MCTL0;  /* ADC12 Memory Control 0 */ 
  
  struct
  {
    unsigned char INCH           : 4; 
    unsigned char SREF           : 3; 
    unsigned char EOS            : 1; 
  } ADC12MCTL0_bit;  
} @ 0x0080;


enum {
  INCH                = 0x0008,
  SREF                = 0x0040,
  EOS                 = 0x0080,
};


__no_init volatile union
{
  unsigned char ADC12MCTL1;  /* ADC12 Memory Control 1 */ 
  
  struct
  {
    unsigned char INCH           : 4; 
    unsigned char SREF           : 3; 
    unsigned char EOS            : 1; 
  } ADC12MCTL1_bit;  
} @ 0x0081;




__no_init volatile union
{
  unsigned char ADC12MCTL2;  /* ADC12 Memory Control 2 */ 
  
  struct
  {
    unsigned char INCH           : 4; 
    unsigned char SREF           : 3; 
    unsigned char EOS            : 1; 
  } ADC12MCTL2_bit;  
} @ 0x0082;




__no_init volatile union
{
  unsigned char ADC12MCTL3;  /* ADC12 Memory Control 3 */ 
  
  struct
  {
    unsigned char INCH           : 4; 
    unsigned char SREF           : 3; 
    unsigned char EOS            : 1; 
  } ADC12MCTL3_bit;  
} @ 0x0083;




__no_init volatile union
{
  unsigned char ADC12MCTL4;  /* ADC12 Memory Control 4 */ 
  
  struct
  {
    unsigned char INCH           : 4; 
    unsigned char SREF           : 3; 
    unsigned char EOS            : 1; 
  } ADC12MCTL4_bit;  
} @ 0x0084;




__no_init volatile union
{
  unsigned char ADC12MCTL5;  /* ADC12 Memory Control 5 */ 
  
  struct
  {
    unsigned char INCH           : 4; 
    unsigned char SREF           : 3; 
    unsigned char EOS            : 1; 
  } ADC12MCTL5_bit;  
} @ 0x0085;




__no_init volatile union
{
  unsigned char ADC12MCTL6;  /* ADC12 Memory Control 6 */ 
  
  struct
  {
    unsigned char INCH           : 4; 
    unsigned char SREF           : 3; 
    unsigned char EOS            : 1; 
  } ADC12MCTL6_bit;  
} @ 0x0086;




__no_init volatile union
{
  unsigned char ADC12MCTL7;  /* ADC12 Memory Control 7 */ 
  
  struct
  {
    unsigned char INCH           : 4; 
    unsigned char SREF           : 3; 
    unsigned char EOS            : 1; 
  } ADC12MCTL7_bit;  
} @ 0x0087;




__no_init volatile union
{
  unsigned char ADC12MCTL8;  /* ADC12 Memory Control 8 */ 
  
  struct
  {
    unsigned char INCH           : 4; 
    unsigned char SREF           : 3; 
    unsigned char EOS            : 1; 
  } ADC12MCTL8_bit;  
} @ 0x0088;




__no_init volatile union
{
  unsigned char ADC12MCTL9;  /* ADC12 Memory Control 9 */ 
  
  struct
  {
    unsigned char INCH           : 4; 
    unsigned char SREF           : 3; 
    unsigned char EOS            : 1; 
  } ADC12MCTL9_bit;  
} @ 0x0089;




__no_init volatile union
{
  unsigned char ADC12MCTL10;  /* ADC12 Memory Control 10 */ 
  
  struct
  {
    unsigned char INCH           : 4; 
    unsigned char SREF           : 3; 
    unsigned char EOS            : 1; 
  } ADC12MCTL10_bit;  
} @ 0x008A;




__no_init volatile union
{
  unsigned char ADC12MCTL11;  /* ADC12 Memory Control 11 */ 
  
  struct
  {
    unsigned char INCH           : 4; 
    unsigned char SREF           : 3; 
    unsigned char EOS            : 1; 
  } ADC12MCTL11_bit;  
} @ 0x008B;




__no_init volatile union
{
  unsigned char ADC12MCTL12;  /* ADC12 Memory Control 12 */ 
  
  struct
  {
    unsigned char INCH           : 4; 
    unsigned char SREF           : 3; 
    unsigned char EOS            : 1; 
  } ADC12MCTL12_bit;  
} @ 0x008C;




__no_init volatile union
{
  unsigned char ADC12MCTL13;  /* ADC12 Memory Control 13 */ 
  
  struct
  {
    unsigned char INCH           : 4; 
    unsigned char SREF           : 3; 
    unsigned char EOS            : 1; 
  } ADC12MCTL13_bit;  
} @ 0x008D;




__no_init volatile union
{
  unsigned char ADC12MCTL14;  /* ADC12 Memory Control 14 */ 
  
  struct
  {
    unsigned char INCH           : 4; 
    unsigned char SREF           : 3; 
    unsigned char EOS            : 1; 
  } ADC12MCTL14_bit;  
} @ 0x008E;




__no_init volatile union
{
  unsigned char ADC12MCTL15;  /* ADC12 Memory Control 15 */ 
  
  struct
  {
    unsigned char INCH           : 4; 
    unsigned char SREF           : 3; 
    unsigned char EOS            : 1; 
  } ADC12MCTL15_bit;  
} @ 0x008F;



 
#define __MSP430_HAS_ADC12__          /* Definition to show that Module is available */

#define SHT0_0             (0*0x100u)
#define SHT0_1             (1*0x100u)
#define SHT0_2             (2*0x100u)
#define SHT0_3             (3*0x100u)
#define SHT0_4             (4*0x100u)
#define SHT0_5             (5*0x100u)
#define SHT0_6             (6*0x100u)
#define SHT0_7             (7*0x100u)
#define SHT0_8             (8*0x100u)
#define SHT0_9             (9*0x100u)
#define SHT0_10           (10*0x100u)
#define SHT0_11           (11*0x100u)
#define SHT0_12           (12*0x100u)
#define SHT0_13           (13*0x100u)
#define SHT0_14           (14*0x100u)
#define SHT0_15           (15*0x100u)

#define SHT1_0             (0*0x1000u)
#define SHT1_1             (1*0x1000u)
#define SHT1_2             (2*0x1000u)
#define SHT1_3             (3*0x1000u)
#define SHT1_4             (4*0x1000u)
#define SHT1_5             (5*0x1000u)
#define SHT1_6             (6*0x1000u)
#define SHT1_7             (7*0x1000u)
#define SHT1_8             (8*0x1000u)
#define SHT1_9             (9*0x1000u)
#define SHT1_10           (10*0x1000u)
#define SHT1_11           (11*0x1000u)
#define SHT1_12           (12*0x1000u)
#define SHT1_13           (13*0x1000u)
#define SHT1_14           (14*0x1000u)
#define SHT1_15           (15*0x1000u)

#define CONSEQ_0             (0*2u)
#define CONSEQ_1             (1*2u)
#define CONSEQ_2             (2*2u)
#define CONSEQ_3             (3*2u)
#define ADC12SSEL_0          (0*8u)
#define ADC12SSEL_1          (1*8u)
#define ADC12SSEL_2          (2*8u)
#define ADC12SSEL_3          (3*8u)
#define ADC12DIV_0           (0*0x20u)
#define ADC12DIV_1           (1*0x20u)
#define ADC12DIV_2           (2*0x20u)
#define ADC12DIV_3           (3*0x20u)
#define ADC12DIV_4           (4*0x20u)
#define ADC12DIV_5           (5*0x20u)
#define ADC12DIV_6           (6*0x20u)
#define ADC12DIV_7           (7*0x20u)
#define SHS_0                (0*0x400u)
#define SHS_1                (1*0x400u)
#define SHS_2                (2*0x400u)
#define SHS_3                (3*0x400u)
#define CSTARTADD_0          (0*0x1000u)
#define CSTARTADD_1          (1*0x1000u)
#define CSTARTADD_2          (2*0x1000u)
#define CSTARTADD_3          (3*0x1000u)
#define CSTARTADD_4          (4*0x1000u)
#define CSTARTADD_5          (5*0x1000u)
#define CSTARTADD_6          (6*0x1000u)
#define CSTARTADD_7          (7*0x1000u)
#define CSTARTADD_8          (8*0x1000u)
#define CSTARTADD_9          (9*0x1000u)
#define CSTARTADD_10        (10*0x1000u)
#define CSTARTADD_11        (11*0x1000u)
#define CSTARTADD_12        (12*0x1000u)
#define CSTARTADD_13        (13*0x1000u)
#define CSTARTADD_14        (14*0x1000u)
#define CSTARTADD_15        (15*0x1000u)

#define INCH_0               (0)
#define INCH_1               (1)
#define INCH_2               (2)
#define INCH_3               (3)
#define INCH_4               (4)
#define INCH_5               (5)
#define INCH_6               (6)
#define INCH_7               (7)
#define INCH_8               (8)
#define INCH_9               (9)
#define INCH_10             (10)
#define INCH_11             (11)
#define INCH_12             (12)
#define INCH_13             (13)
#define INCH_14             (14)
#define INCH_15             (15)

#define SREF_0               (0*0x10u)
#define SREF_1               (1*0x10u)
#define SREF_2               (2*0x10u)
#define SREF_3               (3*0x10u)
#define SREF_4               (4*0x10u)
#define SREF_5               (5*0x10u)
#define SREF_6               (6*0x10u)
#define SREF_7               (7*0x10u)
 

/*-------------------------------------------------------------------------
 *   Flash Memory
 *-------------------------------------------------------------------------*/


__no_init volatile union
{
  unsigned short FCTL1;  /* FLASH Control 1 */ 
  
  struct
  {
    unsigned short               : 1; 
    unsigned short ERASE         : 1;  /* Enable bit for Flash segment erase */ 
    unsigned short MERAS         : 1;  /* Enable bit for Flash mass erase */ 
    unsigned short               : 3; 
    unsigned short WRT           : 1;  /* Enable bit for Flash write */ 
    unsigned short BLKWRT        : 1;  /* Enable bit for Flash segment write */ 
    unsigned short KEY           : 8;  /* old definition */ /* Enable bit for Flash segment write */ 
  } FCTL1_bit;  
} @ 0x0128;


enum {
  ERASE               = 0x0002,
  MERAS               = 0x0004,
  WRT                 = 0x0040,
  BLKWRT              = 0x0080,
};


__no_init volatile union
{
  unsigned short FCTL2;  /* FLASH Control 2 */ 
  
  struct
  {
    unsigned short FN0           : 1;  /* Divide Flash clock by 1 to 64 using FN0 to FN5 according to: */ 
    unsigned short FN1           : 1;  /* 32*FN5 + 16*FN4 + 8*FN3 + 4*FN2 + 2*FN1 + FN0 + 1 */ 
    unsigned short FN2           : 1; 
    unsigned short FN3           : 1; 
    unsigned short FN4           : 1; 
    unsigned short FN5           : 1; 
    unsigned short SSEL0         : 1;  /* Flash clock select 0 */ 
    unsigned short SSEL1         : 1;  /* Flash clock select 1 */ 
    unsigned short KEY           : 8; 
  } FCTL2_bit;  
} @ 0x012A;


enum {
  FN0                 = 0x0001,
  FN1                 = 0x0002,
  FN2                 = 0x0004,
  FN3                 = 0x0008,
  FN4                 = 0x0010,
  FN5                 = 0x0020,
};


__no_init volatile union
{
  unsigned short FCTL3;  /* FLASH Control 3 */ 
  
  struct
  {
    unsigned short BUSY          : 1;  /* Flash busy: 1 */ 
    unsigned short KEYV          : 1;  /* Flash Key violation flag */ 
    unsigned short ACCVIFG       : 1;  /* Flash Access violation flag */ 
    unsigned short WAIT          : 1;  /* Wait flag for segment write */ 
    unsigned short LOCK          : 1;  /* Lock bit: 1 - Flash is locked (read only) */ 
    unsigned short EMEX          : 1;  /* Flash Emergency Exit */ 
    unsigned short KEY           : 8; 
    unsigned short               : 2; 
  } FCTL3_bit;  
} @ 0x012C;


enum {
  BUSY                = 0x0001,
  KEYV                = 0x0002,
  ACCVIFG             = 0x0004,
  WAIT                = 0x0008,
  LOCK                = 0x0010,
  EMEX                = 0x0020,
  KEY                 = 0x2000,
};

 
#define __MSP430_HAS_FLASH__         /* Definition to show that Module is available */

#define FRKEY            (0x9600)    /* Flash key returned by read */
#define FWKEY            (0xA500)    /* Flash key for write */
#define FXKEY            (0x3300)    /* for use with XOR instruction */

#define FSSEL_0          (0x0000)    /* Flash clock select: 0 - ACLK */
#define FSSEL_1          (0x0040)    /* Flash clock select: 1 - MCLK */
#define FSSEL_2          (0x0080)    /* Flash clock select: 2 - SMCLK */
#define FSSEL_3          (0x00C0)    /* Flash clock select: 3 - SMCLK */
 


#pragma language=default
#endif  /* __IAR_SYSTEMS_ICC__  */


/************************************************************
* Timer A interrupt vector value
************************************************************/

#define TAIV_CCIFG1         (2)       /* Capture/compare 1 */
#define TAIV_CCIFG2         (4)       /* Capture/compare 2 */
#define TAIV_CCIFG3         (6)       /* Capture/compare 3 */
#define TAIV_CCIFG4         (8)       /* Capture/compare 4 */
#define TAIV_TAIFG          (10)      /* Timer overflow */

/************************************************************
* Interrupt Vectors (offset from 0xFFE0)
************************************************************/

#define PORT2_VECTOR        (1 * 2u)  /* 0xFFE2 Port 2 */
#define USART1TX_VECTOR     (2 * 2u)  /* 0xFFE4 USART 1 Transmit */
#define USART1RX_VECTOR     (3 * 2u)  /* 0xFFE6 USART 1 Receive */
#define PORT1_VECTOR        (4 * 2u)  /* 0xFFE8 Port 1 */
#define TIMERA1_VECTOR      (5 * 2u)  /* 0xFFEA Timer A CC1-2, TA */
#define TIMERA0_VECTOR      (6 * 2u)  /* 0xFFEC Timer A CC0 */
#define ADC12_VECTOR        (7 * 2u)  /* 0xFFEE ADC */
#define USART0TX_VECTOR     (8 * 2u)  /* 0xFFF0 USART 0 Transmit */
#define USART0RX_VECTOR     (9 * 2u)  /* 0xFFF2 USART 0 Receive */
#define WDT_VECTOR          (10 * 2u) /* 0xFFF4 Watchdog Timer */
#define COMPARATORA_VECTOR  (11 * 2u) /* 0xFFF6 Comparator A */
#define TIMERB1_VECTOR      (12 * 2u) /* 0xFFF8 Timer B CC1-6, TB */
#define TIMERB0_VECTOR      (13 * 2u) /* 0xFFFA Timer B CC0 */
#define NMI_VECTOR          (14 * 2u) /* 0xFFFC Non-maskable */
#define RESET_VECTOR        (15 * 2u) /* 0xFFFE Reset [Highest Priority] */

#define UART1TX_VECTOR      USART1TX_VECTOR
#define UART1RX_VECTOR      USART1RX_VECTOR
#define UART0TX_VECTOR      USART0TX_VECTOR
#define UART0RX_VECTOR      USART0RX_VECTOR
#define ADC_VECTOR          ADC12_VECTOR



#endif /* __IO430xxxx */


