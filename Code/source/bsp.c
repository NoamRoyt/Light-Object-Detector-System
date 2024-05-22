#include  "../header/bsp.h"    // private library - BSP layer

//-----------------------------------------------------------------------------  
//           GPIO configuration
//-----------------------------------------------------------------------------
void GPIOconfig(void){
 // volatile unsigned int i; // in case of while loop usage
  
  WDTCTL = WDTHOLD | WDTPW;     // Stop WDT
   
 // PWM
  PWMDIR |= PWM;
  PWMSEL &= ~PWM;

  // TRIGER
  trigerDir |= triger;
  trigerSel &= ~triger;

  // ECHO
  echoDer &= ~echo;
  echoSel |= echo;

  // to congi p2.2 to input and sel = 1 i/o dir on zero
/*
  // Generator Setup
  //From the table at CCIx p2.4
  GenPortDir &=  ~BIT4;               // P2.4 Input Capture = '1'
  GenPortSel |=  BIT4;              // P2.4 Select = '1'

  // Buzzer Setup
  BuzzPortDir |= BIT2;             // P2.2 Output compare - '1'
  BuzzPortSel |= BIT2;             // P2.2 Select = '1'
  BuzzPortOut &= ~BIT2;             // P2.2 out = '0'

  // PushButtons Setup
  PBsArrPortSel &= ~0x07;           //
  PBsArrPortOut &= ~0x07;            // Set P1Out to '0'
  PBsArrPortDir &= ~0x07;            // P1.0-2 - Input ('0')
  PBsArrPortDir |= 0x08;             // P1.3 - Output ('1')
  PBsArrIntEdgeSel |= 0x03;          // pull-up mode   P1.0-P1.1 - '1'
  PBsArrIntEdgeSel &= ~0x0C;         // pull-down mode  P1.2 - '0'
  PBsArrIntEn |= 0x07;               // P1.0-2 - '1'
  PBsArrIntPend &= ~0xFF;            // clear pending interrupts P1.0-P1.3 all P1
  
  // PushButton 3 Setup For Main Lab
   PB3sArrPortSel &= ~BIT0;           //
   PB3sArrPortOut &= ~BIT0;            // Set P2Out to '0'
   PB3sArrPortDir &= ~BIT0;            // P2.0 - Input ('0')
   PB3sArrIntEdgeSel &= ~BIT0;         // pull-down mode  P2.0 - '0'
   PB3sArrIntEn |= BIT0;               // P1.0-2 - '1'
   PB3sArrIntPend &= ~BIT0;            // clear pending interrupts P2.0
*/
  _BIS_SR(GIE);                     // enable interrupts globally
  TIMER_A0_config();

}



void TIMER_A0_config(void){
    //PWMDIR |= PWM;
    //PWMSEL &= ~PWM;
    TA0CTL  |= TACLR;
    TA0CCR0 = 3300;// FOR TIME ON 25mes 25X10^-3 * 2^17 = CCR0 (2^17 = SMCLK/8)
    TA0CCTL0 |= CCIE; // LOCAL ENABLE FOR CCR0;
    TA0CCTL1 |= CCIE; // LOCAL ENABLE FOR CCR1
    TA0CCTL0 &= ~CCIFG;
    TA0CCTL1 &= ~CCIFG;

    //TA0CCTL1 = OUTMOD_7;
    //TA0CTL = TASSEL_2 + MC_1;
    TA0CTL  = TASSEL_2 + MC_1+ ID_3;


}

//---------------------------------------------------------------------------------
//            ADC configuration
//-------------------------------------------------------------------------------------
void ADC_config_P3(void){
      ADC10CTL0 = ADC10SHT_2 + ADC10ON+ SREF_0 + ADC10IE;  // 16*ADCLK+ Turn on, set ref to Vcc and Gnd, and Enable Interrupt
      ADC10CTL1 = INCH_3 + ADC10SSEL_3;     // Input A3 and SMCLK, was |
      ADC10AE0 |= BIT3;                         // P1.3 ADC option select
}

void ADC_config_P0(void){
      ADC10CTL0 = ADC10SHT_2 + ADC10ON+ SREF_0 + ADC10IE;  // 16*ADCLK+ Turn on, set ref to Vcc and Gnd, and Enable Interrupt
      ADC10CTL1 = INCH_0 + ADC10SSEL_0;     // Input A3 and SMCLK, was |
      ADC10AE0 |= BIT0;                         // P1.0 ADC option select
}

void cler_ADC(void){
    ADC10CTL0 &= ~ENC; // Disable ADC conversion
    ADC10CTL0 &= ~(ADC10ON | ADC10IFG | ADC10SC); // Turn off ADC, clear interrupt flags, and stop any conversions
    ADC10CTL1 = 0; // Clear ADC10CTL1 settings
    ADC10CTL0 = 0; // Clear ADC10CTL0 settings

}



