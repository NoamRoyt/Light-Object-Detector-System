#ifndef _bsp_H_
#define _bsp_H_

#include  <msp430g2553.h>          // MSP430x2xx
// #include  <msp430xG46x.h>  // MSP430x4xx


#define   debounceVal      250
//#define   LEDs_SHOW_RATE   0xFFFF  // 62_5ms

// RGB abstraction
//#define RGBArrPortOut      P2OUT

// PWM FROM P2.6

#define PWM                 0x10 //p2.4
#define PWMOUT              P2OUT
#define PWMDIR              P2DIR
#define PWMSEL              P2SEL
// for triger
#define triger              0x40 // p2.6
#define trigerOut           P2OUT
#define trigerDir           P2DIR
#define trigerSel           P2SEL

// for echo
#define echo               0x04 // p2.2
#define echoSel            P2SEL
#define echoDer            P2DIR
// UART
#define TXD BIT2
#define RXD BIT1
#define TXLED BIT0
#define RXLED BIT6

#define PB1                0x02  // P1.1
#define PB2                0x04  // P1.2


extern void GPIOconfig(void);
extern void TIMER_A0_config(void);
extern void ADC_config_P0(void);
extern void ADC_config_P3(void);
extern void  cler_ADC(void);
extern void UART_init(void);

#endif



